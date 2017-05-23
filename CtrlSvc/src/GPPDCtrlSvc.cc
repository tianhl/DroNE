#include "CtrlSvc/GPPDCtrlSvc.h"
#include "CtrlSvc/HeartBeatHdl.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"
#include <iostream>
#include <list>

DECLARE_SERVICE(CtrlSvc);

CtrlSvc::CtrlSvc(const std::string& name)
    : SvcBase(name)
{
}

CtrlSvc::~CtrlSvc()
{
}

bool CtrlSvc::initialize()
{

	Task* par = getScope();

	IIncidentHandler* hbi = new HeartBeatHdl(par);
	if ( par->isTop() ) hbi->regist("HeartBeat");
	else hbi->regist(par->scope() + par->objName() + ":HeartBeat");


	hbi->listening();

	m_icdts.push_back(hbi);
	std::cout << "ctrlsvc incidents size: " << m_icdts.size() << std::endl;

	return true;
}

bool CtrlSvc::finalize()
{
	return true;
}
