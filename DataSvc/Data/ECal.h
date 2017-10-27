#ifndef ECAL_H
#define ECAL_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  ECal.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

#include <stdlib.h>
#include <vector>

#include "DroNECore/DataObject.h"
using namespace std;

class ECal:public DataObject{
	public:
		ECal(){};
		virtual ~ECal(){};

		const uint32_t getNTube() const {return m_ntube; }
		void setNTube(uint32_t nt) {m_ntube = nt; }

		const uint32_t getNSample() const {return m_nsamp; }
		void setNSample(uint32_t ns) {m_nsamp = ns; }

		const uint32_t getNStage() const {return m_nstag; }
		void setNStage(uint32_t ns) {m_nstag = ns; }

	private:
		uint32_t m_ntube;	
		uint32_t m_nsamp;	
		uint32_t m_nstag;	

};

#endif
