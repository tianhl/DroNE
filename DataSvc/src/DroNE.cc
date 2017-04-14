#include "DataSvc/DroNE.h"

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/SvcBase.h"
#include "SniperKernel/DataMemSvc.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/DeclareDLE.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SniperLog.h"
#include <algorithm>

SNIPER_DECLARE_DLE(DroNE);


DroNE::DroNE(const std::string& name)
    : Task(name)
{
    declProp("is_online", m_onlinemode);
}

DroNE::~DroNE() {
}

bool DroNE::execute(){

  int run = switchStatus(Running);
  if ( run != 0 ) return (run > 0);

  ++m_loop;
  LogTest << " drone executing " << m_loop << std::endl;

  try {
    //trigger the BeginEvent incident
    local_fire("BeginEvent");

    //executing algorithms
    for ( std::list<AlgBase*>::iterator iAlg = m_algs.begin();
	  iAlg != m_algs.end();
	  ++iAlg ) {
      if ( ! (*iAlg)->execute() ) {
	LogError << "execute failed!" << std::endl;
	//FIXME: this is not an elegant way...
	Incident::fire("StopRun");
      }
    }

    //trigger the EndEvent incident
    local_fire("EndEvent");
    local_fire("hi");

  }
  catch (SniperStopped& e) {
    if ( ! m_isTop ) throw e;
    LogInfo << "Stopping execution..." << std::endl;
  }

  return true;

}

