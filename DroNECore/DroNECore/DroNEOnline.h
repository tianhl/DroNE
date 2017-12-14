#ifndef DRONE_ONLINE_H
#define DRONE_ONLINE_H
// @(#)DroNE/DroNECore:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DroNEOnline.h
//  
//
//  Created by Haolai TIAN 10th Dec. 2017.
//
//

#include "SniperKernel/Task.h"

class DroNEOnline : public Task{
   public:
      DroNEOnline(const std::string& name);
      virtual ~DroNEOnline();
      virtual bool execute();
   private:
      bool m_onlinemode;
};

#endif
