// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  SaveMatrix2JSONAlg.cc
//  
//
//  Created by Haolai TIAN 3rd Nov. 2017.
//
//
#include "SaveMatrix2JSONAlg.h"
#include "DroNECore/DataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include "stdlib.h"
#include "Data/GPPDStatistic.h"


DECLARE_ALGORITHM(SaveMatrix2JSONAlg);

SaveMatrix2JSONAlg::SaveMatrix2JSONAlg(const std::string& name)
: AlgBase(name)
{
	declProp("PIDStart", m_pidstart);
	declProp("PIDSize",  m_pidsize);
	declProp("OutputFileName",m_outputfile);
}

SaveMatrix2JSONAlg::~SaveMatrix2JSONAlg()
{
}

bool SaveMatrix2JSONAlg::initialize()
{
	LogInfo << " initialized successfully" << std::endl;

	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) return false;
	m_svc = pSvc.data();


	return true;
}

bool SaveMatrix2JSONAlg::execute()
{

	return true;
}

bool SaveMatrix2JSONAlg::finalize()
{

	PixelCountList* pcs  = m_svc->getObj<PixelCountList>("/statistic/pixel_counts");
	uint32_t   size  = pcs->size();


	std::stringstream retVal;
	//KEYS& pids = pc->getPixelIDs();
	retVal << "[";
	for(uint32_t i = 0; i < uint32_t(m_pidsize); i++){
		if(0<i)retVal << ",";
		retVal << "[";
		for(uint32_t j = 0; j < size; j++){
			if(0<j)retVal << ",";
			retVal << pcs->at(j)->getCount(i+m_pidstart);
		}
		retVal << "]";
	}
	retVal << "]";

	//std::cout << "SaveMatrix2Json: " << retVal.str() << std::endl;
	std::ofstream m_ofstream;
	m_ofstream.open(m_outputfile.c_str(),ios::out);
	m_ofstream<<retVal.str()<<endl;
	m_ofstream.close();

	return true;
}
