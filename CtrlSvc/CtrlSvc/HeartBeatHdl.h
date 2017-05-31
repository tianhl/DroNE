#ifndef HEART_BEAT_HANDLER_H
#define HEART_BEAT_HANDLER_H
// @(#)DroNE/CtrlSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  HeartBeatHdl.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

#include "DroNECore/DroNEIncidentHandler.h"

class Task;
class DataSvc;

class HeartBeatHdl : public DroNEIncidentHandler
{
    public :

        HeartBeatHdl(Task* par);

        bool handle(Incident& incident);

    private :

	DataSvc* m_svc;
	Task*          m_par;
};

#endif
