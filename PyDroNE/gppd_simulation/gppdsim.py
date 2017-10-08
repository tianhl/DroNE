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
    task.property("svcs").append("GPPDDataSvc/DataSvc")
    task.property("svcs").append("CtrlSvc")
    task.property("svcs").append("SimNeutronGunInputSvc/DataInputSvc")
    iDat = task.find("DataSvc")
    iSvc = task.find("DataInputSvc")

    iSvc.property("InputFileName").set("Sample_refl_D_3.6.txt")
    iSvc.property("MaxTof").set(7375)
    iSvc.property("MaxDet").set(5328)
    iSvc.property("MaxEvt").set(1000)
    iSvc.show()

    import Algorithms
    task.property("algs").append("GPPDSNDFastSimAlg")
    task.property("algs").append("GPPDSNDRecAlg")
    #    
    task.setEvtMax(-1)
    task.run()