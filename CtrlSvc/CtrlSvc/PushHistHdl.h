#ifndef PUSH_HIST_HANDLER_H
#define PUSH_HIST_HANDLER_H

#include "DroNECore/DroNEIncidentHandler.h"

class Task;
class DataSvc;

class PushHistHdl : public DroNEIncidentHandler
{
    public :

        PushHistHdl(Task* par);

        bool handle(Incident& incident);

    private :

	DataSvc*       m_svc;
	Task*          m_par;
};

#endif
