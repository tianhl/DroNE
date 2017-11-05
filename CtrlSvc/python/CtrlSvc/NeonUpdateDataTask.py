#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: H.L. TIAN 2017

from libSniperPython import Task
#from libSniperPython import Incident
#import PyIncident
import NEON
import DroNECore.PyIncident as PI
import json

class NeonUpdateDataTask(Task) :

    def __init__(self, name, remotedata = None, configure = None) :
        Task.__init__(self, name)
        Task.regist(self, "UpdateRemoteData")
        self.remotedata = remotedata
        self.configure  = configure
        self.incident   = PI.PyIncident("PushMatrix")

    def handle(self, incident) :
        retval = self.incident.fire(self.configure)
        if(retval):
            self.remotedata.setData(retval)
            self.remotedata.dump()
