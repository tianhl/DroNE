#ifndef HEART_BEAT_HANDLER_H
#define HEART_BEAT_HANDLER_H

#include "SniperKernel/IIncidentHandler.h"

class Task;

class HeartBeatHdl : public IIncidentHandler
{
    public :

        HeartBeatHdl(Task* par);

        bool handle(Incident& incident);

    private :

        Task*          m_par;
};

#endif
