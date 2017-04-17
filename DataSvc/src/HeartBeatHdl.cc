#include "DataSvc/HeartBeatHdl.h"

#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"

#include <time.h>
#include <stdio.h> 


	HeartBeatHdl::HeartBeatHdl(Task* par)
: m_par(par)
{

}

bool HeartBeatHdl::handle(Incident& /*incident*/)
{
	time_t t = time(NULL);
	std::cout << "local time: " << t << std::endl;
	return true;
}
