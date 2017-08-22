#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: H.L. TIAN 2017

import DroNECore
import DroNECore.PyIncident as PI
import NEON
import json
import CtrlSvc.HeartBeatIncident  as HBI
import CtrlSvc.PushHistIncident   as PHI
import CtrlSvc.PushMatrixIncident as PMI
import CtrlSvc.HelloDroNEIncident as HDI
import CtrlSvc.NeonRPCIncident    as NRI
import CtrlSvc.NeonRPCTask        as NRT
import time


import uuid    
def app(Server, RPC):
    Task = DroNECore.DroNE("task")
    if(not Task.isTop()):
        Task.asTop()
    Task.setLogLevel(2)     
    
###############################################################################
    m_taskheartbeat = NEON.Data.SingleValue(Server, '/GPPD/hearbeat/detector/192.168.0.1:drone01', \
                                           description='192.168.0.1:01')
    m_taskheartbeat.setData(HBI.HeartBeatCronIncident.encodeHeartBeat(status='configuring', \
                                    hit = 0, event = 0, pulse = 0, idx = str(uuid.uuid1())))
    m_taskheartbeat.dump()
###############################################################################

###############################################################################
    xbins = range(0,111*48)
    m_taskhist      = NEON.Data.Hist1D(Server, '/GPPD/workspace/detector/module001', \
                                           xAxis= xbins, data='')
    m_taskhist.dump()
###############################################################################

###############################################################################
    nr = NRT.NeonRPCTask("NeonRPC", RPC, remotedata = m_taskheartbeat)

###############################################################################

    ct = DroNECore.CtrlTask("ctrl")
    di = HDI.HelloIncident('HelloDroNE')
    ni = NRI.NeonRPCCronIncident("NeonRPC", cron = 2, repeatable = True)
    hi = HBI.HeartBeatCronIncident("HeartBeat", cron = 2, repeatable = True, remotedata = m_taskheartbeat)
    #pi = PHI.PushHistCronIncident("PushHist", cron = 2, repeatable = True, remotedata = m_taskhist)
    #pi = PMI.PushMatrixCronIncident("PushMatrix", cron = 5, repeatable = True, remotedata = m_taskhist)
    ct.add(di)
    ct.add(ni)
    ct.add(hi)
    #ct.add(pi)
    

    import DataSvc
    import CtrlSvc

    Task.property("svcs").append("DataSvc")
    Task.property("svcs").append("CtrlSvc")
    Task.property("svcs").append("RawDataInputSvc/DataInputSvc")
    Task.property("svcs").append("FileInputSvc/DataProvideSvc")
    iSvc = Task.find("DataInputSvc")
    iPvd = Task.find("DataProvideSvc")

    filelist = []
    for i in xrange(100):
        filelist.append("16adjust.dat")
    iPvd.property("InputFile").set(filelist) #"N_3Cdmaskslit.dat", 
    iSvc.property("BuffSize").set(5000)

    iSvc.show()
    iPvd.show()

    import Algorithms
    Task.property("algs").append("GPPDSNDRecAlg")
    Task.property("algs").append("GPPDSNDMapAlg")
    Task.property("algs").append("RunningInfAlg")
    #alg = task.find("DumpAlg")

    #    
    Task.setEvtMax(1000000)
    Task.run()
    #Task.run()

    print "stop run"
    index = json.loads(m_taskheartbeat.getData())['uuid']
    hitct = json.loads(m_taskheartbeat.getData())['hit']
    evtct = json.loads(m_taskheartbeat.getData())['event']
    plsct = json.loads(m_taskheartbeat.getData())['pulse']
    m_taskheartbeat.setData(HBI.HeartBeatCronIncident.encodeHeartBeat(status='stopping', \
                                    hit = int(hitct), event = int(evtct), \
                                    pulse = int(plsct), idx = index))
    m_taskheartbeat.dump()
    del Task

def test():
    m_neonRediscli  = NEON.Neon.NeonRedis(host='localhost', port=9999, db = 0, isWritable = False) # server mode, set msgs to Redis
    m_taskheartbeat2= NEON.Data.SingleValue(m_neonRediscli, '/GPPD/hearbeat/detector/192.168.0.1:drone01')
    m_taskheartbeat2.load()
    print m_taskheartbeat2.getData()
    m_taskhist2= NEON.Data.Hist1D(m_neonRediscli, '/GPPD/workspace/detector/module001')
    m_taskhist2.load()
    print m_taskhist2.getData()

class RPCMethods(NEON.Neon.NeonService.NeonRPC.MethodCall):
    def __init__(self, Server, RPC):
        self.Server = Server
        self.RPC    = RPC
        pass

    def Start(self, params=None):
        print "==========================StartDroNE"
        app(Server = self.Server, RPC = self.RPC)    
        
        return "Ready"


def rpcserver(Server, RPC):
    heartbeatdata = HBI.HeartBeatCronIncident.encodeHeartBeat(status='stopping', \
                                           hit = 0, event = 0, pulse = 0, idx = str(uuid.uuid1()))
    m_taskheartbeat = NEON.Data.SingleValue(Server, '/GPPD/hearbeat/detector/192.168.0.1:drone01', \
                                           description='192.168.0.1:01', data=heartbeatdata)
    m_taskheartbeat.dump()
   
    rpcmethod = RPCMethods(Server=Server, RPC=RPC)
    while(True):
        time.sleep(1)
        rpclist = RPC.recvRPC()
        for rpcitem in rpclist:
            neonid, method, parameters = rpcitem
            print "uuid: ", neonid, " method: ", method, " parameters: ", parameters
            RPC.sndRSLT(neonid, "return OK", error = "None ERROR")
            returnValue, errorCode = rpcmethod.execute(method, parameters)

if __name__ == "__main__":
    m_neonRedis = NEON.Neon.NeonRedis(host='localhost', port=9999, db = 0, isWritable = True) # server mode, set msgs to Redis
    servpob = NEON.Neon.NeonService.POBox(m_neonRedis, '/GPPD/process/detector', '192.168.0.1:drone01')
    servrpc = NEON.Neon.NeonService.NeonRPC(sendPOBox = servpob, recvPOBox = servpob, isServer=True)
    #app(Server=m_neonRedis, RPC=servrpc)
    
    rpcserver(Server = m_neonRedis, RPC=servrpc)

    #app(Server = m_neonRedis, Task = task, RPC = servrpc)    
    #test()
    #while(True):
    #    time.sleep(1)
