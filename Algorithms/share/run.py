#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: tianhl
# for GPPD offline data processing

import Sniper

if __name__ == "__main__":

    task = Sniper.Task("task")
    task.asTop()
    task.setLogLevel(1)
    
    import DataSvc
    task.property("svcs").append("DataSvc")
    task.property("svcs").append("RawDataInputSvc")
    task.property("svcs").append("FileInputSvc/DataProvideSvc")
    iSvc = task.find("RawDataInputSvc")
    iPvd = task.find("DataProvideSvc")

    filelist = ["20161022_tangb/2016_10_31_16_22_51_192.168.10.18_in.dat"]
    iPvd.property("InputFile").set(filelist) #"N_3Cdmaskslit.dat", 
    iSvc.property("BuffSize").set(5000)

    iSvc.show()
    iPvd.show()

    import Algorithms
    task.property("algs").append("GPPDMAPMTRecAlg")
    task.property("algs").append("GPPDMAPMTMapAlg")
    task.property("algs").append("DumpAlg")
    dumpalg = task.find("DumpAlg")
    dumpalg.property("OutputFileName").set("temp.txt")
    #    
    task.setEvtMax(200000000)
    task.run()
