#ifndef DRONE_INCIDENT_HANDLER_H
#define DRONE_INCIDENT_HANDLER_H

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
