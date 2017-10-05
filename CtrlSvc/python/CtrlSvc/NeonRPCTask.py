#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: H.L. TIAN 2017

from libSniperPython import Task
#from libSniperPython import Incident
#import PyIncident
import NEON
import DroNECore.PyIncident as PI
import json
import CtrlSvc.HeartBeatIncident  as HBI

class RPCMethods(NEON.Neon.NeonService.NeonRPC.MethodCall):
    def __init__(self):
        pass

    def Stop(self, params=None):
        print "NEON RPC RECV & EVEC STOPRUN COMMAND"
        ic = PI.PyIncident("StopRun")
        ic.fire('StopRun')
        return "Ready"

    def Clear(self, params=None):
        print "NEON RPC RECV & EVEC CLEAR COMMAND"
        ic = PI.PyIncident("ClearDataSvc")
        ic.fire('ClearDataSvc')
        return "Ready"

class NeonRPCTask(Task) :

    def __init__(self, name, rpcserver, remotedata = None) :
        Task.__init__(self, name)
        Task.regist(self, "NeonRPC")
        self.rpcserver = rpcserver
        self.rpcmethod = RPCMethods()
        self.remotedata = remotedata

    def handle(self, incident) :
        rpclist = self.rpcserver.recvRPC()
        for rpcitem in rpclist:
            neonid, method, parameters = rpcitem
            print "uuid: ", neonid, " method: ", method, " parameters: ", parameters
            # NOW it is only for Stop
            if(method=="Stop"):
                index = json.loads(self.remotedata.getData())['uuid']
                heartbeatdata = HBI.HeartBeatCronIncident.encodeHeartBeat(status='stopping', \
                                                hit = 0, event = 0, \
                                                pulse = 0, idx = index)
                print heartbeatdata, "============="
                self.remotedata.setData(heartbeatdata)
                self.remotedata.dump()
                self.rpcserver.sndRSLT(neonid, result = "Stop OK", error = "None ERROR")
                returnValue, errorCode = self.rpcmethod.execute(method, parameters)
             else:
                returnValue, errorCode = self.rpcmethod.execute(method, parameters)
                self.rpcserver.sndRSLT(neonid, result = returnValue, error = errorCode)

