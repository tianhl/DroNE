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
