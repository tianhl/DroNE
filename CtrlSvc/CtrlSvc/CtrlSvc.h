#ifndef CTRL_SVC_H
#define CTRL_SVC_H
// @(#)DroNE/CtrlSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  CtrlSvc.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

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
