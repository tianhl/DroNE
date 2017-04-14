#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

# using the new Mgr.

import Sniper
import DataSvc

if __name__ == "__main__":

    #task = Sniper.Task("task")
    #task.asTop()
    #task.setLogLevel(0)
 
    #tmp = Sniper.Task("tmp")
    task = DataSvc.DroNE("dd")
    
    #task = tmp.createTask("DroNE")
    task.asTop()
    task.setLogLevel(0)     

    #import DataSvc
    task.property("svcs").append("DataSvc")
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
    #task.property("algs").append("DumpAlg")
    #alg = task.find("DumpAlg")

    #    
    task.setEvtMax(10000000)
    task.run()
