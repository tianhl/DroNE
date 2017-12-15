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
//  SaveDataAlg.h
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

class SaveDataAlg: public AlgBase
{

    public:
        SaveDataAlg(const std::string& name);

        ~SaveDataAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        uint64_t m_count;
	uint64_t m_evtnum;
	uint64_t m_hitnum;
	uint64_t m_pulsenum;
	clock_t  m_start;
	clock_t  m_finish;
	std::string m_outputfile1;
	std::string m_outputfile2;
	std::ofstream m_ofstream1;
	std::ofstream m_ofstream2;
	std::vector<uint64_t> m_xvect;
	std::vector<uint64_t> m_yvect;

	DataSvc* m_svc;
};

#endif
