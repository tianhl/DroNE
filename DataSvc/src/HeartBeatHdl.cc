#include "DataSvc/HeartBeatHdl.h"
#include "DataSvc/DroNEIncident.h"

#include <time.h>
#include <stdio.h> 
#include <sstream>
#include <iostream>
#include <Python.h>

HeartBeatHdl::HeartBeatHdl(Task* par)
: DroNEIncidentHandler(par)
{

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
