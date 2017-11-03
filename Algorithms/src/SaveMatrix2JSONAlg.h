#ifndef SAVEMATRIX2JSONALG_h
#define SAVEMATRIX2JSONALG_h
// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  SaveMatrix2JSONAlg.h
//  
//
//  Created by Haolai TIAN 3rd Nov. 2017.
//
//

#include "SniperKernel/AlgBase.h"
#include <string>

#include <iostream>
#include <fstream>
class DataSvc;

class SaveMatrix2JSONAlg: public AlgBase
{

    public:
        SaveMatrix2JSONAlg(const std::string& name);

        ~SaveMatrix2JSONAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:

	DataSvc* m_svc;
	std::string m_outputfile;
        uint32_t m_pidstart;
        uint32_t m_pidsize;
};

#endif
