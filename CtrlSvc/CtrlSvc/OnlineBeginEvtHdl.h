#ifndef ONLINE_BEGIN_EVENT_HANDLER_H
#define ONLINE_BEGIN_EVENT_HANDLER_H
// @(#)DroNE/CtrlSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  OnlineBeginEvtHdl.h
//  
//
//  Created by Haolai TIAN 10th Dec. 2017.
//
//

#include "SniperKernel/IIncidentHandler.h"

class Task;
class DataInputSvc;

class OnlineBeginEvtHdl : public IIncidentHandler
{
    public :

        OnlineBeginEvtHdl(Task* par);

        bool handle(Incident& incident);

    private :

        Task*          m_par;
        DataInputSvc*  m_iSvc;
};

#endif
