#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: tianhl
# for GPPD online data processing

import Sniper

if __name__ == "__main__":

    task = Sniper.Task("task")
    task.asTop()
    task.setLogLevel(0)
    
    import DataSvc
    task.property("svcs").append("DataSvc")
    task.property("svcs").append("RawDataInputSvc")
    task.property("svcs").append("DimRecvSvc/DataProvideSvc")
    iSvc = task.find("RawDataInputSvc")
    iPvd = task.find("DataProvideSvc")

    iPvd.property("DataSize").set(10)
    iSvc.property("BuffSize").set(10)

    iSvc.show()
    iPvd.show()

    import Algorithms
    task.property("algs").append("GPPDMAPMTRecAlg")
    task.property("algs").append("GPPDMAPMTMapAlg")
    task.property("algs").append("DumpAlg")
    dumpalg = task.find("DumpAlg")
    dumpalg.property("OutputFileName").set("temp.txt")

    task.setEvtMax(-1)
    task.initialize()
    #import time
    #time.sleep(30)
    task.run()
