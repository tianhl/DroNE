#ifndef SANSHE3TMAPALG_h
#define SANSHE3TMAPALG_h
// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  SANSHe3TMapAlg.h
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//

#include "SniperKernel/AlgBase.h"

#include "Data/Pulse.h"
#include "Data/Evt.h"

class DataSvc;

class SANSHe3TMapAlg: public AlgBase
{
    public:
        SANSHe3TMapAlg(const std::string& name);

        ~SANSHe3TMapAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
	DataSvc*      m_svc;
	EvtList*      m_evtcol;
	NeutronPulse* m_pulse;
    private:
	uint64_t getPixelID(uint32_t& module, uint32_t& x, uint32_t& y);

	//std::string m_configfile;
	//typedef std::map<int, int> MODULE2PID;
	//MODULE2PID m_m2p;


};

#endif
