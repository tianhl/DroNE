#include "DataSvc/DroNEIncident.h"
#include <iostream>

void DroNEIncident::setRetVal(std::string& retValue) { 
	m_ret = retValue; 
};

const std::string& DroNEIncident::getRetVal() { 
	return m_ret; 
};

void DroNEIncident::setPyDict(PyObject* obj){
	clearPyDict();
	m_dict = obj;
}

PyObject* DroNEIncident::getPyDict(){
	return m_dict;
}

void DroNEIncident::clearPyDict(){
	if(m_dict) m_dict = NULL;
}
