#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: H.L. TIAN 2017

import DroNECore
import DroNECore.PyIncident as PI
import NEON
import json

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

