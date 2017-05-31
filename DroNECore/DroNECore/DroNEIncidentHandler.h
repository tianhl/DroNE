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
#include <vector>
#include <string>

#include "SniperKernel/SvcBase.h"


class DataInputSvc : public SvcBase
{
    public :

      DataInputSvc(const std::string& name);
      virtual ~DataInputSvc();

      virtual bool initialize();
      virtual bool finalize();
      virtual bool next();

};


#endif

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
