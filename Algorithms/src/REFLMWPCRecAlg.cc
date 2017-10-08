// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  REFLMWPCRecAlg.cc
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//
#include "REFLMWPCRecAlg.h"
#include "DroNECore/DataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include <math.h>

DECLARE_ALGORITHM(REFLMWPCRecAlg);

REFLMWPCRecAlg::REFLMWPCRecAlg(const std::string& name)
    : AlgBase(name)
{
    m_count = 0;
}

REFLMWPCRecAlg::~REFLMWPCRecAlg()
{
}

bool
REFLMWPCRecAlg::initialize()
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
    m_evtcol = m_svc->getObj<EvtList>("/pulse/evts");

    return true;
}

bool
REFLMWPCRecAlg::execute()
{
    ++m_count;
    if((m_count%100000)==0) LogInfo << "Hello world: count: " << m_count << std::endl;


    int time=0;
    int timeleap=0;  
    uint32_t size = m_hitcol->size();

    for(uint32_t i = 0; i < size; i++){
	    MWPCHit* hit = m_hitcol->at(i);
	    if(0==i){
		    m_buffer.clear();
		    m_charge.clear();
		    m_buffer.push_back(hit->getChannel());
		    m_charge.push_back(hit->getCharge());
		    time = hit->getTOF();
	    }
	    else{
		    timeleap = time - hit->getTOF();
		    timeleap = abs(timeleap);
		    if(80>timeleap){
			    m_buffer.push_back(hit->getChannel());
			    m_charge.push_back(hit->getCharge());
		    }
		    else{
			    reconstruct(time);
			    m_buffer.clear();
			    m_charge.clear();
			    m_buffer.push_back(hit->getChannel());
			    m_charge.push_back(hit->getCharge());
		    }
		    time = hit->getTOF();
	    }
    }
    // the last hit
    if(0<m_buffer.size()){
	    reconstruct(time);
    }

    m_buffer.clear();
    m_charge.clear();

    //    LogInfo << "Num of Pulse: " << m_count << " , includeing " 
    //	    << m_hitcol->size() << " hits and " << m_evtcol->size() << " evts reconstruced" << std::endl;
    return true;
}

void REFLMWPCRecAlg::reconstruct(int time){
	int X=0;
	int Y=0;
	int effect=0;

	calculation(X, Y, effect);
	//m_buffer.clear();
	//m_buffer.push_back(hit->getChannel());
	if(1 ==  effect){
		Evt* evt = m_evtcol->add_item();
		//std::cout << "======= add evt" << std::endl;
		evt->setX(X);
		evt->setY(Y);
		//evt->setPixleID(int); waiting for MappingAlg
		evt->setTOF(time);
	}
}

bool REFLMWPCRecAlg::finalize()
{
	//EventList* evtcol = m_svc->getObj<EventList >("/pulse/event");
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//evtcol->clear();
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//LogInfo << " finalized successfully" << std::endl;

	return true;
}


int REFLMWPCRecAlg::calculation(int &X,int &Y,int &Effect)
{
	//std::cout << "calculation" << std::endl;
	int Xposition[10];
	int Yposition[10];
	int Xcharge[10];
	int Ycharge[10];
	int Xposit=0;
	int Yposit=0;
	int Xreal=0;
	int Yreal=0;
	int m,n;
	m=n=0;
	Effect=0;

	int Size=m_buffer.size();

	if (Size<2||Size>20)  { Effect=0;return 0;}  //Ò»¸öÖÐ×ÓÊÂÀýÖÐÖ»ÓÐÒ»¸öhit»ò20¸÷ÒÔÉÏµÄhitÊý£¬ÎÞÐ§
	for(int i=0;i<Size;i++)	                     //°ÑÖÐ×ÓÊÂÀýÖÐ¶à¸öhitÐÅÏ¢·Ö±ð¸³Öµ¸øXºÍYµÄÊý×é
	{
		if (m_buffer[i] <100)  
		{  
			Xposition[m]=m_buffer[i];           
                        Xcharge[m]=m_charge[i];
			m++; 
			if (m>10)   { Effect=0;return 0;}  //µ¥¸öÖÐ×ÓÊÂÀý XÖá10¸öhitÊý£¬ÎÞÐ§
		}
		else 
		{  
			Yposition[n]=m_buffer[i]-100;           
                        Ycharge[n]  =m_charge[i];
			n++;	
			if (n>10)   { Effect=0;return 0;} 
		}
	}
	if ((m==0) || (n==0))  { Effect=0;return 0;}  //Ò»¸öÖÐ×ÓÊÂÀýÖÐÖ»ÓÐÒ»¸öÖáÉÏÓÐhitÊý£¬ÎÞÐ§
	scan(Xposition,Xcharge,m,Xposit,Xreal);                //¼ÆËãÖÐ×ÓÊÂÀýÖÐXÖáÉÏ¶à¸öhitµÃµ½µÄÊµ¼ÊÎ»ÖÃXposit£¬Èô¶à¸öhit²»ÏàÁÚ£¬Ôò·µ»ØXreal=0
	scan(Yposition,Ycharge,n,Yposit,Yreal);
	if ((Xreal==1)&&(Yreal==1))
	{
		X=Xposit;
		Y=Yposit;        
		Effect=1;
	}
	return 1;	
}

int REFLMWPCRecAlg::scan(int *Pos,int *Cha, int l,int &ThePosition,int &Real){
	//std::cout << "scan" << std::endl;
	Real=0;
	if(l<1||l>10){
		Real=0; return 0;
	}

	int Psum=0;
        int Qsum=0;
	for(int i=0;i<l;i++)
	{		
		Psum=Psum+Pos[i]*Cha[i];		  //¼ÆËãËùÓÐhitÊýµÄ¼¸ºÎÖÐÐÄ
		Qsum=Qsum+Cha[i];		  //¼ÆËãËùÓÐhitÊýµÄ¼¸ºÎÖÐÐÄ
	}

	ThePosition=Psum/Qsum;
	Real=1;
	return 1;	
}
