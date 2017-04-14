#ifndef GPPDSNDRecAlg_h
#define GPPDSNDRecAlg_h

#include "SniperKernel/AlgBase.h"

#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/Hit.h"

class DataSvc;

class GPPDSNDRecAlg: public AlgBase
{
    public:
        GPPDSNDRecAlg(const std::string& name);

        ~GPPDSNDRecAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
	int m_count;
	HitList* m_hitcol;
	EvtList* m_evtcol;
	NeutronPulse* m_pulse;
	DataSvc* m_svc;

    private: // tangb@ihep.ac.cn
	std::vector<int> m_buffer;
	void reconstruct(int time);
	int scan(int *Pos,int l,int &ThePosition,int &Real); 
	int sort(int *src, int len);
	int calculation(int &X,int &Y,int &Effect);
};

#endif
