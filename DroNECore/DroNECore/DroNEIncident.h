#ifndef DRONE_INCIDENT_H
#define DRONE_INCIDENT_H

#include "SniperKernel/Incident.h"
#include <Python.h>

class DroNEIncident: public Incident
{
    public :


        DroNEIncident(const std::string& msg)
            : Incident(msg) {m_dict=NULL;}

        virtual ~DroNEIncident() {}


        void  setRetVal(std::string& retValue); 
        const std::string& getRetVal();
	void  clearRetVal();

	void  setPyDict(PyObject* obj);
	PyObject* getPyDict();
	void  clearPyDict();


    protected :

        std::string  m_ret;
	PyObject*    m_dict;
};

#endif
