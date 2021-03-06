#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: H.L. TIAN 2017

#import Sniper
import DroNECore
import CtrlSvc
import DataSvc
import NEON
import CtrlSvc.HeartBeatIncident  as HBI

if __name__ == "__main__":

    ## ===========================
    ## HeartBeat
    ## ===========================
    import socket, os
    m_taskheartbeat = NEON.Data.RedisRemoteData("127.0.0.1", 9999, \
                    '/GPPD/heartbeat/detector/127.0.0.1:test')
    m_taskheartbeat.setData(HBI.HeartBeatCronIncident.encodeHeartBeat(status='configuring', \
                                    hit = 0, event = 0, pulse = 0, idx = os.getpid()))
    m_taskheartbeat.dump()

    ## ===========================
    ## DroNE
    ## ===========================

    task = DroNECore.DroNE("task")
    #task = Sniper.Task("task")
    task.asTop()
    task.setLogLevel(0)
    
    ## ===========================
    ct = DroNECore.CtrlTask("ctrl")
    hi = HBI.HeartBeatCronIncident("HeartBeat", cron = 2, repeatable = True, remotedata = m_taskheartbeat)
    ct.add(hi)

    ## ===========================
    task.property("svcs").append("CtrlSvc")
    task.property("svcs").append("GPPDDataSvc/DataSvc")
    task.property("svcs").append("RawDataInputSvc/DataInputSvc")
    task.property("svcs").append("FileInputSvc/DataProvideSvc")
    iDat = task.find("DataSvc")
    iSvc = task.find("DataInputSvc")
    iPvd = task.find("DataProvideSvc")

    filelist = ["16adjust.dat"]
    iPvd.property("InputFile").set(filelist) #"N_3Cdmaskslit.dat", 
    iSvc.property("BuffSize").set(5000)

    iSvc.show()
    iPvd.show()

    ## ===========================
    import Algorithms
    task.property("algs").append("GPPDSNDRecAlg")
    task.property("algs").append("GPPDSNDMapAlg")
    task.property("algs").append("RunningInfAlg")
    task.property("algs").append("DumpAlg")

    iMap = task.find("GPPDSNDMapAlg")
    iMap.property("ConfigFileName").set("configure.xml")

    iRun = task.find("RunningInfAlg")
    iRun.property("TofBins").set(10000)
    iRun.property("TofStep").set(1000)

    ## ===========================
    task.setEvtMax(10000000)
    task.run()
