#ifndef REFLMAPMTALG_h
#define REFLMAPMTALG_h

#include "SniperKernel/AlgBase.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "Data/Pulse.h"
#include "Data/Hit.h"


class DataSvc;

class REFLMAPMTSimAlg: public AlgBase
{
/** @class REFLMAPMTSimAlg 

    REFLMAPMTSimAlg is an algorithm of REFL, which reads the instrument
    simulation result and writes to DataSvc.

    @author Haolai TIAN, CSNS, IHEP, CAS
    @date 12/12/2016

    Copyright &copy; 2016-now CSNS, IHEP, CAS

    This file is part of DroNE.

    DroNE is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    DroNE is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
    public:
        REFLMAPMTSimAlg(const std::string& name);

        ~REFLMAPMTSimAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
	uint32_t mapIdx(uint32_t tofidx, uint32_t detidx);
	//uint32_t detXIdx(uint32_t detidx){return detidx%m_detXMax;};
	//uint32_t detYIdx(uint32_t detidx){return detidx/m_detXMax;};

    private:
        uint64_t  m_count;
        uint64_t  m_totcnt;
	uint64_t  m_curcnt;
	uint32_t* m_cmap;
	int m_maxtof;
	int m_maxdet;
	//int m_detXMax;


	HitList* m_hitcol;
	NeutronPulse* m_pulse;

	DataSvc* m_svc;
	std::string m_inputfilename;
	std::ifstream m_ifstream;
};

#endif
