#ifndef BEGIN_EVENT_HANDLER_H
#define BEGIN_EVENT_HANDLER_H
// @(#)DroNE/CtrlSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  BeginEvtHdl.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

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
