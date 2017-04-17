from libSniperPython import Task
from libSniperPython import Incident

class CtrlTask(Task) :

    def __init__(self, name) :
        print "hello Ctrl Task"
        Task.__init__(self, name)
        Task.regist(self, "CtrlTaskSayHi")

    def run(self) :
        print "Hello World!"

    def handle(self, incident) :
        if ( incident.name() == 'CtrlTaskSayHi' ) :
            print "CtrlTask say: How are you doing ?"
        if ( incident.name() == 'hi' ) :
            print " How are you doing ?"
        return Task.handle(self, incident)
