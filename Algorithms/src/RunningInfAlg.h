#ifndef RUNNINGINFALG_h
#define RUNNINGINFALG_h

#include "SniperKernel/AlgBase.h"
#include <string>

class DataSvc;

class RunningInfAlg: public AlgBase
{
/** @class RunningInf 

    RunningInf is an example of DroNE algorithm, showing how to access 
    and modify the data stored in the DataSvc.

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
        RunningInfAlg(const std::string& name);

        ~RunningInfAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:

	DataSvc* m_svc;
};

#endif
