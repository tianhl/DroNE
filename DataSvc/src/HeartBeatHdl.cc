#include "DataSvc/HeartBeatHdl.h"
#include "DataSvc/DroNEIncident.h"

#include <time.h>
#include <stdio.h> 
#include <sstream>
#include <iostream>


	HeartBeatHdl::HeartBeatHdl(Task* par)
: m_par(par)
{

}

bool HeartBeatHdl::handle(Incident& incident)
{
	time_t t = time(NULL);
	DroNEIncident* pIncident = dynamic_cast<DroNEIncident*>(&incident);
	if (NULL==pIncident)return false;
	std::stringstream retVal;
	retVal << t;
	std::string ret(retVal.str());
	pIncident->setRetVal(ret);
	return true;
}
