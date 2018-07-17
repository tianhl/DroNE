// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2018
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2018 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  MRDataSvcV02.cc
//  
//
//  Created by Haolai TIAN 4th May 2018.
//
//
#include "DataSvc/MRDataSvcV02.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"
#include <iostream>
#include <list>

#include "Data/GPPDStatistic.h"
#include "Data/Pulse.h"
#include "Data/EvtD.h"
#include "Data/MWPCHit.h"

DECLARE_SERVICE(MRDataSvcV02);

MRDataSvcV02::MRDataSvcV02(const std::string& name)
    : DataSvc(name)
{
}

MRDataSvcV02::~MRDataSvcV02()
{
}

bool MRDataSvcV02::initialize()
{

//============================
// Pulse/Hit/Evt
//============================
  this->branch("pulse");
  this->find("/pulse")->branch("ecal");
  this->find("/pulse")->branch("hits");
  this->find("/pulse")->branch("evts");

  this->regObj("/pulse",      new NeutronPulse);
  this->regObj("/pulse/hits", new MWPCHitList);
  this->regObj("/pulse/evts", new EvtDList);

//============================
// Simulation
//============================
  this->branch("simulation");
  this->find("/simulation")->branch("evts");

  this->regObj("/simulation/evts", new EvtDList);

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
  return true;
}

bool MRDataSvcV02::finalize()
{
	return true;
}
