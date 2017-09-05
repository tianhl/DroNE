#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: H.L. TIAN 2017

#import DroNECore
import DroNECore.PyIncident as PI
import NEON
import json


class PushMatrixCronIncident(PI.PyCronIncident):
    def __init__(self, name, cron = 0, repeatable = False, remotedata = None, configure = None):
        super(PushMatrixCronIncident, self).__init__(name, cron, repeatable)
        self.remotedata = remotedata
        self.configure  = configure
        
    def execute(self):         
        #retval = self.fire({"pidstart":1110001,"pidsize":5328})
        retval = self.fire(self.configure)
        if(retval):
            self.remotedata.setData(retval)
            self.remotedata.dump()
        return retval

