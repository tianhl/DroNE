#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: tianhl

import DataSvc

class PyIncident(DataSvc.DroNEIncident):
    def __init__(self, name):
        super(PyIncident, self).__init__(name)

    def fire(self, params = None):
        super(PyIncident, self).clearRetVal()
        if(params is not None):
            self.setPyDict(params)
        super(PyIncident, self).fire()
        ret = super(PyIncident, self).getRetVal()
        super(PyIncident, self).clearPyDict()
        return ret    

