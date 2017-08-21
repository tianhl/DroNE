#!/usr/bin/env python

# after v05

import NEON
import numpy
import time

#class Hist2D(NeonData.BasedData, object):
#    # for python 2.7.x, force the class new-style. there is not this problem in python 3.x
#    def __init__(self, remoteDB_backend, name, xAxis = None, data = None):
#        self.xAxis = xAxis
#        staticDic = {'xAxis':self.xAxis}
#        super(Hist2D, self).__init__(remoteDB_backend, name, staticDic, data)
#
#    def getXAxis(self):
#        return self.xAxis
#
#    def setXAxis(self, xAxis):
#        self.xAxis  = xAxis
#        setStatic({'xAxis':self.xAxis})
#
#    def load(self):
#        super(Hist2D, self).load()
#        staticDic  = self.getStatic()
#        self.xAxis = staticDic['xAxis']
#
#class Hist3D(NeonData.BasedData, object):
#    def __init__(self, remoteDB_backend, name, xAxis = None, yAxis = None, data = None):
#        self.xAxis = xAxis
#        self.yAxis = yAxis
#        staticDic = {'xAxis':self.xAxis, 'yAxis':self.yAxis}
#        super(Hist3D, self).__init__(remoteDB_backend, name, staticDic, data)
#
#    def getYAxis(self):
#        return self.yAxis
#
#    def setYAxis(self, yAxis):
#        self.yAxis  = yAxis
#        setStatic({'xAxis':self.xAxis, 'yAxis':self.yAxis})
#
#    def getXAxis(self):
#        return self.xAxis
#
#    def setXAxis(self, xAxis):
#        self.xAxis  = xAxis
#        setStatic({'xAxis':self.xAxis, 'yAxis':self.yAxis})
#
#    def load(self):
#        super(Hist3D, self).load()
#        staticDic  = self.getStatic()
#        self.xAxis = staticDic['xAxis']
#        self.yAxis = staticDic['yAxis']
#
#class TimeSeriesData(NeonData.Tuple, object):
#    
#    def load(self):
#        # need to specify the last item in the db
#	super(TimeSeriesData, self).load(time.time())
#
#    def addItem(self, value, time):
#        self.addItems([(value, time)])


if __name__ == '__main__':
    m_neonRedis    = NEON.Neon.NeonRedis(host='localhost', port=9999, db = 0, isWritable = True) # server mode, set msgs to Redis
    m_neonRedisCli = NEON.Neon.NeonRedis(host='localhost', port=9999, db = 0, isWritable = True) # client mode, get msgs from Redis
    m_neonRegister = NEON.Neon.NeonService.Register(m_neonRedis, 'NeonRegister')
    import random

    isTestNeonReg    = True 
    isTestHist       = True  
    isTestTimeSeries = True 
    isTestPOBoxCenter= True 
    isTestPOBoxGevent= True

    if(isTestNeonReg):
        interface = {'name':'haolai',
                'addr':'192.168.0.1',
                'port':9999}
        m_neonRegister.register(name = 'haolai', attribute = interface)
        pro = m_neonRegister.request(name = 'haolai')
        print pro
        print pro['name']
    

    if(isTestHist):
        xbins_raw = numpy.array([0,1,2,3,4,5])
        ybins_raw = numpy.array([0,1,2,3,4,5])
        xvals_raw = numpy.array([ 3.0,4.0,5.0,6.0,7.0])
        matrix_raw = numpy.matrix([[3,4,5,6,7], 
                                  [4,5,6,7,8],
                                  [5,6,7,8,9],
                                  [2,3,4,5,6],
                                  [1,2,3,4,5]])

        hist1d = NEON.Data.Hist1D(m_neonRedis, '/Bank1/2DHist/ID_100', xbins_raw, xvals_raw)
        hist1d.dump()
        print hist1d.getXAxis()
        print hist1d.getData()
        print hist1d.getStamp()

        time.sleep(2)
        hist2d = NEON.Data.Hist2D(m_neonRedis, '/Bank1/3DHist/ID_200', xbins_raw, ybins_raw, matrix_raw)
        hist2d.dump()

        hist = NEON.Data.Hist2D(m_neonRedis, '/Bank1/3DHist/ID_200')
        hist.load()
        print hist.getXAxis()
        print hist.getYAxis()
        print hist.getData()
        print hist.getStamp()

    if(isTestTimeSeries):
        ms_list = []
        for i in xrange(0,10):
            ms_list.append((random.randint(1,100), time.time()))
            time.sleep(0.1)

        ts01 = NEON.Data.TimeSeriesData(m_neonRedis, '/Bank1/TimeSeries/ProtonCharge', ms_list)
        ts01.dump()

        ts   = NEON.Data.TimeSeriesData(m_neonRedis, '/Bank1/TimeSeries/ProtonCharge')
        ts.load()
        print ts.getItems()

        ts01.addItem(random.randint(100, 1000), time.time())
        ts01.dump()

        ts.load()
        print ts.getItems()

    if(isTestPOBoxCenter):
        # zipcode = /POBox
        pob = NEON.Neon.NeonService.POBox(m_neonRedis, '/POBox', 'Bank1')    
        pob.send('Bank1', ['hello, POBox'])

        pob1 = NEON.Neon.NeonService.POBox(m_neonRedis, '/POBox')
        pob1.send('Bank1', ['Anonymous says hello'])

        print pob.recv()
        print pob.recv()

    groupName      = '/GPOBox'
    m_neonReg      = NEON.Neon.NeonService.Register(m_neonRedis, groupName, 'LOCALE')
    m_neonGresv    = NEON.Neon.NeonGevent(host='localhost', port=9998, db = m_neonReg, isWritable = False) 
    # server mode, accepts msgs from clients, host(listen host), port(listen port)
    m_neonGsent    = NEON.Neon.NeonGevent(db = m_neonReg, isWritable = True)  
    # client mode, sends msgs to other hosts, host and port do not use 
    # Destination address can be found though NeonRegister by its groupName, location and name
    # server and client mode of NeonGevent are both running in other thread, the run function starts
    # the new thread

    if(isTestPOBoxGevent):
        serv = NEON.Neon.NeonService.POBox(m_neonGresv, groupName, 'Geventresv')
        m_neonGresv.run()
        m_neonGsent.run()

        messages = []
        for i in xrange(1, 10):
            messages.append(str(random.randint(1,10))+' ')
        print '1 raw msgs: ', messages

        cli1  = NEON.Neon.NeonService.POBox(m_neonGsent, groupName, 'Geventsent1')
        cli1.send('Geventresv', messages)

        for i in xrange(1, 10):
            messages.append(str(random.randint(1,10))+' ')
        print '2 raw msgs: ', messages

        cli2  = NEON.Neon.NeonService.POBox(m_neonGsent, groupName, 'Geventsent2')
        cli2.send('Geventresv', messages)

        # need some time to finish this transmition process
        time.sleep(10)
        print 'POBox Gevent Msgs: '
        print serv.recv()

