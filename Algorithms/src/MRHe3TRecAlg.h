#ifndef MRHE3TRECALG_h
#define MRHE3TRECALG_h
// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN  2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  MRHe3TRecAlg.h
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//  Supported by Du Rong since 1st Dec. 2017.
//
//

#include "SniperKernel/AlgBase.h"

#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/ECal.h"
#include "Data/He3THit.h"

class DataSvc;

class MRHe3TRecAlg: public AlgBase
{
    public:
        MRHe3TRecAlg(const std::string& name);

        ~MRHe3TRecAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
	int m_count;
	He3THitList*  m_hitcol;
	EvtList*      m_evtcol;
	NeutronPulse* m_pulse;
	ECal*         m_ecal;
	DataSvc* m_svc;
        int m_threshold;
        int m_bins;

};

#endif
