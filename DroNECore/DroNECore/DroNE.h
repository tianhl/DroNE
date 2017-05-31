#ifndef DRONE_H
#define DRONE_H
// @(#)DroNE/DroNECore:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DroNE.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

#include "SniperKernel/Task.h"

class DroNE : public Task{
   public:
      DroNE(const std::string& name);
      virtual ~DroNE();
      virtual bool execute();
   private:
      bool m_onlinemode;
};

#endif
