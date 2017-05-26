#include "DataSvc/DataSvc.h"
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
  this->find("/statistic")->branch("running_inf");
  this->regObj("/statistic/pixel_count", new PixelCount);
  this->regObj("/statistic/running_inf", new RunningInf);

  //Task* par = getScope();
  //std::cout << "incidents size: " << m_icdts.size() << std::endl;

  return true;
}

bool DataSvc::finalize()
{
	return true;
}