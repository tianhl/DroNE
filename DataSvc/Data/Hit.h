#ifndef DIFF_SCINTILLANT_H
#define DIFF_SCINTILLANT_H

// how to get binary string
//#include <bitset>
//bitset<32> binary_orig(uint32_orig);

#include <stdint.h>
#include "DataObject.h"

class Hit:public DataObject{
/** @class Hit 

    Evt is a data object for GPPD,
    presenting detector fire in DataSvc. 

    @author Haolai TIAN, CSNS, IHEP, CAS
    @date 12/12/2016

    Copyright &copy; 2016-now CSNS, IHEP, CAS

    This file is part of DroNE.

    DroNE is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    DroNE is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
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
