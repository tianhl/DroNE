#ifndef GPPDSNDMapAlg_h
#define GPPDSNDMapAlg_h

#include "SniperKernel/AlgBase.h"

#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/Hit.h"

class DataSvc;

class GPPDSNDMapAlg: public AlgBase
{
    public:
        GPPDSNDMapAlg(const std::string& name);

        ~GPPDSNDMapAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
	EvtList* m_evtcol;
	NeutronPulse* m_pulse;
	DataSvc* m_svc;
    private:
	void getModInfo(uint32_t module, uint32_t& bank, uint32_t& row, uint32_t& column);
	uint64_t getPixelID(uint32_t& bank, uint32_t& row, uint32_t& column, uint32_t& x, uint32_t& y);

};

#endif