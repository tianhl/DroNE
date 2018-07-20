#ifndef MRMWPCRRECALG_h
#define MRMWPCRRECALG_h
// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN & Haiyun Teng 2018
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2018 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  MRMWPCRecAlg.h
//  
//
//  Created by Haolai TIAN & Haiyun Teng 10th July. 2018.
//
//

#include "SniperKernel/AlgBase.h"

#include "Data/Pulse.h"
#include "Data/EvtD.h"
#include "Data/MWPCHit.h"


#define SLICESIZE 50000 //max slice(800ns) number in 40ms
#define EVENTSIZE 1600000 // max event number in 40ms
#define SIGNALSIZE 144 // XCHANNELNUM+YCHANNELNUM
#define XCHANNELNUM 50
#define YCHANNELNUM 94
#define DEBUG 0
typedef struct {
	float center;
	float charge;
	int time;
	int nstrip;
} Hit;

class DataSvc;

class MRMWPCRecAlg: public AlgBase
{
	public:
		MRMWPCRecAlg(const std::string& name);

		~MRMWPCRecAlg();

		bool initialize();
		bool execute();
		bool finalize();

	private:
		int m_count;
		MWPCHitList*  m_hitcol;
		EvtDList*      m_evtcol;
		NeutronPulse* m_pulse;
		DataSvc* m_svc;

	private: 

		int signalNum, intervalNum;
		uint32_t sliceCount, pre_eventNumber, cur_eventNumber, slice_eventNumber, totalEvent, validEvent;
		uint32_t *eventMapT; //[SLICESIZE][SIGNALSIZE];
		uint16_t *eventMapC; //[SLICESIZE][SIGNALSIZE];
		uint16_t *eventMapSlice, *eventMapMin, *eventMapMax; //[0-SLICESIZE]
		uint16_t eventMapIdx;
		uint16_t *sBuffer;
		uint16_t sBufferIdx;
		uint32_t *signalMapXT, *signalMapYT; // inordered tof record for each slice signal in X and Y
		uint16_t *signalMapXM, *signalMapYM, *signalMapXS, *signalMapYS, *signalMapXR, *signalMapYR; // matchN, slice, range((strip_high<<8)|strip_low) for each tof from above record in X and Y 
		uint32_t *signalMapXIdx, *signalMapYIdx; // end number of record for each slice signal in X and y
		uint32_t *matchX, *matchY; // idx of matched Y for X
		uint32_t matchIdx;

		Hit hitX, hitY;

		int QMIN;
		int QMAX;
		int MATCHWINDOW;
		float QSUM;

	private:

		void scanSlice(unsigned int *ch, unsigned int *Q, unsigned int *base, unsigned int *tof, unsigned int *sliceId, const int& size);
		void processMap();
		void matchNeutron();

		inline void getHit(uint16_t sliceIdx, uint16_t range, Hit &hit) {
			uint16_t *eventMapCS = &eventMapC[sliceIdx*SIGNALSIZE];
			uint16_t signalIdx1 = range&0xff;
			uint16_t signalIdx2 = (range>>8)&0xff;
			float Q, sumQ=0., sumW=0.;
			for(uint16_t iter=signalIdx1; iter<=signalIdx2; iter++) {
				Q = (float)eventMapCS[iter];
				sumQ += Q;
				sumW += Q*((float)iter+0.5); // weighted strip number
				//if(DEBUG) cout << hex << "event , ch: " << iter << ", charge: " << Q << endl;
			}
			hit.charge = sumQ;
			hit.nstrip = signalIdx2-signalIdx1+1;
			hit.center = sumW/(sumQ+0.000001); // for safty
		}


};

#endif
