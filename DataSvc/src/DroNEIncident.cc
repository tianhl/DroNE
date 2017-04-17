#include "DataSvc/DroNEIncident.h"

void DroNEIncident::setRetVal(std::string& retValue) { 
	m_ret = retValue; 
};

const std::string& DroNEIncident::getRetVal() { 
	return m_ret; 
};

void DroNEIncident::setPyDict(PyObject* obj){
	if(m_dict)delete m_dict;
	m_dict = obj;
}

PyObject* DroNEIncident::getPyDict(){
	return m_dict;
}
