#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: tianhl

import DroNECore
import DroNECore.PyIncident as PI

class HelloIncident(PI.PyIncident):
    def __init__(self, name):
        super(HelloIncident, self).__init__(name)
        
    def execute(self):         
        return self.fire({"string":" Hello PyIncident ", "times":2})

class HelloCronIncident(PI.PyCronIncident):
    def __init__(self, name, cron = 0, repeatable = False):
        super(HelloCronIncident, self).__init__(name, cron, repeatable)
        
    def execute(self):         
        return self.fire({"string":" Hello PyCronIncident ", "times":2})


if __name__ == "__main__":

    task = DroNECore.DroNE("task")
    task.asTop()
    task.setLogLevel(0)     

    ct = DroNECore.CtrlTask("ctrl")
    hi = HelloIncident('HeartBeat')
    hc = HelloCronIncident("HeartBeat", cron = 2, repeatable = True)
    ct.add(hi)
    ct.add(hc)

    import DataSvc
    import CtrlSvc
    task.property("svcs").append("DataSvc")
    task.property("svcs").append("CtrlSvc")
    task.property("svcs").append("RawDataInputSvc/DataInputSvc")
    task.property("svcs").append("FileInputSvc/DataProvideSvc")
    iSvc = task.find("DataInputSvc")
    iPvd = task.find("DataProvideSvc")

    filelist = ["16adjust.dat"]
    iPvd.property("InputFile").set(filelist) #"N_3Cdmaskslit.dat", 
    iSvc.property("BuffSize").set(5000)

    iSvc.show()
    iPvd.show()

    import Algorithms
    task.property("algs").append("GPPDSNDRecAlg")
    task.property("algs").append("GPPDSNDMapAlg")
    #task.property("algs").append("DumpAlg")
    #alg = task.find("DumpAlg")

    #    
    task.setEvtMax(1000000)
    task.run()

