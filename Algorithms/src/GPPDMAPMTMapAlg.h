#ifndef GPPDMAPMTMapAlg_h
#define GPPDMAPMTMapAlg_h

#include "SniperKernel/AlgBase.h"

#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/Hit.h"

class DataSvc;

class GPPDMAPMTMapAlg: public AlgBase
{
/** @class GPPDMAPMTMapAlg 

    GPPDMAPMTMapAlg is an algorithm for GPPD, mapping the event 
    position to pixel id.

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
        GPPDMAPMTMapAlg(const std::string& name);

        ~GPPDMAPMTMapAlg();

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
