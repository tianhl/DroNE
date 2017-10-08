#ifndef REFLMWPCRRECALG_h
#define REFLMWPCRRECALG_h
// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN  2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  REFLMWPCRecAlg.h
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

class REFLMWPCRecAlg: public AlgBase
{
    public:
        REFLMWPCRecAlg(const std::string& name);

        ~REFLMWPCRecAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
	int m_count;
	MWPCHitList*  m_hitcol;
	EvtList*      m_evtcol;
	NeutronPulse* m_pulse;
	DataSvc* m_svc;

    private: 
	std::vector<int> m_buffer;
	std::vector<int> m_charge;
	void reconstruct(int time);
	int scan(int *Pos,int *Cha, int l,int &ThePosition,int &Real); 
	int calculation(int &X,int &Y,int &Effect);
};

#endif
