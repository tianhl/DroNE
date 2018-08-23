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

      DataSvc*              m_dataSvc;
      DecodeHe3TRawData*    m_decoder;

};


#endif
