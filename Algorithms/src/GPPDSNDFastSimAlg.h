#ifndef GPPDSNDFASTSIMALG_h
#define GPPDSNDFASTSIMALG_h
// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  GPPDSNDFastSimAlg.h
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//

#include "SniperKernel/AlgBase.h"

#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/Hit.h"

class DataSvc;

class GPPDSNDFastSimAlg: public AlgBase
{
    public:
        GPPDSNDFastSimAlg(const std::string& name);

        ~GPPDSNDFastSimAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
	int m_count;
	HitList* m_hitcol;
	EvtList* m_evtcol;
	NeutronPulse* m_pulse;
	DataSvc* m_svc;

    private: 
//	std::vector<int> m_buffer;
	void getXYChannel(uint32_t pid, uint32_t &xchannel, uint32_t &ychannel);
	void simulation(uint32_t xchannel, uint32_t ychannel, uint32_t tdata);
        void calculation(uint32_t position, uint32_t timedata, bool setoffset);
//	int scan(int *Pos,int l,int &ThePosition,int &Real); 
//	int sort(int *src, int len);
//	int calculation(int &X,int &Y,int &Effect);
};

#endif
