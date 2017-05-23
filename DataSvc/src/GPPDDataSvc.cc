#include "DataSvc/GPPDDataSvc.h"
#include "DroNECore/BeginEvtHdl.h"
#include "DroNECore/EndEvtHdl.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"
#include <iostream>
#include <list>

#include "Data/GPPDStatistic.h"
#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/Hit.h"

DECLARE_SERVICE(DataSvc);

DataSvc::DataSvc(const std::string& name)
    : SvcBase(name)
{
}

DataSvc::~DataSvc()
{
}

bool DataSvc::initialize()
{

  this->branch("pulse");
  this->find("/pulse")->branch("hits");
  this->find("/pulse")->branch("evts");

  this->regObj("/pulse", new NeutronPulse);
  this->regObj("/pulse/hits", new HitList);
  this->regObj("/pulse/evts", new EvtList);

  this->branch("statistic");

  this->find("/statistic")->branch("pixel_count");
  this->regObj("/statistic/pixel_count", new PixelCount);

  Task* par = getScope();
  IIncidentHandler* bi = new BeginEvtHdl(par);
  if ( par->isTop() ) bi->regist("BeginEvent");
  else bi->regist(par->scope() + par->objName() + ":BeginEvent");

  IIncidentHandler* ei = new EndEvtHdl(par);
  if ( par->isTop() ) ei->regist("EndEvent");
  else ei->regist(par->scope() + par->objName() + ":EndEvent");

  m_icdts.push_back(bi);
  m_icdts.push_back(ei);
  std::cout << "incidents size: " << m_icdts.size() << std::endl;

  return true;
}

bool DataSvc::finalize()
{
	return true;
}
