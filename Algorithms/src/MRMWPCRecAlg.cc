// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN & Haiyun Teng 2018
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2018 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  MRMWPCRecAlg.cc
//  
//
//  Created by Haolai TIAN & Haiyun Teng 10st July 2018.
//
//
#include "MRMWPCRecAlg.h"
#include "DroNECore/DataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include <math.h>

DECLARE_ALGORITHM(MRMWPCRecAlg);

	MRMWPCRecAlg::MRMWPCRecAlg(const std::string& name)
: AlgBase(name)
{
	declProp("QMin", QMIN);
	declProp("QMax", QMAX);
	declProp("MATCHWINDOW", MATCHWINDOW);
	declProp("QSUM", QSUM);

	m_count = 0;

	signalNum = 0;
	totalEvent = 0;
	validEvent = 0;

	QMIN = 10;
	QMAX = 50;
	MATCHWINDOW = 40;
	QSUM = 1.;

	eventMapT = new uint32_t[SLICESIZE*SIGNALSIZE];
	eventMapC = new uint16_t[SLICESIZE*SIGNALSIZE];
	eventMapSlice = new uint16_t[SLICESIZE];
	eventMapMin = new uint16_t[SLICESIZE];
	eventMapMax = new uint16_t[SLICESIZE];
	sBuffer = new uint16_t[SIGNALSIZE];
	signalMapXT = new uint32_t[SLICESIZE*XCHANNELNUM];
	signalMapYT = new uint32_t[SLICESIZE*YCHANNELNUM];
	signalMapXM = new uint16_t[SLICESIZE*XCHANNELNUM];
	signalMapYM = new uint16_t[SLICESIZE*YCHANNELNUM];
	signalMapXS = new uint16_t[SLICESIZE*XCHANNELNUM];
	signalMapYS = new uint16_t[SLICESIZE*YCHANNELNUM];
	signalMapXR = new uint16_t[SLICESIZE*XCHANNELNUM];
	signalMapYR = new uint16_t[SLICESIZE*YCHANNELNUM];
	signalMapXIdx = new uint32_t[SLICESIZE];
	signalMapYIdx = new uint32_t[SLICESIZE];
	matchX = new uint32_t[SLICESIZE*XCHANNELNUM];
	matchY = new uint32_t[SLICESIZE*XCHANNELNUM];
}

MRMWPCRecAlg::~MRMWPCRecAlg()
{
	delete [] eventMapT;
	delete [] eventMapC;
	delete [] eventMapSlice;
	delete [] eventMapMin;
	delete [] eventMapMax;
	delete [] sBuffer;
	delete [] signalMapXT;
	delete [] signalMapYT;
	delete [] signalMapXM;
	delete [] signalMapYM;
	delete [] signalMapXS;
	delete [] signalMapYS;
	delete [] signalMapXR;
	delete [] signalMapYR;
	delete [] signalMapXIdx;
	delete [] signalMapYIdx;
	delete [] matchX;
	delete [] matchY;
}

	bool
MRMWPCRecAlg::initialize()
{
	LogInfo << " initialized successfully" << std::endl;

	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) {
		return false;
	}

	m_svc = pSvc.data();
	std::cout << "get DataSvc " << m_svc->objName() << std::endl;

	m_pulse = m_svc->getObj<NeutronPulse>("/pulse");
	m_hitcol = m_svc->getObj<MWPCHitList>("/pulse/hits");
	m_evtcol = m_svc->getObj<EvtDList>("/pulse/evts");

	return true;
}

	bool
