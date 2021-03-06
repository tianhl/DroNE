#ifndef EPICS_INPUT_SVC_H
#define EPICS_INPUT_SVC_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  EpicsInputSvc.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

#include <vector>
#include <string>

#include "DroNECore/DataInputSvc.h"
#include "DroNECore/DynamicThreadedQueue.h"
#include "DataSvc/EpicsV4Hh/neutronClientStartMonitor.hh" 


class DataSvc;

struct MonitorData{
    NeutronPulseData* pPulse;
    uint32_t*         pTOFList;
    uint32_t*         pPIDList;
};

typedef DataItem<MonitorData*> MonitorItem;

class EpicsInputSvc : public DataInputSvc
{
    public :

      EpicsInputSvc(const std::string& name);
      virtual ~EpicsInputSvc();

      bool initialize();
      bool finalize();
      bool next();

    private :
      // main
      //void popDataItem(); 
      bool popDataItem(); 
      bool eraseDataItem();
      // thread
      void epicsClient();
      friend void functionWrapper(NeutronPulseData* pNeutronPulseData);
      static void pushDataItem(NeutronPulseData* pNeutronPulseData);

    private :

      DataSvc*          m_dataSvc;
      static DynamicThreadedQueue<MonitorItem*> dataQueue;
      neutronClientStartMonitor* m_EpicsMonitor;
      MonitorItem* m_curDataItem;
      boost::thread *m_client;
      std::string m_epicsChannel;

};


#endif
