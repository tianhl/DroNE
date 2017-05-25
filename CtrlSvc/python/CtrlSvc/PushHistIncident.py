#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: tianhl

#import DroNECore
import DroNECore.PyIncident as PI
import NEON
import json


class PushHistCronIncident(PI.PyCronIncident):
    def __init__(self, name, cron = 0, repeatable = False, remotedata = None):
        super(PushHistCronIncident, self).__init__(name, cron, repeatable)
        self.remotedata = remotedata
        
    def execute(self):         
        retval = self.fire()
        if(retval):
            self.remotedata.setData(retval)
            self.remotedata.dump()
        return retval

