#ifndef MR_DATA_SVC_H
#define MR_DATA_SVC_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  MRDataSvc.h
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//

#include "DroNECore/DataSvc.h"

class MRDataSvc : public DataSvc
{
    public :

      MRDataSvc(const std::string& name);
      virtual ~MRDataSvc();

      bool initialize();
      bool finalize();

    private :
      //uint32_t         m_tofbins;

};


#endif
