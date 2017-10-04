// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  SANSDataSvc.cc
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//
#include "DataSvc/SANSDataSvc.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"
#include <iostream>
#include <list>

//#include "Data/GPPDStatistic.h"
#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/He3THit.h"

DECLARE_SERVICE(SANSDataSvc);

SANSDataSvc::SANSDataSvc(const std::string& name)
    : DataSvc(name)
{
}

SANSDataSvc::~SANSDataSvc()
{
}

bool SANSDataSvc::initialize()
{

//============================
// Pulse/Hit/Evt
//============================
  this->branch("pulse");
  this->find("/pulse")->branch("hits");
  this->find("/pulse")->branch("evts");

  this->regObj("/pulse",      new NeutronPulse);
  this->regObj("/pulse/hits", new He3THitList);
  this->regObj("/pulse/evts", new EvtList);

//============================
// Simulation
//============================
  this->branch("simulation");
  this->find("/simulation")->branch("evts");

  this->regObj("/simulation/evts", new EvtList);

  return true;
}

bool SANSDataSvc::finalize()
{
	return true;
}
