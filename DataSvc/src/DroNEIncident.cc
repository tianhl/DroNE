#include "DataSvc/DroNEIncident.h"


void DroNEIncident::setRetVal(std::string& retValue) { 
	m_ret = retValue; 
};

const std::string& DroNEIncident::getRetVal() { 
	return m_ret; 
};
bool DroNEIncident::fire(){
	return Incident::fire();
}
