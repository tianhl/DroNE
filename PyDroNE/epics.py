#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: H.L. TIAN 2017


import Sniper

if __name__ == "__main__":

    task = Sniper.Task("task")
    task.asTop()
    task.setLogLevel(0)
    
    import DataSvc
    task.property("svcs").append("DataSvc")
    task.property("svcs").append("EpicsInputSvc/DataInputSvc")
    iSvc = task.find("DataInputSvc")

    import Algorithms
    task.property("algs").append("DumpAlg")

    iSvc.show()

    task.setEvtMax(-1)
    task.initialize()
    task.run()
