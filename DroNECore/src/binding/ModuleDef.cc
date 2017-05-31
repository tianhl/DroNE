// @(#)DroNE/DroNECore:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  binding/ModuleDef.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include <boost/python.hpp>

extern void export_DroNE();
extern void export_DroNE_Incident();


BOOST_PYTHON_MODULE(libDroNECorePython)
{
	export_DroNE();
	export_DroNE_Incident();
}
