#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: H.L. TIAN 2017

#import Sniper
import DroNECore
import DroNECore.PyIncident as PI
import CtrlSvc.PushMatrixIncident as PMI
import CtrlSvc.PushHistIncident as PHI
import redis
import json


class RedisRemoteData:
    def __init__(self, ip, port, path):
       self.server = redis.Redis(ip, port, db=0)
       self.path   = path

    def setData(self, data):
       self.data = data

    def dump(self):
       self.server.set(self.path, self.data)

if __name__ == "__main__":

    m_tofbins = 1000
    m_tofstep = 10000
    xbins = range(0,111*48)
    m_taskMatrix = RedisRemoteData("10.1.53.240", 9000, "/GPPD/workspace/detector/module131/value")
    m_taskTOF    = RedisRemoteData("10.1.53.240", 9000, "/GPPD/workspace/detector/module131/tof")
    m_taskPID    = RedisRemoteData("10.1.53.240", 9000, "/GPPD/workspace/detector/module131/pid")
    toflist = [i*m_tofstep for i in xrange(m_tofbins+1)]
    m_taskTOF.setData(json.dumps(toflist))
    m_taskTOF.dump()
    pidlist = [i+1310001 for i in xrange(5328)]
    m_taskTOF.setData(json.dumps(toflist))
    m_taskTOF.dump()

    task = DroNECore.DroNE("task")
    task.asTop()
    task.setLogLevel(0)
    
    import CtrlSvc
    ct = DroNECore.CtrlTask("ctrl")
    pi = PMI.PushMatrixCronIncident("PushMatrix", cron = 15, repeatable = True, remotedata = m_taskMatrix)
    ct.add(pi)

    import DataSvc
    task.property("svcs").append("DataSvc")
    task.property("svcs").append("CtrlSvc")
    task.property("svcs").append("RawDataInputSvc/DataInputSvc")
    task.property("svcs").append("FileInputSvc/DataProvideSvc")
    iDat = task.find("DataSvc")
    iSvc = task.find("DataInputSvc")
    iPvd = task.find("DataProvideSvc")

    filelist = ["16adjust.dat"]
    for i in xrange(100):
        filelist.append("16adjust.dat")
    iPvd.property("InputFile").set(filelist) #"N_3Cdmaskslit.dat", 
    iSvc.property("BuffSize").set(5000)

    iSvc.show()
    iPvd.show()

    import Algorithms
    task.property("algs").append("GPPDSNDRecAlg")
    task.property("algs").append("GPPDSNDMapAlg")
    task.property("algs").append("RunningInfAlg")
    iRun = task.find("RunningInfAlg")
    iRun.property("TofBins").set(m_tofbins)
    iRun.property("TofStep").set(m_tofstep)

    #    
    task.setEvtMax(10000000)
    task.run()
