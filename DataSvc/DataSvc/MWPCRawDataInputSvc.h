#ifndef MWPC_RAWDATA_INPUT_SVC_H
#define MWPC_RAWDATA_INPUT_SVC_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2018
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2018 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  MWPCRawDataInputSvc.h
//  
//
//  Created by Haolai TIAN 4th May 2018.
//
//

#include <vector>
#include <string>

#include "DroNECore/DataInputSvc.h"

class DataSvc;
class DataProvideSvc;
class DecodeMWPCRawData;

class MWPCRawDataInputSvc : public DataInputSvc
{
    public :

      MWPCRawDataInputSvc(const std::string& name);
      virtual ~MWPCRawDataInputSvc();

      bool initialize();
      bool finalize();
      bool next();

    private :
      size_t nextSegment();
      uint8_t* readByte();

    private :

      DataSvc*              m_dataSvc;
      DataProvideSvc*       m_dataPvdSvc;

      DecodeMWPCRawData* m_decoder;

      uint8_t*          m_dataBuff;
      uint32_t          m_offset;
      uint32_t          m_buffsize;
      uint32_t          m_currbuffsize;
      bool              m_isLastSegment;
      //std::vector<std::string> m_inputFile;

};


#endif