MRMWPCRecAlg::execute()
{
	++m_count;
	uint32_t size = m_hitcol->size();
        uint32_t ch[size];
        uint32_t Q[size];
        uint32_t base[size];
        uint32_t tof[size];
        uint32_t sliceId[size];
        std::cout << "============================ hit col size: " << size << std::endl;
	for(uint32_t i = 0; i < size; i++){
		MWPCHit* hit = m_hitcol->at(i);
                ch[i]      = hit->getChannel();
                Q[i]       = hit->getCharge();
                base[i]    = hit->getBaseline();
                tof[i]     = hit->getTOF();
                sliceId[i] = hit->getTDataID();
	}

	memset(eventMapT, 0, SLICESIZE*SIGNALSIZE*sizeof(uint32_t));
	memset(eventMapC, 0, SLICESIZE*SIGNALSIZE*sizeof(uint16_t));
	//eventMapSlice.clear();
	//eventMapMin.clear();
	//eventMapMax.clear();
	eventMapIdx = 0;
	//matchX.clear();
	//matchY.clear();
	memset(signalMapXM, 0, SLICESIZE*sizeof(uint16_t));
	memset(signalMapYM, 0, SLICESIZE*sizeof(uint16_t));
	memset(signalMapXIdx, 0, SLICESIZE*sizeof(uint16_t));
	memset(signalMapYIdx, 0, SLICESIZE*sizeof(uint16_t));
	matchIdx = 0;

	scanSlice(ch, Q, base, tof, sliceId, size);
	processMap();
	matchNeutron();


	//    LogInfo << "Num of Pulse: " << m_count << " , includeing " 
	//	    << m_hitcol->size() << " hits and " << m_evtcol->size() << " evts reconstruced" << std::endl;
	return true;
}

bool MRMWPCRecAlg::finalize()
{
	//EventList* evtcol = m_svc->getObj<EventList >("/pulse/event");
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//evtcol->clear();
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//LogInfo << " finalized successfully" << std::endl;

	return true;
}

void MRMWPCRecAlg::scanSlice(unsigned int *ch, unsigned int *Q, unsigned int *base, unsigned int *tof, unsigned int *sliceId, const int& size) {
    uint32_t *eventMapTS;
    uint16_t *eventMapCS;
    //uint32_t tof, value;
    uint32_t value;
    //uint16_t channel, base, Q, bareQ, minChannel=SIGNALSIZE, maxChannel=0;
    uint16_t channel,  bareQ, minChannel=SIGNALSIZE, maxChannel=0;
    int sliceIdx=0;
    eventMapTS = &eventMapT[0];
    eventMapCS = &eventMapC[0];
    validEvent = 0;
    slice_eventNumber=0;
    eventMapIdx = 0;
    for(int idx=0; idx<size; idx++) {

        if(sliceIdx!=sliceId[idx]) {
            if(slice_eventNumber!=0) {
                validEvent += slice_eventNumber;
                eventMapSlice[eventMapIdx] = sliceIdx;
                eventMapMin[eventMapIdx] = minChannel;
                eventMapMax[eventMapIdx] = maxChannel;
                eventMapIdx++;
            }

            slice_eventNumber = 0;
            sliceIdx = sliceId[idx];
            eventMapTS = &eventMapT[SIGNALSIZE*sliceIdx];
            eventMapCS = &eventMapC[SIGNALSIZE*sliceIdx];
            minChannel = SIGNALSIZE;
            maxChannel = 0;
        }

        channel = ch[idx];
        bareQ = Q[idx]-(base[idx]>>3);
        if(bareQ>=QMIN) {
            eventMapTS[channel] = tof[idx];
            eventMapCS[channel] = bareQ;
            slice_eventNumber++;
            if(channel<minChannel)
                minChannel = channel;
            if(channel>maxChannel)
                maxChannel = channel;
        }
    }

    if(slice_eventNumber!=0) {
        validEvent += slice_eventNumber;
        eventMapSlice[eventMapIdx] = sliceIdx;
        eventMapMin[eventMapIdx] = minChannel;
        eventMapMax[eventMapIdx] = maxChannel;
        eventMapIdx++;
    }

}


