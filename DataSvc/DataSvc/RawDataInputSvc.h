#ifndef RAWDATA_INPUT_SVC_H
#define RAWDATA_INPUT_SVC_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DataSvc.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

#include <vector>
#include <string>

#include "DroNECore/DataInputSvc.h"

class DataSvc;
class DecodeRawData;

class RawDataInputSvc : public DataInputSvc
{
    public :

      RawDataInputSvc(const std::string& name);
      virtual ~RawDataInputSvc();

      bool initialize();
      bool finalize();
      bool next();

    private :

      DataSvc*          m_dataSvc;
      DecodeRawData*    m_decoder;


};


#endif
