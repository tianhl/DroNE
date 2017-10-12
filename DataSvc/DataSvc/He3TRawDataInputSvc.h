#ifndef HE3T_RAWDATA_INPUT_SVC_H
#define HE3T_RAWDATA_INPUT_SVC_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  He3TRawDataInputSvc.h
//  
//
//  Created by Haolai TIAN 10th Oct. 2017.
//
//

#include <vector>
#include <string>

#include "DroNECore/DataInputSvc.h"

class DataSvc;
class DataProvideSvc;
class DecodeHe3TRawData;

class He3TRawDataInputSvc : public DataInputSvc
{
    public :

      He3TRawDataInputSvc(const std::string& name);
      virtual ~He3TRawDataInputSvc();

      bool initialize();
      bool finalize();
      bool next();

    private :
      size_t nextSegment();
      uint8_t* readByte();

    private :

      DataSvc*              m_dataSvc;
      DataProvideSvc*       m_dataPvdSvc;

      DecodeHe3TRawData*    m_decoder;

      uint8_t*          m_dataBuff;
      uint32_t          m_offset;
      uint32_t          m_buffsize;
      uint32_t          m_currbuffsize;
      bool              m_isLastSegment;
      //std::vector<std::string> m_inputFile;

};


#endif
