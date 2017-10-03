#ifndef GPPD_DATA_SVC_H
#define GPPD_DATA_SVC_H
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

#include "DroNECore/DataSvc.h"

class GPPDDataSvc : public DataSvc
{
    public :

      GPPDDataSvc(const std::string& name);
      virtual ~GPPDDataSvc();

      bool initialize();
      bool finalize();

    private :
      //uint32_t         m_tofbins;

};


#endif
