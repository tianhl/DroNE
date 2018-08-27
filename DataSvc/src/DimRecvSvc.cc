// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DimRecvSvc.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include "DataSvc/DimRecvSvc.h"

#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"
#include<cstdlib>

extern "C"{
#include "dic.h"
}


DECLARE_SERVICE(DimRecvSvc);

DynamicThreadedQueue<Item*> DimRecvSvc::dataQueue;

//=========================================================
DimRecvSvc::DimRecvSvc(const std::string& name)
: DataProvideSvc(name)
{
	declProp("DataSize", m_dataSize);
        declProp("DimServerName", m_dimServer);

	m_client = NULL;
	m_curDataItem = NULL;
	m_current = NULL;
	m_currSize = 0;
	m_offset = 0;
	m_dimID = -1;
}

DimRecvSvc::~DimRecvSvc(){
}

bool DimRecvSvc::initialize(){
        LogInfo << " Max Size of Queue (0 means nolimit): " << m_dataSize << std::endl;
        if(m_dataSize>0)dataQueue.setMaxSize(m_dataSize);
	m_client = new boost::thread(boost::bind(&DimRecvSvc::dimClient, this));
	m_client->detach();
	return true;
}

bool DimRecvSvc::finalize() {
	// need process all the data item in the queue
	if(-1 != m_dimID)dic_release_service(m_dimID);
	if(m_client) m_client->interrupt();
	return true;
}

bool DimRecvSvc::eraseDataItem(){
	if(not m_curDataItem) return false;
	delete []m_curDataItem->getData();
	delete m_curDataItem;
	m_curDataItem = NULL;
	return true;
}

bool DimRecvSvc::read(uint8_t* buff, uint32_t buffsize){

	uint8_t* pBuff    = buff;
	size_t   needsize = buffsize;
	m_currSize        = 0;

	for(uint32_t i = 0; i < buffsize; i++)pBuff[i]=0x00; // initialize
	//if(NULL == m_curDataItem) popDataItem(); // waiting for input stream
	if(NULL == m_curDataItem) {
		if(false == popDataItem())return false; // if there is noe data
	}

	while(needsize>0){
		if(NULL == m_curDataItem)return true;
		else m_current = m_curDataItem->getData();

		size_t length = m_curDataItem->getSize()-m_offset;
		if(0 == length) return eraseDataItem(); //all data have been read in this item
		size_t cpsize = (needsize < length)?needsize:length;

		if(copyBuff(pBuff, cpsize, m_current+m_offset)){
			pBuff      += cpsize;
			m_currSize += cpsize;
			needsize   -= cpsize;
		}
	}

	return true;
}

size_t DimRecvSvc::count() const{
	return m_currSize;
}

//=========================================================
//private method: thread
//========================================================
void DimRecvSvc::pushDataItem(uint8_t* item, size_t size) {
	uint8_t* data = new uint8_t[size];
	memcpy(data, (uint8_t*)item, size);
	Item* dataItem = new Item(data, size);
	dataQueue.put(dataItem);
}


void functionWrapper(void* flag, void* buff, int* size){
	if(1200 == *((int*)flag)) DimRecvSvc::pushDataItem((uint8_t*)buff, size_t(*size));
	//for(int i = 0; i < *size; i++){
	//    printf("functionWrapper: %x\n", *((unsigned char*)buff+i));
	//}
}

void DimRecvSvc::dimClient(){
	char* envNode = getenv("DIM_DNS_NODE");
	char* envPort = getenv("DIM_DNS_PORT");
	if(envNode and envPort) {
		LogInfo << "DIM_DNS_NODE: " << envNode << std::endl; 
		LogInfo << "DIM_DNS_PORT: " << envPort << std::endl; 
		static int no_link = -1;
		//static char aux[80];
		//sprintf(aux,"%s",m_dimServer.c_str());
		m_dimID = dic_info_service_stamped( m_dimServer.c_str(), MONITORED, 0, 0, 0, functionWrapper, 1200, &no_link, 4 );  
		//m_dimID = dic_info_service_stamped( aux, MONITORED, 0, 0, 0, functionWrapper, 1200, &no_link, 4 );  
	}
	else throw SniperException("DIM_DNS_NODE or DIM_DNS_PORT undefined!");
}

//=========================================================
//private method: main
//========================================================

//void DimRecvSvc::popDataItem(){
//	m_curDataItem = dataQueue.get();
//	m_offset = 0;
//}

bool DimRecvSvc::popDataItem(){
	m_curDataItem = dataQueue.getUB();
	if(NULL == m_curDataItem) return false; 
	m_offset = 0;
	return true;
}

bool DimRecvSvc::copyBuff(uint8_t* destBuff, size_t size, uint8_t* srcBuff){
	memcpy(destBuff, srcBuff, size);
	m_offset += size;
	return true;
}
