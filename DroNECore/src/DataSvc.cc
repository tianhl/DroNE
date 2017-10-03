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
//  Created by Haolai TIAN 1st Oct. 2017.
//
//
#include "DroNECore/DataSvc.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"
#include <iostream>
#include <list>

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

  return true;
}

bool DataSvc::finalize()
{
	return true;
}
