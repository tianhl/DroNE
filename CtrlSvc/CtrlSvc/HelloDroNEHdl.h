#ifndef HELLO_DRONE_HANDLER_H
#define HELLO_DRONE_HANDLER_H

#include "DroNECore/DroNEIncidentHandler.h"

class Task;
class DataSvc;

class HelloDroNEHdl : public DroNEIncidentHandler
{
	public :

		HelloDroNEHdl(Task* par);

		bool handle(Incident& incident);

	private :

		DataSvc*       m_svc;
		Task*          m_par;
};

#endif
