#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: H.L. TIAN 2017

#import Sniper
import DroNECore
import CtrlSvc
import DataSvc

if __name__ == "__main__":

    ## ===========================
    ## DroNE
    ## ===========================

    task = DroNECore.DroNE("task")
    #task = Sniper.Task("task")
    task.asTop()
    task.setLogLevel(3)
    

    ## ===========================
    task.property("svcs").append("CtrlSvc")
    task.property("svcs").append("GPPDDataSvc/DataSvc")
    task.property("svcs").append("RawDataInputSvc/DataInputSvc")
    task.property("svcs").append("FileInputSvc/DataProvideSvc")
    iDat = task.find("DataSvc")
    iSvc = task.find("DataInputSvc")
    iPvd = task.find("DataProvideSvc")

    filelist = ["data/BL18_RUN119_MODULE02_0000001_2017_11_2_19_31_27.dat"]
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
    task.property("algs").append("SaveMatrix2JSONAlg")

    iMap = task.find("GPPDSNDMapAlg")
    iMap.property("ConfigFileName").set("configure.xml")

    iRun = task.find("RunningInfAlg")
    iRun.property("TofStart").set(0)
    iRun.property("TofStep").set(640)
    iRun.property("TofBins").set(2500)

    iMatrix = task.find("SaveMatrix2JSONAlg")
    iMatrix.property("PIDStart").set(3310001)
    iMatrix.property("PIDSize").set(5328)
    iMatrix.property("OutputFileName").set("json.txt")

    ## ===========================
    task.setEvtMax(-1)
    task.run()
