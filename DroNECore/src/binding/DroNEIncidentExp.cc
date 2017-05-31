// @(#)DroNE/DroNECore:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  binding/DroNEIncidentExp.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include "DroNECore/DroNEIncident.h"
#include <boost/python.hpp>


void export_DroNE_Incident()
{
	using namespace boost::python;

	//bool (DroNEIncident::*member_fire)() = &DroNEIncident::fire;

	class_<DroNEIncident, bases<Incident>, boost::noncopyable>
		("DroNEIncident", init<const std::string&>())
		.def("getRetVal", &DroNEIncident::getRetVal,
				return_value_policy<copy_const_reference>())
		.def("clearRetVal", &DroNEIncident::clearRetVal)
		.def("setPyDict", &DroNEIncident::setPyDict,
				return_value_policy<copy_const_reference>())
		.def("clearPyDict", &DroNEIncident::clearPyDict)
		;
}
