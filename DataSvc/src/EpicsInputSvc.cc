// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  EpicsInputSvc.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include "DataSvc/EpicsInputSvc.h"
#include "DataSvc/EpicsV4Hh/neutronClientStartMonitor.hh"
#include "DroNECore/DataSvc.h"

#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"

#include <iostream>
#include <list>
#include <string>
#include <ctime>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include "Data/Pulse.h"
#include "Data/Evt.h"

DECLARE_SERVICE(EpicsInputSvc);

DynamicThreadedQueue<MonitorItem*> EpicsInputSvc::dataQueue;
EpicsInputSvc::EpicsInputSvc(const std::string& name)
: DataInputSvc(name) {
	declProp("Channel", m_epicsChannel);

	m_EpicsMonitor= NULL;
	m_curDataItem = NULL;
}

EpicsInputSvc::~EpicsInputSvc() {
}

bool EpicsInputSvc::initialize() {

	LogInfo << "EpicsInputSvc initialize " << std::endl;


	SniperPtr<DataSvc> pDSvc("DataSvc");
	if ( pDSvc.invalid()) throw SniperException("DataSvc is invalid!");
	m_dataSvc = pDSvc.data();

	m_client = new boost::thread(boost::bind(&EpicsInputSvc::epicsClient, this));
	
	m_client->detach();
	return true;
}



bool EpicsInputSvc::finalize() {
	//m_EpicsMonitor->stopMonitor();
	if(m_client) m_client->interrupt();
	return true;
}

bool EpicsInputSvc::next() {
	NeutronPulse* npulse = m_dataSvc->getObj<NeutronPulse>("/pulse");
	EvtList* evtcol      = m_dataSvc->getObj<EvtList>("/pulse/evts");
	evtcol->clear();
	Evt* evt;

	if(false == popDataItem())return false;

	MonitorData* pData = m_curDataItem->getData();
	npulse->setInstrument(pData->pPulse->mSpecID);
	npulse->setDetector(pData->pPulse->mDetTypeID);
	//npulse->(pData->pPulse->mProtonCharge;
	//npulse->(pData->pPulse->mPulseT0High;
	//npulse->(pData->pPulse->mPulseT0Low;
	//uint32_t hitCnt       = pData->pPulse->mHitCounts;

	for(uint32_t idx = 0; idx < pData->pPulse->mHitCounts; idx++){
		evt = evtcol->add_item();
		evt->setPixelID(pData->pPIDList[idx]);
		evt->setTOF(pData->pTOFList[idx]);
                evt->setX(0);
                evt->setY(0);
	}

	eraseDataItem();
	return true;

}

//=========================================================
//private method: thread
//========================================================
void EpicsInputSvc::pushDataItem(NeutronPulseData* pNeutronPulseData){
	NeutronPulseData* pulse = new NeutronPulseData;
	memcpy(pulse, pNeutronPulseData, sizeof(*pNeutronPulseData));
	uint32_t hitCnt = pulse->mHitCounts;
	uint32_t *tofList = new uint32_t[hitCnt];
	uint32_t *pidList = new uint32_t[hitCnt];
	memcpy(tofList, pNeutronPulseData->mMonitorNeutronEventData.pTimeOfFlight, 
			hitCnt*sizeof(*(pNeutronPulseData->mMonitorNeutronEventData.pTimeOfFlight)));
	memcpy(pidList, pNeutronPulseData->mMonitorNeutronEventData.pPixelID,     
			hitCnt*sizeof(*(pNeutronPulseData->mMonitorNeutronEventData.pPixelID)));

	MonitorData* pMonitorData = new MonitorData;
	pMonitorData->pPulse   = pulse;
	pMonitorData->pTOFList = tofList;
	pMonitorData->pPIDList = pidList;
	MonitorItem* dataItem = new MonitorItem(pMonitorData, 1);
	dataQueue.put(dataItem);
	//for(int i = 0; i < hitCnt; i++){
        //  std::cout << "RAW TOF1: " << tofList[i] << " RAW PID1: " << pidList[i] << std::endl; 
        //  std::cout << "RAW TOF2: " << pNeutronPulseData->mMonitorNeutronEventData.pTimeOfFlight[i] << " RAW PID2: " << pNeutronPulseData->mMonitorNeutronEventData.pPixelID[i] << std::endl; 
	//}
}

void functionWrapper(NeutronPulseData* pNeutronPulseData){
	EpicsInputSvc::pushDataItem(pNeutronPulseData);
}
void EpicsInputSvc::epicsClient(){
	m_EpicsMonitor = new neutronClientStartMonitor(m_epicsChannel);
	m_EpicsMonitor->startMonitor(functionWrapper);

}
//=========================================================
//private method: main
//========================================================


//void EpicsInputSvc::popDataItem(){
//	m_curDataItem = dataQueue.get();
//}

bool EpicsInputSvc::popDataItem(){
	m_curDataItem = dataQueue.get();
        if(NULL == m_curDataItem)return false;
        else return true;
}

bool EpicsInputSvc::eraseDataItem(){
	if(not m_curDataItem) return false;
	delete m_curDataItem->getData()->pPulse; 
	delete []m_curDataItem->getData()->pTOFList; 
	delete []m_curDataItem->getData()->pPIDList; 
	delete []m_curDataItem->getData();
	delete m_curDataItem;
	m_curDataItem = NULL;
	return true;
}

