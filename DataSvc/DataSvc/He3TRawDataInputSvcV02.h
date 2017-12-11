#ifndef HE3T_RAWDATA_INPUT_SVC_V02_H
#define HE3T_RAWDATA_INPUT_SVC_V02_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  He3TRawDataInputSvcV02.h
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
class DecodeHe3TRawDataV02;

class He3TRawDataInputSvcV02 : public DataInputSvc
{
    public :

      He3TRawDataInputSvcV02(const std::string& name);
      virtual ~He3TRawDataInputSvcV02();

      bool initialize();
      bool finalize();
      bool next();

    private :
      size_t nextSegment();
      uint8_t* readByte();

    private :

      DataSvc*              m_dataSvc;
      DataProvideSvc*       m_dataPvdSvc;

      DecodeHe3TRawDataV02* m_decoder;

      uint8_t*          m_dataBuff;
      uint32_t          m_offset;
      uint32_t          m_buffsize;
      uint32_t          m_currbuffsize;
      bool              m_isLastSegment;
      //std::vector<std::string> m_inputFile;

};


#endif
