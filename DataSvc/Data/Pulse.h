#ifndef DIFF_PULSE_H
#define DIFF_PULSE_H

#include <stdlib.h>
#include <vector>

#include "DataObject.h"
using namespace std;

class NeutronPulse:public DataObject{
	public:
		NeutronPulse(){};
		virtual ~NeutronPulse(){};

		const uint32_t getInstrument() const {return m_ins; }
		void setInstrument(uint32_t ins) {m_ins = ins; }

		const uint32_t getDetector() const {return m_det; }
		void setDetector(uint32_t det) {m_det = det; }

		const uint32_t getModule() const {return m_mod; }
		void setModule(uint32_t mod) {m_mod = mod; }

		const uint32_t getRunMode() const {return m_run; }
		void setRunMode(uint32_t run) {m_run = run; }

		const uint32_t getFrame() const {return m_fam; }
		void setFrame(uint32_t fam) {m_fam = fam; }

		const uint32_t getVersion() const {return m_ver; }
		void setVersion(uint32_t ver) {m_fam = ver; }

		const uint32_t getT0() const {return m_t0c; }
		void setT0(uint32_t t0c) {m_t0c = t0c; }

		const uint32_t getStatus() const {return m_sta; }
		void setStatus(uint32_t sta) {m_sta = sta; }

		const uint32_t getCount() const {return m_cnt; }
		void setCount(uint32_t cnt) {m_cnt = cnt; }

	private:
		uint32_t m_ins;	
		uint32_t m_det;	
		uint32_t m_mod;	
		uint32_t m_run;	
		uint32_t m_fam;	
		uint32_t m_ver;	
		uint32_t m_t0c;	
		uint32_t m_sta;	
		uint32_t m_cnt;	

};

#endif
