#include "DataSvc/HeartBeatHdl.h"
#include "DataSvc/DroNEIncident.h"

#include <time.h>
#include <stdio.h> 
#include <sstream>
#include <iostream>
#include <Python.h>


	HeartBeatHdl::HeartBeatHdl(Task* par)
: m_par(par)
{

}

bool HeartBeatHdl::handle(Incident& incident)
{
	std::string ret;
	time_t t = time(NULL);
	DroNEIncident* pIncident = dynamic_cast<DroNEIncident*>(&incident);
	if (NULL==pIncident)return false;


        PyObject* dict = pIncident->getPyDict();
	if(NULL == dict){
		std::cout << "py obj = null" << std::endl;
	}
	else{
		if(not PyDict_Check(dict)){
			std::cout << "Error" << std::endl;
			return false;
		}

		PyObject* vstring = PyDict_GetItemString(dict, "string");
		std::string vstringstring(PyString_AsString(vstring));
		std::cout << "string: " << vstringstring << std::endl;

		PyObject* vint = PyDict_GetItemString(dict, "times");
		int vintint(PyInt_AsLong(vint));
		std::cout << "times: " << vintint << std::endl;

		for (int i = 0 ; i < vintint; i++){
			ret += vstringstring;
		}

	}



	std::stringstream retVal;
	retVal << t;
	ret += retVal.str();
	pIncident->setRetVal(ret);
	return true;
}
