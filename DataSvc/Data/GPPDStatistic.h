#ifndef GPPD_STATISTIC_H
#define GPPD_STATISTIC_H

// how to get binary string
//#include <bitset>
//bitset<32> binary_orig(uint32_orig);

#include <map>
#include <vector>
#include <algorithm>
#include "DataObject.h"

typedef std::map<uint32_t, uint32_t> ICMAP;
typedef std::vector<uint32_t> KEYS;

class PixelCount:public DataObject{
/** @class GPPDStatistic 

    GPPDStatistic is a data object for GPPD,
    presenting statistic information in DataSvc. 

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
		PixelCount(){};
		virtual ~PixelCount(){};

		uint32_t addCount(uint32_t pixel_id, uint32_t count){
			ICMAP::iterator it = m_map.find(pixel_id);
			if(it == m_map.end()) {
				m_map.insert(ICMAP::value_type(pixel_id, count));
				m_key.push_back(pixel_id);
				std::sort(m_key.begin(), m_key.end());
			}
			else{

				uint32_t c = it->second; 
				it->second = c + count;
			}

				return getCount(pixel_id);
			};
			uint32_t getCount(uint32_t pixel_id){
				ICMAP::iterator it = m_map.find(pixel_id);
				if(it == m_map.end()) return 0;
				else return it->second;
			};

			KEYS& getPixelIDs(){return m_key;}


			private:
			ICMAP   m_map;
			KEYS    m_key;


		};



#endif
