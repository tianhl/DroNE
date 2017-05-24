#ifndef HEART_BEAT_HANDLER_H
#define HEART_BEAT_HANDLER_H

#include "DroNECore/DroNEIncidentHandler.h"

class Task;
class DataSvc;

class HeartBeatHdl : public DroNEIncidentHandler
{
    public :

        HeartBeatHdl(Task* par);

        bool handle(Incident& incident);

    private :

	DataSvc* m_svc;
	Task*          m_par;
};

#endif
