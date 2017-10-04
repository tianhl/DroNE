#ifndef SANS_DATA_SVC_H
#define SANS_DATA_SVC_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  SANSDataSvc.h
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//

#include "DroNECore/DataSvc.h"

class SANSDataSvc : public DataSvc
{
    public :

      SANSDataSvc(const std::string& name);
      virtual ~SANSDataSvc();

      bool initialize();
      bool finalize();

    private :
      //uint32_t         m_tofbins;

};


#endif
