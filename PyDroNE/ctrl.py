#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: tianhl

import DroNECore
import DroNECore.PyIncident as PI
import NEON
import json

def encodeHeartBeat(status, hit, event, pulse, idx):
    import json, time
    formattime = time.strftime("%Y-%m-%dT%H:%M:%S.000%Z", time.localtime())
    formatuuid = idx
    formatstat = status 
    formathitct= hit
    formatevent= event
    formatpulse= pulse
    heartbeatdata   = json.dumps({
         'timestamp': formattime,
         'uuid'     : formatuuid,
         'status'   : formatstat,
         'hit'      : formathitct,
         'event'    : formatevent,
         'pulse'    : formatpulse
         })
    return heartbeatdata

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

class HeartBeatCronIncident(PI.PyCronIncident):
    def __init__(self, name, cron = 0, repeatable = False, remotedata = None):
        super(HeartBeatCronIncident, self).__init__(name, cron, repeatable)
        self.remotedata = remotedata
        
    def execute(self):         
        retval = self.fire()
        if(retval):
            hitcnt, evtcnt, plscnt = retval.split(":")
            index = json.loads(self.remotedata.getData())['uuid']
            heartbeatdata = encodeHeartBeat(status='running', \
                                            hit = int(hitcnt), event = int(evtcnt), \
                                            pulse = int(plscnt), idx = index)
            self.remotedata.setData(heartbeatdata)
            self.remotedata.dump()
        return retval


if __name__ == "__main__":

    task = DroNECore.DroNE("task")
    task.asTop()
    task.setLogLevel(0)     
    
    import uuid    
    heartbeatdata = encodeHeartBeat(status='configuring', hit = 0, event = 0, pulse = 0, idx = str(uuid.uuid1()))
    m_neonRedis     = NEON.Neon.NeonRedis(host='localhost', port=9999, db = 0, isWritable = True) # server mode, set msgs to Redis
    m_taskheartbeat = NEON.Data.SingleValue(m_neonRedis, '/GPPD/hearbeat/detector/192.168.0.1:drone01', \
                                           description='192.168.0.1:01', data=heartbeatdata)
    m_taskheartbeat.dump()

    ct = DroNECore.CtrlTask("ctrl")
    #di = HelloIncident('HelloDroNE')
    hi = HeartBeatCronIncident("HeartBeat", cron = 2, repeatable = True, remotedata = m_taskheartbeat)
    #ct.add(di)
    ct.add(hi)
    #ct.add(hc)

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
    task.property("algs").append("RunningInfAlg")
    #alg = task.find("DumpAlg")

    #    
    task.setEvtMax(1000000)
    task.run()

    index = json.loads(m_taskheartbeat.getData())['uuid']
    hitct = json.loads(m_taskheartbeat.getData())['hit']
    evtct = json.loads(m_taskheartbeat.getData())['event']
    plsct = json.loads(m_taskheartbeat.getData())['pulse']
    m_taskheartbeat.setData(encodeHeartBeat(status='stopping', \
                                    hit = int(hitct), event = int(evtct), \
                                    pulse = int(plsct), idx = index))
    m_taskheartbeat.dump()

    m_neonRediscli  = NEON.Neon.NeonRedis(host='localhost', port=9999, db = 0, isWritable = False) # server mode, set msgs to Redis
    m_taskheartbeat2= NEON.Data.SingleValue(m_neonRediscli, '/GPPD/hearbeat/detector/192.168.0.1:drone01')
    m_taskheartbeat2.load()
    print m_taskheartbeat2.getData()

