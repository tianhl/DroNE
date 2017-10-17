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
    task.setLogLevel(0)
    

    ## ===========================
    task.property("svcs").append("CtrlSvc")
    task.property("svcs").append("SANSDataSvc/DataSvc")
    task.property("svcs").append("He3TRawDataInputSvc/DataInputSvc")
    task.property("svcs").append("FileInputSvc/DataProvideSvc")
    iDat = task.find("DataSvc")
    iSvc = task.find("DataInputSvc")
    iPvd = task.find("DataProvideSvc")

    filelist = ["He3_rawdata_bin_10.1.34.60_24_20171016160128.dat"]
    iPvd.property("InputFile").set(filelist) #"N_3Cdmaskslit.dat", 
    iSvc.property("BuffSize").set(5000)

    iSvc.show()
    iPvd.show()

    ## ===========================
    import Algorithms
    #task.property("algs").append("SANSHe3TRecAlg")
    #task.property("algs").append("SANSHe3TMapAlg")
    #task.property("algs").append("RunningInfAlg")
    #task.property("algs").append("DumpAlg")

    #iMap = task.find("GPPDSNDMapAlg")
    #iMap.property("ConfigFileName").set("configure.xml")

    #iRun = task.find("RunningInfAlg")
    #iRun.property("TofBins").set(10000)
    #iRun.property("TofStep").set(1000)

    ## ===========================
    task.setEvtMax(-1)
    task.run()
