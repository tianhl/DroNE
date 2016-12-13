#include "DataSvc/DataSvc.h"
#include "DataSvc/BeginEvtHdl.h"
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

	declProp("SimMode", m_mc);
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

  if(not m_mc){
	  Task* par = getScope();
	  IIncidentHandler* bi = new BeginEvtHdl(par);
	  if ( par->isTop() ) bi->regist("BeginEvent");
	  else bi->regist(par->scope() + par->objName() + ":BeginEvent");

	  m_icdts.push_back(bi);
  }
  std::cout << "simluation mode: " << m_mc << std::endl;

  return true;
}

bool DataSvc::finalize()
{
	return true;
}
