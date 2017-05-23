#include "CtrlSvc/CtrlSvc.h"
#include "CtrlSvc/HeartBeatHdl.h"
#include "CtrlSvc/BeginEvtHdl.h"
#include "CtrlSvc/EndEvtHdl.h"
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

	IIncidentHandler* bi = new BeginEvtHdl(par);
	//if ( par->isTop() ) bi->regist("BeginEvent");
	//else bi->regist(par->scope() + par->objName() + ":BeginEvent");
	bi->regist("BeginEvent");
	bi->listening();

	IIncidentHandler* ei = new EndEvtHdl(par);
	ei->regist("EndEvent");
	//if ( par->isTop() ) ei->regist("EndEvent");
	//else ei->regist(par->scope() + par->objName() + ":EndEvent");
	ei->listening();

	m_icdts.push_back(hbi);
	m_icdts.push_back(bi);
	m_icdts.push_back(ei);

	std::cout << "ctrlsvc incidents size: " << m_icdts.size() << std::endl;

	return true;
}

bool CtrlSvc::finalize()
{
	return true;
}
