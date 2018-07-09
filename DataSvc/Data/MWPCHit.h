#ifndef MWPCHIT_H
#define MWPCHIT_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  MWPCHit.h
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//

// how to get binary string
//#include <bitset>
//bitset<32> binary_orig(uint32_orig);

#include <stdint.h>
#include "DroNECore/DataObject.h"

class MWPCHit:public DataObject{
	public:
		MWPCHit(){m_chn=0;m_tof=0;m_charge=0;m_tdataid=0;};
		MWPCHit(uint32_t chn, uint32_t tof, uint32_t charge, uint32_t tdataid){
			m_chn=chn;m_tof=tof;m_charge=charge;m_tdataid=tdataid;
		};
		virtual ~MWPCHit(){};

		const uint32_t getChannel() const { return m_chn; }
		void  setChannel(uint32_t chn) { m_chn = chn; }

		const uint32_t getTOF() const { return m_tof; }
		void  setTOF(uint32_t tof) { m_tof = tof; }

		const uint32_t getCharge() const { return m_charge; }
		void  setCharge(uint32_t charge) { m_charge = charge; }

		const uint32_t getBaseline() const { return m_baseline; }
		void  setBaseline(uint32_t baseline) { m_baseline = baseline; }

		const uint32_t getTDataID() const { return m_tdataid; }
		void  setTDataID(uint32_t tdataid) { m_tdataid = tdataid; }

	private:
		uint32_t m_chn;
		uint32_t m_tof;
		uint32_t m_charge;
		uint32_t m_baseline;
		uint32_t m_tdataid;

};

typedef DataList<MWPCHit> MWPCHitList;


#endif
