#ifndef DATA_INPUT_SVC_H
#define DATA_INPUT_SVC_H
// @(#)DroNE/DroNECore:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DataInputSvc.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include <vector>
#include <string>

#include "SniperKernel/SvcBase.h"
#include "DroNECore/DataProvideSvc.h"


class DataInputSvc : public SvcBase
{
    public :

      DataInputSvc(const std::string& name);
      virtual ~DataInputSvc();

      virtual bool initialize();
      virtual bool finalize();
      virtual bool next();
    protected :
      size_t nextSegment();
      uint8_t* readByte();

    protected :
      DataProvideSvc*   m_dataPvdSvc;

    protected :
      uint8_t*          m_dataBuff;
      uint32_t          m_offset;
      uint32_t          m_buffsize;
      uint32_t          m_currbuffsize;
      bool              m_isLastSegment;


};


#endif
