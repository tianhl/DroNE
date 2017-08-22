#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: H.L. TIAN 2017

from libSniperPython import Task
from libSniperPython import Incident
import PyIncident

class CtrlTask(Task) :

    def __init__(self, name) :
        Task.__init__(self, name)
        Task.regist(self, "CtrlTask")

        self.__incidentList = []

    def add(self, icd = None):
        if(isinstance(icd, PyIncident.PyIncident)):
            self.__incidentList.append(icd)
            print "This instance ", icd.name(), " is added successfully!"
        else:
            print "Error Type, this instance is NOT PyIncident"

    def handle(self, incident) :
	for eachitem in self.__incidentList:
		ret = eachitem.execute()
                #if(ret):
                    #print ret
        #print self.__heartbeat.fire({"string":" pyIncident hello ", "times":2})

