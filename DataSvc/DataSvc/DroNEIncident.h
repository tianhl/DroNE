#ifndef DRONE_INCIDENT_H
#define DRONE_INCIDENT_H

#include "SniperKernel/Incident.h"

class DroNEIncident: public Incident
{
    public :


        DroNEIncident(const std::string& msg)
            : Incident(msg) {}

        virtual ~DroNEIncident() {}


        void  setRetVal(std::string& retValue); 
        const std::string& getRetVal();
	//virtual bool  fire();


    protected :

        std::string  m_ret;
};

#endif
