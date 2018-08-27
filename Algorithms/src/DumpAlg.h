#ifndef DUMPALG_h
#define DUMPALG_h
// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DumpAlg.h
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

#include "time.h"

class DataSvc;

class DumpAlg: public AlgBase
{

    public:
        DumpAlg(const std::string& name);

        ~DumpAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        int      m_int;
        uint64_t m_count;
	uint64_t m_evtnum;
	uint64_t m_hitnum;
	uint64_t m_pulsenum;
	clock_t  m_start;
	clock_t  m_finish;
	std::string m_outputfile;
	std::ofstream m_ofstream;
	std::vector<uint64_t> m_xvect;
	std::vector<uint64_t> m_yvect;

	DataSvc* m_svc;
};

#endif
