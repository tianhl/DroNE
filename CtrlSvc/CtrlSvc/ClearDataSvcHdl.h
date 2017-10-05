#ifndef CLEARDATASVC_HANDLER_H
#define CLEARDATASVC_HANDLER_H
// @(#)DroNE/CtrlSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  ClearDataSvcHdl.h
//  
//
//  Created by Haolai TIAN 4th Oct. 2017.
//
//

#include "DroNECore/DroNEIncidentHandler.h"

class Task;
class DataSvc;

class ClearDataSvcHdl : public DroNEIncidentHandler
{
    public :

        ClearDataSvcHdl(Task* par);

        bool handle(Incident& incident);

    private :

	DataSvc*       m_svc;
	Task*          m_par;
};

#endif
