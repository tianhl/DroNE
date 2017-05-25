#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: tianhl

#import DroNECore
import DroNECore.PyIncident as PI
import NEON
import json


class HeartBeatCronIncident(PI.PyCronIncident):
    def __init__(self, name, cron = 0, repeatable = False, remotedata = None):
        super(HeartBeatCronIncident, self).__init__(name, cron, repeatable)
        self.remotedata = remotedata
        
    def execute(self):         
        retval = self.fire()
        if(retval):
            hitcnt, evtcnt, plscnt = retval.split(":")
            index = json.loads(self.remotedata.getData())['uuid']
            heartbeatdata = HeartBeatCronIncident.encodeHeartBeat(status='running', \
                                            hit = int(hitcnt), event = int(evtcnt), \
                                            pulse = int(plscnt), idx = index)
            self.remotedata.setData(heartbeatdata)
            self.remotedata.dump()
        return retval

    @staticmethod
    def encodeHeartBeat( status, hit, event, pulse, idx):
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
