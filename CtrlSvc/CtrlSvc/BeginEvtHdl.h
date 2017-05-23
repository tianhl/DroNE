#ifndef BEGIN_EVENT_HANDLER_H
#define BEGIN_EVENT_HANDLER_H

#include "SniperKernel/IIncidentHandler.h"

class Task;
class DataInputSvc;

class BeginEvtHdl : public IIncidentHandler
{
    public :

        BeginEvtHdl(Task* par);

        bool handle(Incident& incident);

    private :

        Task*          m_par;
        DataInputSvc*  m_iSvc;
};

#endif
