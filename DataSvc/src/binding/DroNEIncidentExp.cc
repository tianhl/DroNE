#include "DataSvc/DroNEIncident.h"
#include <boost/python.hpp>


void export_DroNE_Incident()
{
	using namespace boost::python;

	//bool (DroNEIncident::*member_fire)() = &DroNEIncident::fire;

	class_<DroNEIncident, bases<Incident>, boost::noncopyable>
		("DroNEIncident", init<const std::string&>())
		.def("getRetVal", &DroNEIncident::getRetVal,
				return_value_policy<copy_const_reference>())
		;
}
