#ifndef PUSH_HIST_HANDLER_H
#define PUSH_HIST_HANDLER_H
// @(#)DroNE/CtrlSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  PushHistHdl.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

#include "DroNECore/DroNEIncidentHandler.h"

class Task;
class DataSvc;

class PushHistHdl : public DroNEIncidentHandler
{
    public :

        PushHistHdl(Task* par);

        bool handle(Incident& incident);

    private :

	DataSvc*       m_svc;
	Task*          m_par;
};

#endif
