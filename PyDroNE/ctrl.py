#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: tianhl

import DroNECore
import DroNECore.PyIncident as PI
import NEON
import json
import CtrlSvc.HeartBeatIncident as HBI
import CtrlSvc.HelloDroNEIncident as HDI
import CtrlSvc.PushHistIncident as PHI


if __name__ == "__main__":

    task = DroNECore.DroNE("task")
    task.asTop()
    task.setLogLevel(0)     
    
    import uuid    
    heartbeatdata = HBI.HeartBeatCronIncident.encodeHeartBeat(status='configuring', \
                                           hit = 0, event = 0, pulse = 0, idx = str(uuid.uuid1()))
    m_neonRedis     = NEON.Neon.NeonRedis(host='localhost', port=9999, db = 0, isWritable = True) # server mode, set msgs to Redis
    m_taskheartbeat = NEON.Data.SingleValue(m_neonRedis, '/GPPD/hearbeat/detector/192.168.0.1:drone01', \
                                           description='192.168.0.1:01', data=heartbeatdata)
    m_taskheartbeat.dump()

    xbins = range(0,111*48)
    m_taskhist      = NEON.Data.Hist1D(m_neonRedis, '/GPPD/workspace/detector/module001', \
                                           xAxis= xbins, data='')
    m_taskhist.dump()

    ct = DroNECore.CtrlTask("ctrl")
    #di = HDI.HelloIncident('HelloDroNE')
    hi = HBI.HeartBeatCronIncident("HeartBeat", cron = 2, repeatable = True, remotedata = m_taskheartbeat)
    pi = PHI.PushHistCronIncident("PushHist", cron = 2, repeatable = True, remotedata = m_taskhist)
    #ct.add(di)
    ct.add(hi)
    ct.add(pi)
    #ct.add(hc)
    import CtrlSvc
    import DataSvc

    task.property("svcs").append("DataSvc")
    task.property("svcs").append("CtrlSvc")
    task.property("svcs").append("RawDataInputSvc/DataInputSvc")
    task.property("svcs").append("FileInputSvc/DataProvideSvc")
    iSvc = task.find("DataInputSvc")
    iPvd = task.find("DataProvideSvc")

    filelist = ["16adjust.dat"]
    iPvd.property("InputFile").set(filelist) #"N_3Cdmaskslit.dat", 
    iSvc.property("BuffSize").set(5000)

    iSvc.show()
    iPvd.show()

    import Algorithms
    task.property("algs").append("GPPDSNDRecAlg")
    task.property("algs").append("GPPDSNDMapAlg")
    task.property("algs").append("RunningInfAlg")
    #alg = task.find("DumpAlg")

    #    
    task.setEvtMax(1000000)
    task.run()

    index = json.loads(m_taskheartbeat.getData())['uuid']
    hitct = json.loads(m_taskheartbeat.getData())['hit']
    evtct = json.loads(m_taskheartbeat.getData())['event']
    plsct = json.loads(m_taskheartbeat.getData())['pulse']
    m_taskheartbeat.setData(HBI.HeartBeatCronIncident.encodeHeartBeat(status='stopping', \
                                    hit = int(hitct), event = int(evtct), \
                                    pulse = int(plsct), idx = index))
    m_taskheartbeat.dump()

    m_neonRediscli  = NEON.Neon.NeonRedis(host='localhost', port=9999, db = 0, isWritable = False) # server mode, set msgs to Redis
    m_taskheartbeat2= NEON.Data.SingleValue(m_neonRediscli, '/GPPD/hearbeat/detector/192.168.0.1:drone01')
    m_taskheartbeat2.load()
    print m_taskheartbeat2.getData()
    m_taskhist2= NEON.Data.Hist1D(m_neonRediscli, '/GPPD/workspace/detector/module001')
    m_taskhist2.load()
    print m_taskhist2.getData()

