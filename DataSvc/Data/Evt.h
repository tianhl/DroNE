#ifndef DIFF_EVENT_H
#define DIFF_EVENT_H

#include "DataObject.h"


class Evt:public DataObject{
/** @class Evt 

    Evt is a data object for GPPD,
    presenting position, pixel id and tof of event in DataSvc. 

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
		const uint64_t getPixelID() const {return m_pixelid; }
		void setPixelID(uint64_t id) { m_pixelid = id; }

		const uint32_t getTOF() const {return m_tof; }
		void setTOF(uint32_t tof) { m_tof = tof; }

		const uint32_t getX() const {return m_x; }
		void setX(uint32_t x) { m_x = x; }

		const uint32_t getY() const {return m_y; }
		void setY(uint32_t y) { m_y = y; }
	private:
		uint64_t m_pixelid;
		uint32_t m_tof;
		uint32_t m_x;
		uint32_t m_y;
		// pixelcollection?
};

typedef DataList<Evt> EvtList;
#endif
