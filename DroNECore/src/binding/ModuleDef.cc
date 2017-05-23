#include <boost/python.hpp>

extern void export_DroNE();
extern void export_DroNE_Incident();


BOOST_PYTHON_MODULE(libDroNECorePython)
{
	export_DroNE();
	export_DroNE_Incident();
}
