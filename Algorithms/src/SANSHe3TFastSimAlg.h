#ifndef SANSHE3TFASTSIMALG_h
#define SANSHE3TFASTSIMALG_h
// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  SANSHe3TFastSimAlg.h
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//

#include "SniperKernel/AlgBase.h"

#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/He3THit.h"

class DataSvc;

class SANSHe3TFastSimAlg: public AlgBase
{
    public:
        SANSHe3TFastSimAlg(const std::string& name);

        ~SANSHe3TFastSimAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
	int           m_count;
	He3THitList*  m_hitcol;
	EvtList*      m_evtcol;
	NeutronPulse* m_pulse;
	DataSvc*      m_svc;

    private: 
	void getXYChannel(uint32_t pid, uint32_t &xchannel, uint32_t &ychannel);
	void simulation(uint32_t xchannel, uint32_t ychannel, uint32_t tdata);
        void calculation(uint32_t tub, uint32_t tof, uint32_t qu, uint32_t ql);
};

#endif
