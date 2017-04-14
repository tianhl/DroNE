#include <boost/python.hpp>

extern void export_DroNE();

BOOST_PYTHON_MODULE(libDataSvcPython)
{
	export_DroNE();
}
