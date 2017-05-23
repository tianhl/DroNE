#ifndef SNIPER_DATA_SVC_H
#define SNIPER_DATA_SVC_H

#include "SniperKernel/SvcBase.h"
#include "SniperKernel/IIncidentHandler.h"
#include "DroNECore/DynamicTreeNode.h"
#include "DroNECore/DataObject.h"
#include <map>
#include <list>

class DataSvc : public SvcBase, public DynamicTree<DataObject>
{
    public :

      DataSvc(const std::string& name);
      virtual ~DataSvc();

      bool initialize();
      bool finalize();

    private :
      std::list<IIncidentHandler*> m_icdts;

};


#endif
