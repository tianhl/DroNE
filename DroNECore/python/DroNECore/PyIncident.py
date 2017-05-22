#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: tianhl

import DataSvc

class PyIncident(DataSvc.DroNEIncident):
    def __init__(self, name):
        super(PyIncident, self).__init__(name)

    def fire(self, params = None):
        self.clearRetVal()
        self.clearPyDict()
        if(params is not None):
            self.setPyDict(params)
        super(PyIncident, self).fire()
        ret = self.getRetVal()
        self.clearPyDict()
        return ret    

    def execute(self):
        raise Exception("Pure virtual class PyIncident, must be implemented")
        #call self.fire() here


class PyCronIncident(PyIncident):
    def __init__(self, name, cron=0, repeatable = False):
        super(PyCronIncident, self).__init__(name)
        import time
        self.now = time.time()
        if(cron<1):
            self.cron = 1
        else:
            self.cron = cron
        self.repeatable = repeatable
        self.nextfire = self.now + self.cron
        print "PyCronIncident initial time: ", self.now
        print "PyCronIncident next fire at: ", self.nextfire

    def fire(self, params = None):
        import time
        self.now = time.time()
        if(self.now>self.nextfire):
            if(self.repeatable):
                self.nextfire = self.now + self.cron
                print "PyCronIncident next fire at: ", self.nextfire
            return super(PyCronIncident, self).fire(params)

    def execute(self):
        raise Exception("Pure virtual class PyCronIncident, must be implemented")
        #call self.fire() here



