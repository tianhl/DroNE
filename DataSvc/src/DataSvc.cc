// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DataSvc.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include "DataSvc/DataSvc.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"
#include <iostream>
#include <list>

#include "Data/GPPDStatistic.h"
#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/SNDHit.h"

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

//============================
// Pulse/Hit/Evt
//============================
  this->branch("pulse");
  this->find("/pulse")->branch("hits");
  this->find("/pulse")->branch("evts");

  this->regObj("/pulse",      new NeutronPulse);
  this->regObj("/pulse/hits", new SNDHitList);
  this->regObj("/pulse/evts", new EvtList);

//============================
// Simulation
//============================
  this->branch("simulation");
  this->find("/simulation")->branch("evts");

  this->regObj("/simulation/evts", new EvtList);

//============================
// Statistic
//============================
  this->branch("statistic");
  this->find("/statistic")->branch("running_inf");
  this->find("/statistic")->branch("pixel_count");
  this->find("/statistic")->branch("pixel_counts");

  this->regObj("/statistic/running_inf",  new RunningInf);
  this->regObj("/statistic/pixel_count",  new PixelCount);
  this->regObj("/statistic/pixel_counts", new PixelCountList);

  //Task* par = getScope();
  //std::cout << "incidents size: " << m_icdts.size() << std::endl;

  return true;
}

bool DataSvc::finalize()
{
	return true;
}
