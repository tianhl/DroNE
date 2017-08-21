#!/usr/bin/env python

# after v05

from NEON.Neon import NeonData 

class SingleValue(NeonData.BasedData, object):
    def __init__(self, remoteDB_backend, name, description = None, data = None):
        self.description = description
        staticDic = {'description':self.description}
        super(SingleValue, self).__init__(remoteDB_backend, name, staticDic, data)

    def getDescription(self):
        return self.description

    def setDescription(self, description):
        self.description = description
        setStatic({'description':self.description})

    def load(self):
        super(SingleValue, self).load()
        staticDic  = self.getStatic()
        self.description = staticDic['description']

class Hist1D(NeonData.BasedData, object):
    # for python 2.7.x, force the class new-style. there is not this problem in python 3.x
    def __init__(self, remoteDB_backend, name, xAxis = None, data = None):
        self.xAxis = xAxis
        staticDic = {'xAxis':self.xAxis}
        super(Hist1D, self).__init__(remoteDB_backend, name, staticDic, data)

    def getXAxis(self):
        return self.xAxis

    def setXAxis(self, xAxis):
        self.xAxis  = xAxis
        setStatic({'xAxis':self.xAxis})

    def load(self):
        super(Hist1D, self).load()
        staticDic  = self.getStatic()
        self.xAxis = staticDic['xAxis']

class Hist2D(NeonData.BasedData, object):
    def __init__(self, remoteDB_backend, name, xAxis = None, yAxis = None, data = None):
        self.xAxis = xAxis
        self.yAxis = yAxis
        staticDic = {'xAxis':self.xAxis, 'yAxis':self.yAxis}
        super(Hist2D, self).__init__(remoteDB_backend, name, staticDic, data)

    def getYAxis(self):
        return self.yAxis

    def setYAxis(self, yAxis):
        self.yAxis  = yAxis
        setStatic({'xAxis':self.xAxis, 'yAxis':self.yAxis})

    def getXAxis(self):
        return self.xAxis

    def setXAxis(self, xAxis):
        self.xAxis  = xAxis
        setStatic({'xAxis':self.xAxis, 'yAxis':self.yAxis})

    def load(self):
        super(Hist2D, self).load()
        staticDic  = self.getStatic()
        self.xAxis = staticDic['xAxis']
        self.yAxis = staticDic['yAxis']

class TimeSeriesData(NeonData.Tuple, object):
    
    def load(self):
        # need to specify the last item in the db
        import time
	super(TimeSeriesData, self).load(time.time())

    def addItem(self, value, time):
        self.addItems([(value, time)])

