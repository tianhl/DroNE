#ifndef SNIPER_TASK_H
#define SNIPER_TASK_H

#include "SniperKernel/DLElement.h"
#include "SniperKernel/IIncidentHandler.h"
#include <list>
#include <map>

//Pre-declarations
class AlgBase;

class AlgMgr
{
    public :

        //Constructor
        AlgMgr(const std::string& name);

        //Destructor
        virtual ~AlgMgr();



        AlgBase* findAlg(const std::string& name);
        AlgBase* createAlg(const std::string& algName);
	void     deleteAlg(const std::string& algName);
        void     clearAlgs();




    private :
        bool insert(DLElement* obj, bool owned);
        bool remove(const std::string& name);

        typedef std::map<std::string, AlgBase* >  AlgMap;
        AlgMap  m_algs;

};

#endif
