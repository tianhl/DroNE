#include <boost/python.hpp>

extern void export_DroNE();
extern void export_DroNE_Incident();


BOOST_PYTHON_MODULE(libDataSvcPython)
{
	export_DroNE();
	export_DroNE_Incident();
}
