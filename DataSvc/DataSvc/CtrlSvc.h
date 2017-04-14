#ifndef CTRL_SVC_H
#define CTRL_SVC_H

#include "SniperKernel/SvcBase.h"
#include "SniperKernel/IIncidentHandler.h"
#include "DataSvc/DynamicThreadedQueue.h"
#include <string>
#include <list>

class CtrlSvc : public SvcBase
{
    public :

      CtrlSvc(const std::string& name);
      virtual ~CtrlSvc();

      bool initialize();
      bool finalize();

    private :
      //static DynamicThreadedQueue<MonitorItem*> dataQueue;
      //std::list<IIncidentHandler*> m_icdts;

};


#endif
