#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: H.L. TIAN 2017

# using the new Mgr.

import DroNECore

if __name__ == "__main__":

    task = Sniper.Task("task")
    task.asTop()
    task.setLogLevel(0)
    
    import DataSvc
    task.property("svcs").append("SANSDataSvc/DataSvc")
    task.property("svcs").append("He3TRawDataInputSvc/DataInputSvc")
    task.property("svcs").append("DimRecvSvc/DataProvideSvc")
    iSvc = task.find("DataInputSvc")
    iPvd = task.find("DataProvideSvc")

    iPvd.property("DataSize").set(1000000)
    dimsrvname = "dimserver/TEST_SWAP_1"
    iPvd.property("DimServerName").set(dimsrvname)
    iSvc.property("BuffSize").set(1000000)

    iSvc.show()
    iPvd.show()

    import Algorithms
    task.property("algs").append("SANSHe3TRecAlg")
    task.property("algs").append("SANSHe3TMapAlg")
    #task.property("algs").append("DumpAlg")
    #task.property("algs").append("SaveNeXusAlg")
    #dumpalg = task.find("DumpAlg")
    #dumpalg.property("OutputFileName").set("tempdim.txt")
    #dumpalg.property("initXNum").set(111)
    #dumpalg.property("initYNum").set(48)

    task.setEvtMax(-1)
    task.initialize()
    import time
    time.sleep(1)
    task.run()
