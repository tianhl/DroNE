// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  REFLDataSvc.cc
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//
#include "DataSvc/REFLDataSvc.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"
#include <iostream>
#include <list>

//#include "Data/GPPDStatistic.h"
#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/MWPCHit.h"

DECLARE_SERVICE(REFLDataSvc);

REFLDataSvc::REFLDataSvc(const std::string& name)
    : DataSvc(name)
{
}

REFLDataSvc::~REFLDataSvc()
{
}

bool REFLDataSvc::initialize()
{

//============================
// Pulse/Hit/Evt
//============================
  this->branch("pulse");
  this->find("/pulse")->branch("hits");
  this->find("/pulse")->branch("evts");

  this->regObj("/pulse",      new NeutronPulse);
  this->regObj("/pulse/hits", new MWPCHitList);
  this->regObj("/pulse/evts", new EvtList);

//============================
// Simulation
//============================
  this->branch("simulation");
  this->find("/simulation")->branch("evts");

  this->regObj("/simulation/evts", new EvtList);

  return true;
}

bool REFLDataSvc::finalize()
{
	return true;
}
