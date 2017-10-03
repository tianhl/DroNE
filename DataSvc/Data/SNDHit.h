#ifndef SNDHIT_H
#define SNDHIT_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  Hit.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

// how to get binary string
//#include <bitset>
//bitset<32> binary_orig(uint32_orig);

#include <stdint.h>
#include "DroNECore/DataObject.h"

class SNDHit:public DataObject{
	public:
		SNDHit(){m_chn=0;m_tof=0;};
		SNDHit(uint32_t chn, uint32_t tof){m_chn=chn;m_tof=tof;};
		virtual ~SNDHit(){};

		const uint32_t getChannel() const { return m_chn; }
		void  setChannel(uint32_t chn) { m_chn = chn; }

		const uint32_t getTOF() const { return m_tof; }
		void  setTOF(uint32_t tof) { m_tof = tof; }

	private:
		uint32_t m_chn;
		uint32_t m_tof;

};

typedef DataList<SNDHit> SNDHitList;


#endif
