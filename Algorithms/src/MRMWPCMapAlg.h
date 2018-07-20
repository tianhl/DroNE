#ifndef MRMWPCMAPALG_h
#define MRMWPCMAPALG_h
// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  MRMWPCMapAlg.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

#include "SniperKernel/AlgBase.h"

#include "Data/Pulse.h"
#include "Data/EvtD.h"

class DataSvc;

class MRMWPCMapAlg: public AlgBase
{
    public:
        MRMWPCMapAlg(const std::string& name);

        ~MRMWPCMapAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
	DataSvc*      m_svc;
	EvtDList*      m_evtcol;
	NeutronPulse* m_pulse;
    private:
	//uint64_t getPixelID(uint32_t& module, uint32_t& x, uint32_t& y);
	uint64_t getPixelID(uint32_t& module, float& x, float& y);

	//std::string m_configfile;
	//typedef std::map<int, int> MODULE2PID;
	//MODULE2PID m_m2p;


};

#endif
