#include "GPPDSNDMapAlg.h"
#include "DataSvc/GPPDDataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include <math.h>

DECLARE_ALGORITHM(GPPDSNDMapAlg);

GPPDSNDMapAlg::GPPDSNDMapAlg(const std::string& name)
    : AlgBase(name)
{
}

GPPDSNDMapAlg::~GPPDSNDMapAlg()
{
}

bool
GPPDSNDMapAlg::initialize()
{
    LogInfo << " initialized successfully" << std::endl;

    SniperPtr<DataSvc> pSvc("DataSvc");
    if ( pSvc.invalid()) {
      return false;
    }

    m_svc = pSvc.data();

    m_pulse = m_svc->getObj<NeutronPulse>("/pulse");
    m_evtcol = m_svc->getObj<EvtList>("/pulse/evts");

    return true;
}

bool GPPDSNDMapAlg::execute()
{

	uint32_t x, y, module;
	uint32_t bank, row, column;
	uint64_t pixelID;
	uint32_t size = m_evtcol->size();

	for(uint32_t i = 0; i < size; i++){
		Evt* evt = m_evtcol->at(i);

		x = evt->getX();
		y = evt->getY();
		module = m_pulse->getModule();
		getModInfo(module, bank, row, column);
		pixelID = getPixelID(bank, row, column, x, y);
		evt->setPixelID(pixelID);

	}

	return true;
}

bool GPPDSNDMapAlg::finalize()
{
	//EventList* evtcol = m_svc->getObj<EventList >("/pulse/event");
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//evtcol->clear();
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//LogInfo << " finalized successfully" << std::endl;

	return true;
}

void GPPDSNDMapAlg::getModInfo(uint32_t module, uint32_t& bank, uint32_t& row, uint32_t& column)
{
	bank   = 1;
	row    = 1;
	column = module;
}

uint64_t GPPDSNDMapAlg::getPixelID(uint32_t& bank, uint32_t& row, uint32_t& column, uint32_t& x, uint32_t& y)
{
	uint64_t ret_val = 0;
	ret_val =    bank*1000000;
	ret_val +=    row* 100000;
	ret_val += column*  10000;
	ret_val += (uint64_t(y/4)*111+uint64_t(x/4));	

	//std::cout << "bank: " << bank << " row: " << row << " column: " << column << " x: " << x << " y: " << y  << " return: " << ret_val << std::endl;
	return ret_val;
}