void MRMWPCRecAlg::processMap() {
    int sliceNumber = eventMapIdx;
    //if(DEBUG) cout << hex << "slice total number: " << sliceNumber << endl; 

    if(sliceNumber==0) {
        //pre_tDataId = 0;
        //cur_tDataId = 0;
        return;
    }
    sliceNumber--; // fast calc

    uint32_t *eventMapTS1, *eventMapTS2;
    uint16_t *eventMapCS1, *eventMapCS2;
    int sliceIdx1, sliceIdx2, pre_SigIdx, cur_SigIdx, dCH, signalIdx1, signalIdx2, center;
    uint32_t pre_TOF=0, cur_TOF=0, time, sxIdx=0, syIdx=0;
    uint16_t cQ, cross1, cross2;
    int dTOF;
    for(int idx=0; idx<sliceNumber; idx++) {
        sliceIdx1 = eventMapSlice[idx];
        sliceIdx2 = sliceIdx1+1;
        //if(DEBUG) cout << hex << "process slice " << sliceIdx1 << endl;
        eventMapTS1 = &eventMapT[sliceIdx1*SIGNALSIZE];
        eventMapTS2 = &eventMapT[sliceIdx2*SIGNALSIZE];
        eventMapCS1 = &eventMapC[sliceIdx1*SIGNALSIZE];
        eventMapCS2 = &eventMapC[sliceIdx2*SIGNALSIZE];

        pre_SigIdx=-1;
        cur_SigIdx=-1;
        signalIdx1=-1;
        signalIdx2=-1;
        for(uint16_t cIdx=eventMapMin[idx]; cIdx<=eventMapMax[idx]; cIdx++) {
            cQ = (eventMapCS1[cIdx]|eventMapCS2[cIdx]);
            if(cQ>QMAX) {
                pre_TOF = cur_TOF;
                cur_TOF = (eventMapTS1[cIdx]|eventMapTS2[cIdx]);
                pre_SigIdx = cur_SigIdx;
                cur_SigIdx = cIdx;
                //if(DEBUG) cout <<hex << "find one strip(>QMAX) " << cIdx << ", tof: " << cur_TOF << ", Q: " << cQ << ", corss: " << cross1 << "|" << cross2 << endl;
                if(signalIdx1==-1) {
                    signalIdx1 = cur_SigIdx;
                    signalIdx2 = cur_SigIdx;
                    cross1 = eventMapCS1[cur_SigIdx];
                    cross2 = eventMapCS2[cur_SigIdx];
                    sBuffer[0] = cQ;
                    sBufferIdx = 1;
                }
            
                if(pre_SigIdx>0) {
                    dTOF = abs((int)cur_TOF-(int)pre_TOF);
                    dCH = cur_SigIdx-pre_SigIdx;
                
                    if(dTOF<=MATCHWINDOW && dCH==1 && (signalIdx1>=XCHANNELNUM || cur_SigIdx<XCHANNELNUM)) {
                        signalIdx2 = cur_SigIdx;
                        cross1 |= eventMapCS1[cur_SigIdx];
                        cross2 |= eventMapCS2[cur_SigIdx];
                        sBuffer[sBufferIdx] = cQ;
                        sBufferIdx++;
                    }
                    else {
                        if(cross1>0) {
                            //if(DEBUG) cout << "find signal " << signalIdx1 << ", " << signalIdx2 << endl;
                            if(cross2!=0) {
                                memcpy(&eventMapCS1[signalIdx1], sBuffer, sBufferIdx*sizeof(uint16_t));
                                memset(&eventMapCS2[signalIdx1], 0, sBufferIdx*sizeof(uint16_t));
                            }
                            //processSignal(sliceIdx1, signalIdx1, signalIdx2); // [signalIdx1, signalIdx2], record on next adjon slice will be swap
                            {
                                center = (signalIdx1+signalIdx2)>>1;
                                time = eventMapTS1[center]|eventMapTS2[center];
                                if(center<XCHANNELNUM) {
                                    signalMapXT[sxIdx] = time;
                                    signalMapXS[sxIdx] = sliceIdx1;
                                    signalMapXR[sxIdx] = uint16_t((signalIdx2<<8)|signalIdx1);
                                    sxIdx++;
                                }
                                else {
                                    signalMapYT[syIdx] = time;
                                    signalMapYS[syIdx] = sliceIdx1;
                                    signalMapYR[syIdx] = uint16_t((signalIdx2<<8)|signalIdx1);
                                    syIdx++;
                                }
                            }
                            signalNum++;
                        }
                        signalIdx1 = cur_SigIdx;
                        signalIdx2 = cur_SigIdx;
                        cross1 = eventMapCS1[cur_SigIdx];
                        cross2 = eventMapCS2[cur_SigIdx];
                        sBuffer[0] = cQ;
                        sBufferIdx = 1;
                    }
                }
            }
        }
        // last signal
        if(signalIdx1!=-1) {
            //if(DEBUG) cout << "find signal " << signalIdx1 << ", " << signalIdx2 << endl;
            if(cross1>0) {
                if(cross2!=0) {
                    memcpy(&eventMapCS1[signalIdx1], sBuffer, sBufferIdx*sizeof(uint16_t));
                    memset(&eventMapCS2[signalIdx1], 0, sBufferIdx*sizeof(uint16_t));
                }
                //processSignal(sliceIdx1, signalIdx1, signalIdx2); // [signalIdx1, signalIdx2]
                {
                    center = (signalIdx1+signalIdx2)>>1;
                    time = eventMapTS1[center]|eventMapTS2[center];
                    if(center<XCHANNELNUM) {
                        signalMapXT[sxIdx] = time;
                        signalMapXS[sxIdx] = sliceIdx1;
                        signalMapXR[sxIdx] = uint16_t((signalIdx2<<8)|signalIdx1);
                        sxIdx++;
                    }
                    else {
                        signalMapYT[syIdx] = time;
                        signalMapYS[syIdx] = sliceIdx1;
                        signalMapYR[syIdx] = uint16_t((signalIdx2<<8)|signalIdx1);
                        syIdx++;
                    }
                }
                signalNum++;
            }
        }
        signalMapXIdx[idx] = sxIdx;
        signalMapYIdx[idx] = syIdx;
        //if(DEBUG&&(idx>1)) cout << dec << "slice " << sliceIdx1 << " accumulate signalX " << sxIdx-signalMapXIdx[idx-1] << ", signalY " << syIdx-signalMapYIdx[idx-1] << endl;
    }

    // last slice
    pre_SigIdx = -1;
    cur_SigIdx = -1;
    signalIdx1 = -1;
    signalIdx2 = -1;
    sliceIdx1 = eventMapSlice[sliceNumber];
    //if(DEBUG) cout << hex << "process list slice " << sliceIdx1 << endl;
    eventMapCS1 = &eventMapC[sliceIdx1*SIGNALSIZE];
    eventMapTS1 = &eventMapT[sliceIdx1*SIGNALSIZE];
    for(int cIdx=eventMapMin[sliceNumber]; cIdx<=eventMapMax[sliceNumber]; cIdx++) {
        cQ = eventMapCS1[cIdx];
        if(cQ>QMAX) {
            pre_TOF = cur_TOF;
            cur_TOF = eventMapTS1[cIdx];
            pre_SigIdx = cur_SigIdx;
            cur_SigIdx = cIdx;
            if(signalIdx1==-1) {
                signalIdx1 = cur_SigIdx;
                signalIdx2 = cur_SigIdx;
            }

            if(pre_SigIdx>=0) {
                dTOF = cur_TOF - pre_TOF;
                dCH = cur_SigIdx - pre_SigIdx;

                if(dTOF<=MATCHWINDOW && dCH==1 && (signalIdx1>=XCHANNELNUM || cur_SigIdx<XCHANNELNUM)) {
                    signalIdx2 = cur_SigIdx;
                }
                else {
                   // if(DEBUG) cout << "find signal " << signalIdx1 << ", " << signalIdx2 << endl;
                    //processSignal(sliceIdx1, signalIdx1, signalIdx2); // [idx1, idx2]
                    {
                        center = (signalIdx1+signalIdx2)>>1;
                        time = eventMapTS1[center];
                        if(center<XCHANNELNUM) {
                            signalMapXT[sxIdx] = time;
                            signalMapXS[sxIdx] = sliceIdx1;
                            signalMapXR[sxIdx] = uint16_t((signalIdx2<<8)|signalIdx1);
                            sxIdx++;
                        }
                        else {
                            signalMapYT[syIdx] = time;
                            signalMapYS[syIdx] = sliceIdx1;
                            signalMapYR[syIdx] = uint16_t((signalIdx2<<8)|signalIdx1);
                            syIdx++;
                        }
                    }
                    signalNum++;
                    signalIdx1 = cur_SigIdx;
                    signalIdx2 = cur_SigIdx;
                }
            }
        }
    }
    // last signal
    if(signalIdx1!=-1) {
      //  if(DEBUG) cout << "find signal " << signalIdx1 << ", " << signalIdx2 << endl;
        //processSignal(sliceIdx1, signalIdx1, signalIdx2); // [signalIdx1, signalIdx2]
        {
            center = (signalIdx1+signalIdx2)>>1;
            time = eventMapTS1[center];
            if(center<XCHANNELNUM) {
                signalMapXT[sxIdx] = time;
                signalMapXS[sxIdx] = sliceIdx1;
                signalMapXR[sxIdx] = uint16_t((signalIdx2<<8)|signalIdx1);
                sxIdx++;
            }
            else {
                signalMapYT[syIdx] = time;
                signalMapYS[syIdx] = sliceIdx1;
                signalMapYR[syIdx] = uint16_t((signalIdx2<<8)|signalIdx1);
                syIdx++;
            }
        }
        signalNum++;
    }
    signalMapXIdx[sliceNumber] = sxIdx;
    signalMapYIdx[sliceNumber] = syIdx;
   // if(DEBUG) cout << dec << "last slice " << sliceIdx1 << " accumulate signalX " << sxIdx-signalMapXIdx[sliceNumber-1] << ", signalY " << syIdx-signalMapYIdx[sliceNumber-1] << endl;

   // if(DEBUG) cout << "processMap end." << endl;
}

