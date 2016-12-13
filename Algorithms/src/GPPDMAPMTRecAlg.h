#ifndef GPPDMAPMTRecAlg_h
#define GPPDMAPMTRecAlg_h

#include "SniperKernel/AlgBase.h"

#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/Hit.h"

class DataSvc;

class GPPDMAPMTRecAlg: public AlgBase
{
/** @class GPPDMAPMTRecAlg  

    GPPDMAPMTRecAlg is an algorithm of GPPD, restructing raw data into
    event (position and TOF)

    @author Bin TANG, Haolai TIAN, CSNS, IHEP, CAS
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
        GPPDMAPMTRecAlg(const std::string& name);

        ~GPPDMAPMTRecAlg();

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
