#ifndef HEART_BEAT_HANDLER_H
#define HEART_BEAT_HANDLER_H

#include "DataSvc/DroNEIncidentHandler.h"

class Task;

class HeartBeatHdl : public DroNEIncidentHandler
{
    public :

        HeartBeatHdl(Task* par);

        bool handle(Incident& incident);

//    private :

  //      Task*          m_par;
};

#endif
