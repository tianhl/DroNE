#ifndef DRONE_INCIDENT_HANDLER_H
#define DRONE_INCIDENT_HANDLER_H
// @(#)DroNE/DroNECore:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DroNEIncidentHandler.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include "SniperKernel/IIncidentHandler.h"
#include "SniperKernel/SniperException.h"
#include "Python.h"

class Task;
class DroNEIncident;

class DroNEIncidentHandler : public IIncidentHandler
{
	public :

		DroNEIncidentHandler(Task* par);

		bool handle(Incident& incident);

		DroNEIncident* getDroNEIncident(Incident& incident); 


		bool   getBool(const char* kname);
		int     getInt(const char* kname);
		long   getLong(const char* kname);
		float getFloat(const char* kname);
		char*  getChar(const char* kname);
	private:

		Task*        m_par;
		PyObject*    m_dict;
};

#endif
