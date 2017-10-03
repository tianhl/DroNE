#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: H.L. TIAN 2017

#import Sniper
import DroNECore
import DroNECore.PyIncident as PI
import CtrlSvc.PushMatrixIncident as PMI
import CtrlSvc.PushHistIncident as PHI
import CtrlSvc.HeartBeatIncident  as HBI
import redis
import json
import uuid


class RedisRemoteData:
    def __init__(self, ip, port, path):
       self.server = redis.Redis(ip, port, db=0)
       self.path   = path

    def setData(self, data):
       self.data = data

    def dump(self):
       self.server.set(self.path, self.data)

    def getData(self):
       return self.data 
 
    def load(self):
       self.data = self.server.get(self.path)

def App(in_module='', in_ip='', in_port=0, in_cron=1):

    m_modulename = in_module#'module131'
    m_ipadress   = in_ip#'10.1.33.141'
    m_port       = in_port#9000
    m_cron       = in_cron#15

    from xml.etree import ElementTree as ET
    root = ET.parse('configure.xml')
    tof  = root.find('tofInformation')
    factor = 40  # 25ns to 1us
    m_tofstart= int(tof.attrib['tofstart'])*factor
    m_tofstep = int(tof.attrib['tofstep'])*factor 
    m_tofbins = int(tof.attrib['tofbin'])

    for item in root.findall('type/component'):
      if m_modulename== item.attrib['name']:
        m_moduleinfo = item.attrib
        
    #xbins = range(0,int(m_moduleinfo['pixels']))
    path  = '/GPPD/workspace/detector/'+m_modulename
    print path
    m_taskMatrix = RedisRemoteData(m_ipadress, m_port, path+"/value")
    m_taskTOF    = RedisRemoteData(m_ipadress, m_port, path+"/tof")
    m_taskPID    = RedisRemoteData(m_ipadress, m_port, path+"/pid")
    toflist = [m_tofstep+i*m_tofstep for i in xrange(m_tofbins+1)]
    m_taskTOF.setData(json.dumps(toflist))
    m_taskTOF.dump()

    import socket, os
    m_taskheartbeat = RedisRemoteData(m_ipadress, m_port, \
                    '/GPPD/heartbeat/detector/'+socket.gethostbyname(socket.gethostname())+':'+m_modulename)
    m_taskheartbeat.setData(HBI.HeartBeatCronIncident.encodeHeartBeat(status='configuring', \
                                    hit = 0, event = 0, pulse = 0, idx = os.getpid()))
    m_taskheartbeat.dump()
    

    idstart = int(m_moduleinfo['idstart'])
    idsize  = int(m_moduleinfo['pixels'])
    pidlist = [i+idstart for i in xrange(idsize)]
    print pidlist
    m_taskPID.setData(json.dumps(pidlist))
    m_taskPID.dump()

    m_configure = {"pidstart":idstart,"pidsize":idsize}
    task = DroNECore.DroNE("task")
    task.asTop()
    task.setLogLevel(0)
    
    import CtrlSvc
    ct = DroNECore.CtrlTask("ctrl")
    pi = PMI.PushMatrixCronIncident("PushMatrix", cron = m_cron, \
                                    repeatable = True, remotedata = m_taskMatrix,\
                                    configure = m_configure)
    hi = HBI.HeartBeatCronIncident("HeartBeat", cron = 2, repeatable = True, remotedata = m_taskheartbeat)
    ct.add(pi)
    ct.add(hi)

    import DataSvc
    task.property("svcs").append("GPPDDataSvc/DataSvc")
    task.property("svcs").append("CtrlSvc")
    task.property("svcs").append("RawDataInputSvc/DataInputSvc")
    #task.property("svcs").append("FileInputSvc/DataProvideSvc")
    task.property("svcs").append("DimRecvSvc/DataProvideSvc")
    iDat = task.find("DataSvc")
    iSvc = task.find("DataInputSvc")
    iPvd = task.find("DataProvideSvc")

    #filelist = ["16adjust.dat"]
    #for i in xrange(100):
    #    filelist.append("16adjust.dat")
    #iPvd.property("InputFile").set(filelist) 
    iPvd.property("DataSize").set(1000000)
    #dimsrvname = "dimserver/TEST_SWAP_"+m_moduleinfo['moduleNum']
    dimsrvname = "dimserver"+m_moduleinfo['server']+"/TEST_SWAP_"+m_moduleinfo['num']
    iPvd.property("DimServerName").set(dimsrvname)
    #:dimsrvname = '111/TEST_SWAP'
    print dimsrvname
    iSvc.property("BuffSize").set(1000000)

    iSvc.show()
    iPvd.show()

    import Algorithms
    task.property("algs").append("GPPDSNDRecAlg")
    task.property("algs").append("GPPDSNDMapAlg")
    task.property("algs").append("RunningInfAlg")
    task.property("algs").append("DumpAlg")
    iRun = task.find("RunningInfAlg")
    iRun.property("TofStart").set(m_tofstart)
    iRun.property("TofBins").set(m_tofbins)
    iRun.property("TofStep").set(m_tofstep)
    iMap = task.find("GPPDSNDMapAlg")
    iMap.property("ConfigFileName").set("configure.xml")

    #    
    task.setEvtMax(-1)
    task.run()

if __name__ == "__main__":
    import sys
    #App(in_module='module131', in_ip='10.1.33.141', in_port=9000, in_cron=15)
    App(in_module=sys.argv[1], in_ip='10.1.33.141', in_port=9000, in_cron=15)
