#ifndef RUNNINGINFALG_h
#define RUNNINGINFALG_h
// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  RunningInfAlg.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

#include "SniperKernel/AlgBase.h"
#include <string>

class DataSvc;

class RunningInfAlg: public AlgBase
{

    public:
        RunningInfAlg(const std::string& name);

        ~RunningInfAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:

	DataSvc* m_svc;
        uint32_t m_tofbins;
        uint32_t m_tofstep;
};

#endif
