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
    task.setLogLevel(4)
    

    ## ===========================
    task.property("svcs").append("CtrlSvc")
    task.property("svcs").append("MRDataSvcV02/DataSvc")
    task.property("svcs").append("MWPCRawDataInputSvc/DataInputSvc")
    task.property("svcs").append("FileInputSvc/DataProvideSvc")
    iDat = task.find("DataSvc")
    iSvc = task.find("DataInputSvc")
    iPvd = task.find("DataProvideSvc")

    #filelist = ["data/pack220180623212321.dat","data/pack220180623212321.dat"]
    filelist = ['data/pack220180628102524.dat']
    #filelist = ["data/dimstorefile.dat"]
    #filelist = ["data/dimstorefile.dat","data/dimstorefile.dat"]
    iPvd.property("InputFile").set(filelist) #"N_3Cdmaskslit.dat", 
    iSvc.property("BuffSize").set(5000)

    iSvc.show()
    iPvd.show()

    ## ===========================
    import Algorithms
    task.property("algs").append("MRMWPCRecAlg")
    task.property("algs").append("MRMWPCMapAlg")
    task.property("algs").append("DumpAlg")
    iAlg = task.find('DumpAlg')
    iAlg.property('IntExample').set(10)

    ## ===========================
    task.setEvtMax(-1)
    task.run()
