from libSniperPython import Task
from libSniperPython import Incident
import PyIncident

class CtrlTask(Task) :

    def __init__(self, name) :
        Task.__init__(self, name)
        Task.regist(self, "CtrlTask")

        self.__heartbeat = PyIncident.PyIncident("task:HeartBeat")
        print self.__heartbeat.name()

    def handle(self, incident) :
        print self.__heartbeat.fire({"string":" pyIncident hello ", "times":2})

