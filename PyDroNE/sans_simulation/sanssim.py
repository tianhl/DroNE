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
    task.property("svcs").append("SANSDataSvc/DataSvc")
    task.property("svcs").append("CtrlSvc")
    task.property("svcs").append("SimNeutronGunInputSvc/DataInputSvc")
    iDat = task.find("DataSvc")
    iSvc = task.find("DataInputSvc")

    iSvc.property("InputFileName").set("sample_sans_D.txt")
    iSvc.property("MaxTof").set(4999)
    iSvc.property("MaxDet").set(6400)
    iSvc.property("MaxEvt").set(1000)
    iSvc.show()

    import Algorithms
    task.property("algs").append("SANSHe3TFastSimAlg")
        
    task.setEvtMax(-1)
    task.run()
