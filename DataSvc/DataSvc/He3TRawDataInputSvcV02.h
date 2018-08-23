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

      DataSvc*              m_dataSvc;
      DecodeHe3TRawDataV02* m_decoder;

};


#endif
