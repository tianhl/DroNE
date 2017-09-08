#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: H.L. TIAN 2017


import DroNECore

if __name__ == "__main__":

    task = DroNECore.DroNE("task")
    task.asTop()
    task.setLogLevel(0)
    
    import DataSvc
    import CtrlSvc
    task.property("svcs").append("DataSvc")
    task.property("svcs").append("CtrlSvc")
    task.property("svcs").append("EpicsInputSvc/DataInputSvc")
    iSvc = task.find("DataInputSvc")
    iSvc.property("Channel").set("neutrons")

    import Algorithms
    task.property("algs").append("DumpAlg")

    iSvc.show()

    task.setEvtMax(-1)
    task.initialize()
    task.run()
