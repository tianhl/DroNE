#include "CtrlSvc/EndEvtHdl.h"

#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"


EndEvtHdl::EndEvtHdl(Task* par)
: m_par(par)
{

	//SniperPtr<DataInputSvc> pSvc("DataInputSvc");
	//if ( pSvc.invalid()) {
	//	throw SniperException("DataInputSvc is invalid!");
	//}
	//m_iSvc = pSvc.data();

}

bool EndEvtHdl::handle(Incident& /*incident*/)
{
	//if ( m_iSvc->next() ) {
	//	return true;
	//}
	//if (){
	//	return Incident::fire("EndEvent");
	//}
	//return Incident::fire("StopRun");
	//std::cout << "end event" << std::endl;
	return true;
}