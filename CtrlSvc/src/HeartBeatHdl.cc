#include "CtrlSvc/HeartBeatHdl.h"
#include "DataSvc/DataSvc.h"
#include "DroNECore/DroNEIncident.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperException.h"

#include <time.h>
#include <stdio.h> 
#include <sstream>
#include <iostream>
#include <Python.h>

HeartBeatHdl::HeartBeatHdl(Task* par)
: DroNEIncidentHandler(par)
{
	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) {
		throw SniperException("DataInputSvc is invalid!");
	}
	m_svc = pSvc.data();
}


bool HeartBeatHdl::handle(Incident& incident)
{
        DroNEIncident* pIncident = getDroNEIncident(incident);
        if(pIncident == NULL) return false;

	std::string ret;
	time_t t = time(NULL);

	std::string vstringstring(getChar("string"));
	std::cout << "string: " << vstringstring << std::endl;

	int vintint(getInt("times"));
	std::cout << "times: " << vintint << std::endl;

	for (int i = 0 ; i < vintint; i++){
		ret += vstringstring;
	}




	std::stringstream retVal;
	retVal << t;
	ret += retVal.str();
	pIncident->setRetVal(ret);
	return true;
}
