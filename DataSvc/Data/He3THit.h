#ifndef HE3THIT_H
#define HE3THIT_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  He3THit.h
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

class He3THit:public DataObject{
	public:
		He3THit(){m_tub=0;m_tof=0;m_qu=0;m_ql=0;};
		He3THit(uint32_t tub, uint32_t tof, uint32_t qu, uint32_t ql){m_tub=tub;m_tof=tof;m_qu=qu;m_ql=ql;};
		virtual ~He3THit(){};

		const uint32_t getTube() const { return m_tub; }
		void  setTube(uint32_t tub) { m_tub = tub; }

		const uint32_t getTOF() const { return m_tof; }
		void  setTOF(uint32_t tof) { m_tof = tof; }

		const uint32_t getSu() const { return m_su; }
		void  setSu(uint32_t su) { m_su = su; }

		const uint32_t getSl() const { return m_sl; }
		void  setSl(uint32_t sl) { m_sl = sl; }

		const uint32_t getQu() const { return m_qu; }
		void  setQu(uint32_t qu) { m_qu = qu; }

		const uint32_t getQl() const { return m_ql; }
		void  setQl(uint32_t ql) { m_ql = ql; }

	private:
		uint32_t m_tub;
		uint32_t m_tof;
		uint32_t m_su;
		uint32_t m_sl;
		uint32_t m_qu;
		uint32_t m_ql;

};

typedef DataList<He3THit> He3THitList;


#endif
