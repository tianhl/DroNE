#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

# using the new Mgr.

import DataSvc
import Sniper
import CtrlTask

if __name__ == "__main__":

    task = DataSvc.DroNE("task")
    task.asTop()
    task.setLogLevel(0)     

    ct = CtrlTask.CtrlTask("ctrl")
    #ct.run()

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
    task.setEvtMax(10)
    task.run()

    # cpp call pyTask
    i = Sniper.Incident("CtrlTaskSayHi")
    print i.name()
    i.fire()

    # py call cppHdl with return value
    h = DataSvc.DroNEIncident("task:HeartBeat")
    print h.name()
    h.fire()
    print "without dict: ", h.getRetVal()

    # py call cppHdl with return value
    print h.setPyDict({"times":3,"string":"Hello World"})
    h.fire()
    print h.getRetVal()
