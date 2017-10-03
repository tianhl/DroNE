#ifndef REFL_DATA_SVC_H
#define REFL_DATA_SVC_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  REFLDataSvc.h
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//

#include "DroNECore/DataSvc.h"

class REFLDataSvc : public DataSvc
{
    public :

      REFLDataSvc(const std::string& name);
      virtual ~REFLDataSvc();

      bool initialize();
      bool finalize();

    private :
      //uint32_t         m_tofbins;

};


#endif
