#ifndef CTRL_SVC_H
#define CTRL_SVC_H

#include "SniperKernel/SvcBase.h"
#include "SniperKernel/IIncidentHandler.h"
#include <map>
#include <list>

class CtrlSvc : public SvcBase
{
    public :

      CtrlSvc(const std::string& name);
      virtual ~CtrlSvc();

      bool initialize();
      bool finalize();

    private :
      std::list<IIncidentHandler*> m_icdts;

};


#endif
