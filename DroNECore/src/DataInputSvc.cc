// @(#)DroNE/DroNECore:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DataInputSvc.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

#include "DroNECore/DataInputSvc.h"

#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"


DECLARE_SERVICE(DataInputSvc);

DataInputSvc::DataInputSvc(const std::string& name)
: SvcBase(name) {
}

DataInputSvc::~DataInputSvc() {
}

bool DataInputSvc::initialize() {
	return true;
}

bool DataInputSvc::finalize() {
	return true;
}

bool DataInputSvc::next() {
	return true;
}
