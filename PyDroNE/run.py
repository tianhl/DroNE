#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: H.L. TIAN 2017

#import Sniper
import DroNECore

if __name__ == "__main__":

    task = DroNECore.DroNE("task")
    #task = Sniper.Task("task")
    task.asTop()
    task.setLogLevel(0)
    
    import DataSvc
    import CtrlSvc
    task.property("svcs").append("DataSvc")
    task.property("svcs").append("CtrlSvc")
    task.property("svcs").append("RawDataInputSvc/DataInputSvc")
    task.property("svcs").append("FileInputSvc/DataProvideSvc")
    iDat = task.find("DataSvc")
    iSvc = task.find("DataInputSvc")
    iPvd = task.find("DataProvideSvc")

    filelist = ["16adjust.dat"]
    #filelist = []
    #for i in xrange(100):
    #    filelist.append("16adjust.dat")
    iPvd.property("InputFile").set(filelist) #"N_3Cdmaskslit.dat", 
    iSvc.property("BuffSize").set(5000)

    iSvc.show()
    iPvd.show()

    import Algorithms
    task.property("algs").append("GPPDSNDRecAlg")
    #task.property("algs").append("GPPDSNDMapAlg")
    #task.property("algs").append("RunningInfAlg")
    #iRun = task.find("RunningInfAlg")
    #iRun.property("TofBins").set(10000)
    #iRun.property("TofStep").set(1000)

    #    
    task.setEvtMax(10000000)
    task.run()
