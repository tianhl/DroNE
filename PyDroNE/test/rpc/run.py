#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: H.L. TIAN 2017

#import Sniper
import DroNECore
import CtrlSvc
import DataSvc
import NEON
import CtrlSvc.HeartBeatIncident  as HBI
import CtrlSvc.NeonRPCIncident    as NRI
import CtrlSvc.NeonRPCTask        as NRT

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
    m_neonRedis = NEON.Neon.NeonRedis(host='127.0.0.1', port=9999, db = 0, isWritable = True) # server mode, set msgs to Redis
    servpob = NEON.Neon.NeonService.POBox(m_neonRedis, '/GPPD/process/detector', '192.168.0.1:drone01')
    servrpc = NEON.Neon.NeonService.NeonRPC(sendPOBox = servpob, recvPOBox = servpob, isServer=True)
    nr = NRT.NeonRPCTask("NeonRPC", servrpc, remotedata = m_taskheartbeat)

    ct = DroNECore.CtrlTask("ctrl")
    hi = HBI.HeartBeatCronIncident("HeartBeat", cron = 2, repeatable = True, remotedata = m_taskheartbeat)
    ni = NRI.NeonRPCCronIncident("NeonRPC", cron = 2, repeatable = True)
    ct.add(hi)
    ct.add(ni)

    ## ===========================
    task.property("svcs").append("CtrlSvc")
    task.property("svcs").append("GPPDDataSvc/DataSvc")
    task.property("svcs").append("RawDataInputSvc/DataInputSvc")
    task.property("svcs").append("FileInputSvc/DataProvideSvc")
    iDat = task.find("DataSvc")
    iSvc = task.find("DataInputSvc")
    iPvd = task.find("DataProvideSvc")

    filelist = []
    filelist.append("RUN824/BL18_RUN824_MODULE02_0000001_2017_10_22_17_18_51.dat")
    filelist.append("RUN824/BL18_RUN824_MODULE03_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE04_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE05_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE06_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE07_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE08_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE09_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE10_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE11_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE12_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE13_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE14_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE15_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE16_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE17_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE18_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE19_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE20_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE21_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE22_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE23_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE24_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE25_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE27_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE28_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE29_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE30_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE31_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE32_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE33_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE34_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE35_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE36_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE37_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE38_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE39_0000001_2017_10_22_17_18_51.dat") 
    filelist.append("RUN824/BL18_RUN824_MODULE40_0000001_2017_10_22_17_18_51.dat") 
    iPvd.property("InputFile").set(filelist) #"N_3Cdmaskslit.dat", 
    iSvc.property("BuffSize").set(5000)

    iSvc.show()
    iPvd.show()

    ## ===========================
    import Algorithms
    task.property("algs").append("GPPDSNDRecAlg")
    task.property("algs").append("GPPDSNDMapAlg")
    task.property("algs").append("RunningInfAlg")

    iMap = task.find("GPPDSNDMapAlg")
    iMap.property("ConfigFileName").set("configure.xml")

    iRun = task.find("RunningInfAlg")
    iRun.property("TofBins").set(10000)
    iRun.property("TofStep").set(1000)

    ## ===========================
    task.setEvtMax(-1)
    task.run()
