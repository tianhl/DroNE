#ifndef REFLMWPCFASTSIMALG_h
#define REFLMWPCFASTSIMALG_h
// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  REFLMWPCFastSimAlg.h
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//

#include "SniperKernel/AlgBase.h"

#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/MWPCHit.h"

class DataSvc;

class REFLMWPCFastSimAlg: public AlgBase
{
    public:
        REFLMWPCFastSimAlg(const std::string& name);

        ~REFLMWPCFastSimAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
	int           m_count;
	MWPCHitList*  m_hitcol;
	EvtList*      m_evtcol;
	NeutronPulse* m_pulse;
	DataSvc*      m_svc;

    private: 
	void getXYChannel(uint32_t pid, uint32_t &xchannel, uint32_t &ychannel);
	void simulation(uint32_t xchannel, uint32_t ychannel, uint32_t tdata);
        void calculation(uint32_t position, uint32_t timedata, uint32_t qdata, bool setoffset);
};

#endif
