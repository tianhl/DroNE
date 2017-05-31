#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: H.L. TIAN 2017

from libSniperPython import Task
#from libSniperPython import Incident
#import PyIncident
import NEON
import DroNECore.PyIncident as PI

class RPCMethods(NEON.Neon.NeonService.NeonRPC.MethodCall):
    def __init__(self):
        pass

    def Stop(self, params=None):
        print "NEON RPC RECV & EVEC STOPRUN COMMAND"
        ic = PI.PyIncident("StopRun")
        ic.fire('StopRun')
        return "Ready"

class NeonRPCTask(Task) :

    def __init__(self, name, rpcserver) :
        Task.__init__(self, name)
        Task.regist(self, "NeonRPC")
        self.rpcserver = rpcserver
        self.rpcmethod = RPCMethods()

    def handle(self, incident) :
        rpclist = self.rpcserver.recvRPC()
        for rpcitem in rpclist:
            neonid, method, parameters = rpcitem
            print "uuid: ", neonid, " method: ", method, " parameters: ", parameters
            returnValue, errorCode = self.rpcmethod.execute(method, parameters)
            self.rpcserver.sndRSLT(neonid, returnValue = "return OK", error = "None ERROR")
