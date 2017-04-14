#ifndef SaveNeXusAlg_h
#define SaveNeXusAlg_h

#include "SniperKernel/AlgBase.h"

#include "Data/Pulse.h"
#include "Data/Evt.h"

class DataSvc;

class SaveNeXusAlg: public AlgBase
{
    public:
        SaveNeXusAlg(const std::string& name);

        ~SaveNeXusAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
	EvtList* m_evtcol;
	NeutronPulse* m_pulse;
	DataSvc* m_svc;
	std::vector<uint32_t> m_nexusEvt;
};

#endif
