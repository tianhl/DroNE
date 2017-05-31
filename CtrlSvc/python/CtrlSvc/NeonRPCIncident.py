#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: H.L. TIAN 2017

#import DroNECore
import DroNECore.PyIncident as PI
import NEON
import json


class NeonRPCCronIncident(PI.PyCronIncident):
    def __init__(self, name, cron = 0, repeatable = False):
        super(NeonRPCCronIncident, self).__init__(name, cron, repeatable)
        
    def execute(self):         
        return self.fire()
        
