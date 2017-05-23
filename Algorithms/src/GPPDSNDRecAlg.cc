#include "GPPDSNDRecAlg.h"
#include "DataSvc/GPPDDataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include <math.h>

DECLARE_ALGORITHM(GPPDSNDRecAlg);

GPPDSNDRecAlg::GPPDSNDRecAlg(const std::string& name)
    : AlgBase(name)
{
    m_count = 0;
}

GPPDSNDRecAlg::~GPPDSNDRecAlg()
{
}

bool
GPPDSNDRecAlg::initialize()
{
    LogInfo << " initialized successfully" << std::endl;

    SniperPtr<DataSvc> pSvc("DataSvc");
    if ( pSvc.invalid()) {
      return false;
    }

    m_svc = pSvc.data();
    std::cout << "get DataSvc " << m_svc->objName() << std::endl;

    m_pulse = m_svc->getObj<NeutronPulse>("/pulse");
    m_hitcol = m_svc->getObj<HitList>("/pulse/hits");
    m_evtcol = m_svc->getObj<EvtList>("/pulse/evts");

    return true;
}

bool
GPPDSNDRecAlg::execute()
{
    ++m_count;
    if((m_count%100000)==0) LogInfo << "Hello world: count: " << m_count << std::endl;


    // tiangb@ihep.ac.cn
    int time=0;
    int timeleap=0;  
    uint32_t size = m_hitcol->size();

    for(uint32_t i = 0; i < size; i++){
	    Hit* hit = m_hitcol->at(i);
	    if(0==i){
		    m_buffer.clear();
		    m_buffer.push_back(hit->getChannel());
		    time = hit->getTOF();
	    }
	    else{
		    timeleap = time - hit->getTOF();
		    timeleap = abs(timeleap);
		    if(80>timeleap){
			    m_buffer.push_back(hit->getChannel());
		    }
		    else{
			    reconstruct(time);
			    m_buffer.clear();
			    m_buffer.push_back(hit->getChannel());
		    }
		    time = hit->getTOF();
	    }
    }
    // the last hit
    if(0<m_buffer.size()){
	    reconstruct(time);
    }

    m_buffer.clear();

    return true;
}

void GPPDSNDRecAlg::reconstruct(int time){
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

bool GPPDSNDRecAlg::finalize()
{
	//EventList* evtcol = m_svc->getObj<EventList >("/pulse/event");
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//evtcol->clear();
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//LogInfo << " finalized successfully" << std::endl;

	return true;
}


int GPPDSNDRecAlg::calculation(int &X,int &Y,int &Effect)
{
	//std::cout << "calculation" << std::endl;
	int Xposition[10];
	int Yposition[10];
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
		if (m_buffer[i] <128)  
		{  
			Xposition[m]=m_buffer[i];           
			m++; 
			if (m>10)   { Effect=0;return 0;}  //µ¥¸öÖÐ×ÓÊÂÀý XÖá10¸öhitÊý£¬ÎÞÐ§
		}
		else 
		{  
			Yposition[n]=m_buffer[i]-128;           
			n++;	
			if (n>10)   { Effect=0;return 0;} 
		}
	}
	if ((m==0) || (n==0))  { Effect=0;return 0;}  //Ò»¸öÖÐ×ÓÊÂÀýÖÐÖ»ÓÐÒ»¸öÖáÉÏÓÐhitÊý£¬ÎÞÐ§
	scan(Xposition,m,Xposit,Xreal);                //¼ÆËãÖÐ×ÓÊÂÀýÖÐXÖáÉÏ¶à¸öhitµÃµ½µÄÊµ¼ÊÎ»ÖÃXposit£¬Èô¶à¸öhit²»ÏàÁÚ£¬Ôò·µ»ØXreal=0
	scan(Yposition,n,Yposit,Yreal);
	if ((Xreal==1)&&(Yreal==1))
	{
		X=Xposit;
		Y=Yposit;        
		Effect=1;
	}
	return 1;	
}

int GPPDSNDRecAlg::scan(int *Pos,int l,int &ThePosition,int &Real){
	//std::cout << "scan" << std::endl;
	Real=0;
        if(l<1||l>10){
          Real=0; return 0;
         }

        sort(Pos,l);
	int Now=Pos[0];
	for(int i=1;i<l;i++)	
	{
		if ((abs(Pos[i]-Now))>1)  {Real=0;return 0;}  //hitÊý²»ÏàÁÚ£¬Ôò·µ»ØRreal=0
		Now=Pos[i];
	}

	int sum=0;
	for(int i=0;i<l;i++)
	{		
		sum=sum+Pos[i];		  //¼ÆËãËùÓÐhitÊýµÄ¼¸ºÎÖÐÐÄ
	}

	ThePosition=sum*4/l;
	Real=1;
	return 1;	
}

int GPPDSNDRecAlg::sort(int *src, int len){
	//std::cout << "sort" << std::endl;
	int tem;
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < len - i - 1; j++) // Ã¿Ñ­»·Ò»´Î£¬ÔòÉÙÒ»´Î
			if (src[j] > src[j+1])  // Èç¹ûÇ°Ò»¸öÊý¾Ý±ÈºóÒ»¸ö´ó£¬Ôò½»»»
			{
				tem = src[j];
				src[j] = src[j+1];
				src[j+1] = tem;
			}
	}
	return 1;
}
