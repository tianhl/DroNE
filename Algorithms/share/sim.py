#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: tianhl
# REFL detector simulation

import Sniper

if __name__ == "__main__":

    task = Sniper.Task("task")
    task.asTop()
    task.setLogLevel(1)
    
    import DataSvc
    task.property("svcs").append("DataSvc")
    iSvc = task.find("DataSvc")
    iSvc.property("SimMode").set(True)

    iSvc.show()

    import Algorithms
    task.property("algs").append("REFLMAPMTSimAlg")
    alg = task.find("REFLMAPMTSimAlg")
    alg.property("InputFileName").set("/home/tianhl/workarea/Codes/GPPD_CODE/sniper/Algorithms/share/mr_data_new/REFL0.3deg/Direct_D_0.3.txt")
    alg.property("MaxTof").set(2500)
    alg.property("MaxDet").set(6400)
    #    
    task.setEvtMax(100000000)
    task.run()
