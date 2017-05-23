#include "FileReadoutAlg.h"
#include "DataSvc/DataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/Hit.h"
#include "Data/GPPDStatistic.h"


#include <time.h>

DECLARE_ALGORITHM(FileReadoutAlg);

       
static int oldt0 = 0;
FileReadoutAlg::FileReadoutAlg(const std::string& name)
    : AlgBase(name)
{
	declProp("OutputFileName",m_outputfile);
	declProp("initXNum",m_initXNum);
	declProp("initYNum",m_initYNum);

	m_count = 0;
	m_evtnum = 0;
	m_hitnum = 0;
	m_xvect.clear();
	m_yvect.clear();
}

FileReadoutAlg::~FileReadoutAlg()
{
}

	bool
FileReadoutAlg::initialize()
{
	LogInfo << " initialized successfully" << std::endl;

	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) {
		return false;
	}

	m_svc = pSvc.data();
	std::cout << "get DataSvc " << m_svc->objName() << std::endl;

	m_start = clock();
        m_0pulseid = 0;
        m_1pulseid = 0;
        m_2pulseid = 0;
        m_0pulseCt = 0;
        m_1pulseCt = 0;
        m_2pulseCt = 0;
	return true;
}

	bool
FileReadoutAlg::execute()
{
	++m_count;
	//if((m_count%100000)==0) LogInfo << "Hello world: count: " << m_count << std::endl;

	NeutronPulse* pulse = m_svc->getObj<NeutronPulse>("/pulse");
	//LogInfo << "Instrument: " << pulse->getInstrument() << std::endl;
	//LogInfo << "Detector:   " << pulse->getDetector() << std::endl;
	//LogInfo << "Count:      " << pulse->getCount() << std::endl;
	m_ofstream0.open("0.log",ios::app);
	m_ofstream1.open("1.log",ios::app);
	m_ofstream2.open("2.log",ios::app);
        if(0==pulse->getModule()) {
             m_0pulseCt += pulse->getCount();
             m_ofstream0 <<pulse->getModule() <<  " " << pulse->getT0()  << " total Cnt: " << m_0pulseCt  << " getStatus: " << std::hex << pulse->getStatus() <<  std::endl;
             if(pulse->getT0() != (m_0pulseid + 1)) m_ofstream0 << "    Pulse ID ERROR" << std::endl;
             m_0pulseid = pulse->getT0();
        }
        if(1==pulse->getModule()) {
             m_1pulseCt += pulse->getCount();
             m_ofstream1 <<pulse->getModule() <<  " " << pulse->getT0()  << " total Cnt: " << m_1pulseCt << " getStatus: " << std::hex << pulse->getStatus() << std::endl;
             if(pulse->getT0() != (m_1pulseid + 1)) m_ofstream1 << "    Pulse ID ERROR" << std::endl;
             m_1pulseid = pulse->getT0();
        }
        if(2==pulse->getModule()) {
             m_2pulseCt += pulse->getCount();
             m_ofstream2 <<pulse->getModule() <<  " " << pulse->getT0()  << " total Cnt: " << m_2pulseCt  << " getStatus: " << std::hex << pulse->getStatus() << std::endl;
             if(pulse->getT0() != (m_2pulseid + 1)) m_ofstream2 << "    Pulse ID ERROR" << std::endl;
             m_2pulseid = pulse->getT0();
        }
	m_ofstream0.close();
	m_ofstream1.close();
	m_ofstream2.close();
/*
	HitList* hitcol = m_svc->getObj<HitList>("/pulse/hits");
	//std::cout << hitcol->size() << " hits are collected!" << std::endl;
	m_hitnum += hitcol->size();
	//for(uint32_t i = 0; i < hitcol->size(); i++){
	//  Hit* hit = hitcol->at(i);
	//  std::cout << "iterator " << std::dec << i << " channel " << hit->getChannel() << " tof " << hit->getTOF() << std::endl;
	//}

	EvtList* evtcol = m_svc->getObj<EvtList>("/pulse/evts");
	//std::cout << evtcol->size() << " evts are collected!" << std::endl;
	m_evtnum += evtcol->size();

	PixelCount* pc  = m_svc->getObj<PixelCount>("/statistic/pixel_count");
	for(uint32_t i = 0; i < evtcol->size(); i++){
		Evt* evt = evtcol->at(i);
		//std::cout << std::dec << evt->getX() << ", " << evt->getY() << ",  " << evt->getTOF() << "," << evt->getPixelID() << std::endl;
		pc->addCount(evt->getPixelID(), 1);
	}

	//LogInfo << "Pulse T0:  " << pulse->getT0()  << ". " << hitcol->size() << " hits are collected, " << evtcol->size() << " evts are reconstructed in this pulse"<< std::endl;



	if((m_count%40)==0){
		m_ofstream.open(m_outputfile.c_str());

	time_t now;
	time(&now);
	m_ofstream << now << " " << m_count << " " << m_evtnum << " " << m_hitnum << std::endl;
	KEYS& pids = pc->getPixelIDs();
	for(KEYS::iterator it = pids.begin(); it < pids.end(); it++){
		std::cout <<  *it  << " " << pc->getCount(*it) << std::endl;
	}
	for(int i = 0; i <m_initXNum*m_initYNum; i++){
		 m_ofstream <<  i  << " " << pc->getCount(i+1100000) << std::endl;
	}
	for(int i = 0; i <m_initXNum*m_initYNum; i++){
		 m_ofstream <<  i  << " " << pc->getCount(i+1110000) << std::endl;
	}
	for(int i = 0; i <m_initXNum*m_initYNum; i++){
		 m_ofstream <<  i  << " " << pc->getCount(i+1120000) << std::endl;
	}

		m_ofstream.close();

	}

*/
	return true;
}

	bool
FileReadoutAlg::finalize()
{
	//EventList* evtcol = m_svc->getObj<EventList >("/pulse/event");
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//evtcol->clear();
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//LogInfo << " finalized successfully" << std::endl;

	PixelCount* pc  = m_svc->getObj<PixelCount>("/statistic/pixel_count");
	//m_ofstream.open(m_outputfile.c_str());
	//time_t now;
	//time(&now);
	//m_ofstream << now << " " << m_count << " " << m_evtnum << " " << m_hitnum << std::endl;
	//KEYS& pids = pc->getPixelIDs();
	//for(int i = 0; i <111*48; i++){
	//	 m_ofstream <<  i  << " " << pc->getCount(i+1100000) << std::endl;
	//}
	//for(int i = 111*48; i <111*48*2; i++){
	//	 m_ofstream <<  i  << " " << pc->getCount(i+1110000) << std::endl;
	//}
	//for(int i = 111*48*2; i <111*48*3; i++){
	//	 m_ofstream <<  i  << " " << pc->getCount(i+1120000) << std::endl;
	//}

//	for(KEYS::iterator it = pids.begin(); it < pids.end(); it++){
//		m_ofstream <<  *it  << " " << pc->getCount(*it) << std::endl;
//	}
	//m_ofstream.close();

	//m_finish = clock();
	//clock_t duration = (m_finish-m_start)/CLOCKS_PER_SEC;
	LogInfo << m_count  << " pulses are collected" << std::endl;
	//LogInfo << m_hitnum << " hits   are collected" << std::endl;
	//LogInfo << m_evtnum << " evts   are collected" << std::endl;
	//LogInfo << duration << " secs are used!" << std::endl;
	return true;
}
