#ifndef DIFF_SCINTILLANT_H
#define DIFF_SCINTILLANT_H

// how to get binary string
//#include <bitset>
//bitset<32> binary_orig(uint32_orig);

#include <stdint.h>
#include "DroNECore/DataObject.h"

class Hit:public DataObject{
	public:
		Hit(){m_chn=0;m_tof=0;};
		Hit(uint32_t chn, uint32_t tof){m_chn=chn;m_tof=tof;};
		virtual ~Hit(){};

		const uint32_t getChannel() const { return m_chn; }
		void  setChannel(uint32_t chn) { m_chn = chn; }

		const uint32_t getTOF() const { return m_tof; }
		void  setTOF(uint32_t tof) { m_tof = tof; }

	private:
		uint32_t m_chn;
		uint32_t m_tof;

};

typedef DataList<Hit> HitList;


#endif
