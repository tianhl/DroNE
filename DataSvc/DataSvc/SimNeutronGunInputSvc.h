#ifndef SIMNEUTRONGUN_INPUTSVC_h
#define SIMNEUTRONGUN_INPUTSVC_h
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  SimNeutronGunInputSvc.h
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "DroNECore/DataInputSvc.h"
#include "Data/Pulse.h"
#include "Data/SNDHit.h"


class DataSvc;

class SimNeutronGunInputSvc: public DataInputSvc
{
    public:
        SimNeutronGunInputSvc(const std::string& name);

        ~SimNeutronGunInputSvc();

        bool initialize();
        bool finalize();
      bool next();

    private:
	uint32_t mapIdx(uint32_t tofidx, uint32_t detidx);
	//uint32_t detXIdx(uint32_t detidx){return detidx%m_detXMax;};
	//uint32_t detYIdx(uint32_t detidx){return detidx/m_detXMax;};

    private:
        uint64_t  m_count;
        uint64_t  m_totcnt;
	uint64_t  m_curcnt;

	int m_maxtof;
	int m_maxdet;
	int m_maxevt;

	uint32_t* m_cmap;
	uint32_t* m_dmap;
	uint32_t* m_tmap;

	SNDHitList* m_hitcol;
	NeutronPulse* m_pulse;

	DataSvc* m_svc;
	std::string m_inputfilename;
	std::ifstream m_ifstream;
};

#endif
