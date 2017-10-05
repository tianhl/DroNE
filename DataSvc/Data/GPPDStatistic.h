#ifndef GPPD_STATISTIC_H
#define GPPD_STATISTIC_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  GPPDStatistic.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

// how to get binary string
//#include <bitset>
//bitset<32> binary_orig(uint32_orig);

#include <map>
#include <vector>
#include <algorithm>
#include "DroNECore/DataObject.h"

typedef std::map<uint32_t, uint32_t> ICMAP;
typedef std::vector<uint32_t> KEYS;

class PixelCount:public DataObject{
	public:
		PixelCount(){};
		virtual ~PixelCount(){};
                
		void clear(){
                     m_map.clear();
                     m_key.clear();
		}

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


class RunningInf:public DataObject{
	public:
		RunningInf(){m_hitcnt=0;m_evtcnt=0;m_plscnt=0;};
		virtual ~RunningInf(){};

		const uint64_t getHitCnt() const {return m_hitcnt;}
		void setHitCnt(uint64_t hitcnt){m_hitcnt  = hitcnt;}
		void addHitCnt(uint64_t hitcnt){m_hitcnt += hitcnt;}

		const uint64_t getEvtCnt() const {return m_evtcnt;}
		void setEvtCnt(uint64_t evtcnt){m_evtcnt  = evtcnt;}
		void addEvtCnt(uint64_t evtcnt){m_evtcnt += evtcnt;}

		const uint64_t getPulseCnt() const {return m_plscnt;}
		void setPulseCnt(uint64_t plscnt){m_plscnt  = plscnt;}
		void addPulseCnt(uint64_t plscnt){m_plscnt += plscnt;}
	private:
		uint64_t m_hitcnt;
		uint64_t m_evtcnt;
		uint64_t m_plscnt;
};


typedef DataList<PixelCount> PixelCountList;

#endif
