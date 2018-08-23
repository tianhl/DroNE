// @(#)DroNE/DroNECore:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DataInputSvc.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

#include "DroNECore/DataInputSvc.h"
#include "DroNECore/DataProvideSvc.h"

#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"


DECLARE_SERVICE(DataInputSvc);

DataInputSvc::DataInputSvc(const std::string& name)
: SvcBase(name) {
	declProp("BuffSize",  m_buffsize);
	m_dataBuff = new uint8_t[m_buffsize];
	for(uint32_t i =0; i < m_buffsize; i++) m_dataBuff[i] = 0xFF;
	m_isLastSegment = false;
	m_offset = 0;
	m_currbuffsize = 0;
}

DataInputSvc::~DataInputSvc() {
}

bool DataInputSvc::initialize() {
	return true;
}

bool DataInputSvc::finalize() {
	return true;
}

bool DataInputSvc::next() {
	return true;
}

//=====================================================================
// Protected Functions
// ====================================================================

uint8_t* DataInputSvc::readByte(){
	if(m_offset == m_currbuffsize) m_currbuffsize = nextSegment();
        //std::cout << "raw data input svc m_offset: " << m_offset << std::endl;
	//printf("ReadByte %x\n",*(m_dataBuff+m_offset));
	if(0 == m_currbuffsize)return (uint8_t*)NULL;
	else return (uint8_t*)(m_dataBuff+(m_offset++));
}

size_t DataInputSvc::nextSegment() {
        //std::cout << "nextSegment" << std::endl;
	m_offset = 0;
	if (not m_dataPvdSvc->read(m_dataBuff, m_buffsize)) m_isLastSegment = true;
        //std::cout << "nSegment get count: " << m_dataPvdSvc->count() << std::endl;
	return m_dataPvdSvc->count();
}
