#ifndef DIMRECVSVC_H
#define DIMRECVSVC_H


#include <string>
#include <vector>
#include <fstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "DataSvc/DataProvideSvc.h"
#include "DataSvc/DynamicThreadedQueue.h"
#include "DataSvc/DynamicSingleton.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

//class DataItem{
//	public:
//		DataItem(uint8_t* p, size_t s):m_pData(p),m_size(s){};
//		virtual ~DataItem(){};
//
//		uint8_t* getData(){return m_pData;};
//		size_t   getSize(){return m_size;};
//	private:
//		DataItem(){};
//		// set const?
//		uint8_t*  m_pData;
//		size_t    m_size;
//};

typedef DataItem<uint8_t*> Item;

class DimRecvSvc : public DataProvideSvc{
	public:
		DimRecvSvc(const std::string& name);
		virtual ~DimRecvSvc();

		virtual bool   initialize();
		virtual bool   finalize();
		virtual bool   read(uint8_t* buff, uint32_t size);
		virtual size_t count() const;
	private:
		// thread
		void dimClient();
		static void pushDataItem(uint8_t* item, size_t size);
		friend void functionWrapper(void* tag, void* item, int* size);
		//friend void functionWrapper(int* tag, void* item, int* size);
		// main
		bool eraseDataItem();
		void popDataItem();
		bool copyBuff(uint8_t* destBuff, size_t size, uint8_t* srcBuff);
	private:
		static DynamicThreadedQueue<Item*> dataQueue;
		Item* m_curDataItem;
		uint8_t*  m_current;
		size_t m_dataSize; // to upper
                std::string m_dimServer;
		size_t m_currSize;
		size_t m_offset;   // dimbuff offset
		boost::thread *m_client;
		int    m_dimID;

};

// set singleton?
typedef DynamicSingleton<DimRecvSvc> DAQInputSvc;

#endif
