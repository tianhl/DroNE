#ifndef END_EVENT_HANDLER_H
#define END_EVENT_HANDLER_H
// @(#)DroNE/CtrlSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  EndEvtHdl.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

#include "SniperKernel/IIncidentHandler.h"

class Task;

class EndEvtHdl : public IIncidentHandler
{
    public :

        EndEvtHdl(Task* par);

        bool handle(Incident& incident);

    private :

        Task*          m_par;
};

#endif
