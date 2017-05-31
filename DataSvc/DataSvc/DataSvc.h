#ifndef DRONE_DATA_SVC_H
#define DRONE_DATA_SVC_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DataSvc.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

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

    //private :
    //  std::list<IIncidentHandler*> m_icdts;

};


#endif
