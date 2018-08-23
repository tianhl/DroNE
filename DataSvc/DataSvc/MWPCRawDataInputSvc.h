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

      DataSvc*              m_dataSvc;
      DecodeMWPCRawData* m_decoder;
};


#endif
