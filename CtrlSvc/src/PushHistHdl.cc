#include "CtrlSvc/PushHistHdl.h"
#include "DataSvc/DataSvc.h"
#include "DroNECore/DroNEIncident.h"
#include "Data/GPPDStatistic.h"

#include "SniperKernel/Task.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SniperLog.h"

#include <stdio.h> 
#include <sstream>
#include <iostream>
#include <Python.h>

PushHistHdl::PushHistHdl(Task* par)
: DroNEIncidentHandler(par)
{
	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) throw SniperException("DataInputSvc is invalid!");
	m_svc = pSvc.data();
}


bool PushHistHdl::handle(Incident& incident)
{

	PixelCount* pc  = m_svc->getObj<PixelCount>("/statistic/pixel_count");

	DroNEIncident* pIncident = getDroNEIncident(incident);
	if(pIncident == NULL) return false;

	std::stringstream retVal;
	//KEYS& pids = pc->getPixelIDs();
	for(int i = 0; i < 111*48; i++){
		retVal << pc->getCount(i+1100000) << ":";
	}

	std::string ret;
	ret = retVal.str();
	pIncident->setRetVal(ret);
	return true;
}
