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
import CtrlSvc.NeonUpdateDataTask as NUT
import CtrlSvc.PushMatrixIncident as PMI

if __name__ == "__main__":

    # ===========================
    # HeartBeat
    # ===========================
    import socket, os
    m_taskheartbeat = NEON.Data.RedisRemoteData("127.0.0.1", 9999, \
                    '/GPPD/heartbeat/detector/127.0.0.1:test')
    m_taskheartbeat.setData(HBI.HeartBeatCronIncident.encodeHeartBeat(status='configuring', \
                                    hit = 0, event = 0, pulse = 0, idx = os.getpid()))
    m_taskheartbeat.dump()

    # ===========================
    # DroNE
    # ===========================

    task = DroNECore.DroNE("task")
    task.asTop()
    task.setLogLevel(3)
    
    # ===========================
    m_neonRedis = NEON.Neon.NeonRedis(host='127.0.0.1', port=9999, db = 0, isWritable = True) # server mode, set msgs to Redis
    servpob = NEON.Neon.NeonService.POBox(m_neonRedis, '/GPPD/process/detector', '192.168.0.1:drone01')
    servrpc = NEON.Neon.NeonService.NeonRPC(sendPOBox = servpob, recvPOBox = servpob, isServer=True)
    nr = NRT.NeonRPCTask("NeonRPC", servrpc, remotedata = m_taskheartbeat)

    m_taskMatrix = NEON.Data.LocalASCIIData("test.txt")
    m_configure = {"pidstart":3310001,"pidsize":5328}
    nu = NUT.NeonUpdateDataTask("UpdateRemoteData", remotedata = m_taskMatrix, configure=m_configure)

    ct = DroNECore.CtrlTask("ctrl")
    hi = HBI.HeartBeatCronIncident("HeartBeat", cron = 2, repeatable = True, remotedata = m_taskheartbeat)
    ni = NRI.NeonRPCCronIncident("NeonRPC", cron = 2, repeatable = True)
    pi = PMI.PushMatrixCronIncident("PushMatrix", cron = 15, \
                                    repeatable = True, remotedata = m_taskMatrix,\
                                    configure = m_configure)
    ct.add(hi)
    ct.add(ni)
    ct.add(pi)

    # ===========================
    task.property("svcs").append("CtrlSvc")
    task.property("svcs").append("GPPDDataSvc/DataSvc")
    task.property("svcs").append("RawDataInputSvc/DataInputSvc")
    task.property("svcs").append("FileInputSvc/DataProvideSvc")
    iDat = task.find("DataSvc")
    iSvc = task.find("DataInputSvc")
    iPvd = task.find("DataProvideSvc")

    filelist = []
    datafile = "data/BL18_RUN119_MODULE02_0000001_2017_11_2_19_31_27.dat"
    for i in xrange(5):
      filelist.append(datafile)
    iPvd.property("InputFile").set(filelist) #"N_3Cdmaskslit.dat", 
    iSvc.property("BuffSize").set(5000)

    iSvc.show()
    iPvd.show()

    ## ===========================
    import Algorithms
    task.property("algs").append("GPPDSNDRecAlg")
    task.property("algs").append("GPPDSNDMapAlg")
    task.property("algs").append("RunningInfAlg")
    #task.property("algs").append("DumpAlg")

    iMap = task.find("GPPDSNDMapAlg")
    iMap.property("ConfigFileName").set("configure.xml")

    iRun = task.find("RunningInfAlg")
    iRun.property("TofStart").set(0)
    iRun.property("TofBins").set(640)
    iRun.property("TofStep").set(2500)

    ## ===========================
    task.setEvtMax(-1)
    task.run()
