#include "RunningInfAlg.h"
#include "DataSvc/DataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include "stdlib.h"
#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/Hit.h"
#include "Data/GPPDStatistic.h"


DECLARE_ALGORITHM(RunningInfAlg);

RunningInfAlg::RunningInfAlg(const std::string& name)
    : AlgBase(name)
{
}

RunningInfAlg::~RunningInfAlg()
{
}

	bool
RunningInfAlg::initialize()
{
	LogInfo << " initialized successfully" << std::endl;

	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) return false;
	m_svc = pSvc.data();

	return true;
}

	bool
RunningInfAlg::execute()
{
	NeutronPulse* pulse = m_svc->getObj<NeutronPulse>("/pulse");
	HitList* hitcol = m_svc->getObj<HitList>("/pulse/hits");
	EvtList* evtcol = m_svc->getObj<EvtList>("/pulse/evts");

	RunningInf* ri  = m_svc->getObj<RunningInf>("/statistic/running_inf");
        ri->addHitCnt(hitcol->size());
        ri->addEvtCnt(evtcol->size());
        ri->addPulseCnt(1);

	PixelCount* pc  = m_svc->getObj<PixelCount>("/statistic/pixel_count");
	for(uint32_t i = 0; i < evtcol->size(); i++){
		Evt* evt = evtcol->at(i);
		pc->addCount(evt->getPixelID(), 1);
	}


	return true;
}

	bool
RunningInfAlg::finalize()
{
	return true;
}
