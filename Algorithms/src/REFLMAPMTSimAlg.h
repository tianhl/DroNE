#ifndef REFLMAPMTALG_h
#define REFLMAPMTALG_h
// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  REFLMAPMTSimAlg.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

#include "SniperKernel/AlgBase.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "Data/Pulse.h"
#include "Data/Hit.h"


class DataSvc;

class REFLMAPMTSimAlg: public AlgBase
{
    public:
        REFLMAPMTSimAlg(const std::string& name);

        ~REFLMAPMTSimAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
	uint32_t mapIdx(uint32_t tofidx, uint32_t detidx);
	//uint32_t detXIdx(uint32_t detidx){return detidx%m_detXMax;};
	//uint32_t detYIdx(uint32_t detidx){return detidx/m_detXMax;};

    private:
        uint64_t  m_count;
        uint64_t  m_totcnt;
	uint64_t  m_curcnt;
	uint32_t* m_cmap;
	int m_maxtof;
	int m_maxdet;
	//int m_detXMax;


	HitList* m_hitcol;
	NeutronPulse* m_pulse;

	DataSvc* m_svc;
	std::string m_inputfilename;
	std::ifstream m_ifstream;
};

#endif
