from libSniperPython import Task
from libSniperPython import Incident
import DataSvc

class CtrlTask(Task) :

    def __init__(self, name) :
        Task.__init__(self, name)
        Task.regist(self, "CtrlTask")

        self.__heartbeat = DataSvc.DroNEIncident("task:HeartBeat")
        print self.__heartbeat.name()

    def handle(self, incident) :
        self.__heartbeat.fire()
        print "python without dict: ", self.__heartbeat.getRetVal()