void MRMWPCRecAlg::matchNeutron() {
    int tx, ty, sliceIdx, sliceNumber=eventMapIdx;
    uint32_t xStart=0, xEnd=0, yStart=0, yEnd=0;
    int i, j, m;
    matchIdx = 0;
    for(int idx=0; idx<eventMapIdx; idx++) {
        sliceIdx = eventMapSlice[idx];
        xStart = xEnd;
        xEnd = signalMapXIdx[idx];
        
        yStart = (idx>=1) ? signalMapYIdx[idx-1] : 0;
        if(idx>=1 && ((sliceIdx-eventMapSlice[idx-1]-1)<<5)<=MATCHWINDOW) {
            yStart = (idx>=2) ? signalMapYIdx[idx-2] : 0;
            if(idx>=2 && ((sliceIdx-eventMapSlice[idx-2]-1)<<5)<=MATCHWINDOW) {
                yStart = (idx>=3) ? signalMapYIdx[idx-3] : 0;
            }
        }
        yEnd = signalMapYIdx[idx];
        if((idx+1)<sliceNumber && ((eventMapSlice[idx+1]-sliceNumber-1)<<5)<=MATCHWINDOW) {
            yEnd = signalMapYIdx[idx+1];
            if((idx+2)<sliceNumber && ((eventMapSlice[idx+2]-sliceNumber-1)<<5)<=MATCHWINDOW)
                yEnd = signalMapYIdx[idx+2];
        }

        for(i=xStart; i<xEnd; i++) {
            tx = signalMapXT[i];
            for(j=yStart; j<yEnd; j++) {
                ty = signalMapYT[j];
                if(abs(tx-ty)<=MATCHWINDOW) {
                    signalMapXM[i]++;
                    signalMapYM[j]++;
                    m = j;
                }
            }
            //if(DEBUG) cout << dec << "scan match X-" << i << " tof " << tx << "to Ys " << yStart << ", " << yEnd << ", ms " << signalMapXM[i] << endl;
            if(signalMapXM[i]==1) {
                matchX[matchIdx] = i;
                matchY[matchIdx] = m;
                matchIdx++;
                //if(DEBUG) cout << "pre-match X-" << i << " tof " << tx << ", Y-" << m << " tof " << signalMapYT[m] << endl;
            }
        }
    }

    uint32_t xIdx, yIdx;
    uint16_t sliceX, sliceY, rangeX, rangeY;
    for(int mIdx=0; mIdx<matchIdx; mIdx++) {
        xIdx = matchX[mIdx];
        yIdx = matchY[mIdx];
        //if(DEBUG) cout << "checking Y tof " << signalMapYT[yIdx] << " match number " << signalMapYM[yIdx] << endl;
        if(signalMapYM[yIdx]==1) {
            hitX.time = signalMapXT[xIdx];
            hitY.time = signalMapYT[yIdx];
            sliceX = signalMapXS[xIdx];
            sliceY = signalMapYS[yIdx];
            rangeX = signalMapXR[xIdx];
            rangeY = signalMapYR[yIdx];
            getHit(sliceX, rangeX, hitX);
            getHit(sliceY, rangeY, hitY);
            //if(DEBUG) cout << dec << "neutron X range: " << ((rangeX>>8)&0xff) << ", " << (rangeX&0xff) << ", slice: " << sliceX << ", center: " << hitX.center << ", Y range: " << ((rangeY>>8)&0xff) << ", " << (rangeY&0xff) << ", slice: " << sliceY << ", center: " << hitY.center << endl;
            
            //if(DEBUG) cout << hex << "find neutron: " << neutron.x << ", " << neutron.y << ", " << neutron.t << endl;
		EvtD* evt = m_evtcol->add_item();
		evt->setX(0.0);
		evt->setY(0.0);
		evt->setTOF(0.0);
            

        }
    }
}
