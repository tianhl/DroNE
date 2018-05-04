#ifndef MR_DATA_SVC_V02_H
#define MR_DATA_SVC_V02_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2018
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2018 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  MRDataSvcV02.h
//  
//
//  Created by Haolai TIAN 4th May 2018.
//
//

#include "DroNECore/DataSvc.h"

class MRDataSvcV02 : public DataSvc
{
    public :

      MRDataSvcV02(const std::string& name);
      virtual ~MRDataSvcV02();

      bool initialize();
      bool finalize();

    private :
      //uint32_t         m_tofbins;

};


#endif
