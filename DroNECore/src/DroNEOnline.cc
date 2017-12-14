// @(#)DroNEOnline/DroNEOnlineCore:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DroNEOnline.cc
//  
//
//  Created by Haolai TIAN 10th Dec. 2017.
//
//

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/SvcBase.h"
#include "SniperKernel/DataMemSvc.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/DeclareDLE.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SniperLog.h"
#include <algorithm>

#include "DroNECore/DroNEOnline.h"

SNIPER_DECLARE_DLE(DroNEOnline);


DroNEOnline::DroNEOnline(const std::string& name)
    : Task(name)
{
	declProp("is_online", m_onlinemode);


}

DroNEOnline::~DroNEOnline() {
}

bool DroNEOnline::execute(){

	int run = switchStatus(Running);
	if ( run != 0 ) return (run > 0);

	++m_loop;
	LogTest << " drone executing " << m_loop << std::endl;

	try {
STARTNEXT:
		//trigger the BeginEvent incident
		if(local_fire("OnlineBeginEvent")){

			//executing algorithms
			for ( std::list<AlgBase*>::iterator iAlg = m_algs.begin();
					iAlg != m_algs.end();
					++iAlg ) {
				if ( ! (*iAlg)->execute() ) {
					LogError << "execute failed!" << std::endl;
					//FIXME: this is not an elegant way...
					Incident::fire("StopRun");
				}
			}
			//trigger the EndEvent incident
			local_fire("CtrlTask");
		}
		else{
			//trigger the EndEvent incident
			local_fire("CtrlTask");
                        usleep(10000); // suspend execution for 0.01 second
                        LogTest << "waiting for input data" << std::endl;
                        goto STARTNEXT;
		}

	}
	catch (SniperStopped& e) {
		if ( ! m_isTop ) throw e;
		LogInfo << "Stopping execution..." << std::endl;
	}

	return true;

}

