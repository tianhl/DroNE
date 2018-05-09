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
    task.property("svcs").append("MRDataSvcV02/DataSvc")
    task.property("svcs").append("MWPCRawDataInputSvc/DataInputSvc")
    task.property("svcs").append("FileInputSvc/DataProvideSvc")
    iDat = task.find("DataSvc")
    iSvc = task.find("DataInputSvc")
    iPvd = task.find("DataProvideSvc")

    filelist = ["data/HE3_rawdata_bin_10.1.31.70_24_20171011145145.dat"]
    iPvd.property("InputFile").set(filelist) #"N_3Cdmaskslit.dat", 
    iSvc.property("BuffSize").set(5000)

    iSvc.show()
    iPvd.show()

    ## ===========================
    import Algorithms

    ## ===========================
    task.setEvtMax(-1)
    task.run()
