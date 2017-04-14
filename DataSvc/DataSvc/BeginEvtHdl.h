#ifndef BEGIN_EVENT_HANDLER_H
#define BEGIN_EVENT_HANDLER_H

#include "SniperKernel/IIncidentHandler.h"

class Task;
class DataSvc;
class DataInputSvc;

class BeginEvtHdl : public IIncidentHandler
{
    public :

        BeginEvtHdl(Task* par);

        bool handle(Incident& incident);

    private :

        Task*          m_par;
        DataSvc*       m_dataSvc;
        DataInputSvc*  m_iSvc;
};

#endif
