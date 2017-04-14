#include "DataSvc/DroNE.h"
#include "SniperKernel/Incident.h"


#include "SniperKernel/Task.h"
#include "SniperKernel/AlgBase.h"
#include "SniperKernel/SvcBase.h"
#include "SniperKernel/DataMemSvc.h"
#include "SniperKernel/DeclareDLE.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SniperLog.h"

#include <boost/python.hpp>

namespace bp = boost::python;

struct DroNEWrap : DroNE, bp::wrapper<DroNE>
{
    DroNEWrap(const std::string& name)
        : DroNE(name)
    {
    }

    bool handle(Incident& i) {
        if ( bp::override f = this->get_override("handle") ) return f(i);
        return DroNE::handle(i);
    }

    bool default_handle(Incident& i) {
        return this->DroNE::handle(i);
    }

    bool run() {
        if ( bp::override f = this->get_override("run") ) return f();
        return DroNE::run();
    }

    bool default_run() {
        return this->DroNE::run();
    }

    bool initialize() {
        if ( bp::override f = this->get_override("initialize") ) return f();
        return DroNE::initialize();
    }

    bool default_initialize() {
        return this->DroNE::initialize();
    }

    bool execute() {
        if ( bp::override f = this->get_override("execute") ) return f();
        return DroNE::execute();
    }

    bool default_execute() {
        return this->DroNE::execute();
    }

    bool finalize() {
        if ( bp::override f = this->get_override("finalize") ) return f();
        return DroNE::finalize();
    }

    bool default_finalize() {
        return this->DroNE::finalize();
    }
};

void export_DroNE()
{
    using namespace boost::python;

    class_<DroNEWrap, bases<Task>, boost::noncopyable>
        ("DroNE", init<const std::string&>())
        .def("createSvc",  &DroNE::createSvc,
                return_value_policy<reference_existing_object>())
        .def("createAlg",  &DroNE::createAlg,
                return_value_policy<reference_existing_object>())
        .def("createTask", &DroNE::createTask,
                return_value_policy<reference_existing_object>())
        .def("find",       &DroNE::find,
                return_value_policy<reference_existing_object>())
        .def("handle",     &DroNE::handle, &DroNEWrap::default_handle)
        .def("run",        &DroNE::run, &DroNEWrap::default_run)
        .def("initialize", &DroNE::initialize, &DroNEWrap::default_initialize)
        .def("execute",    &DroNE::execute, &DroNEWrap::default_execute)
        .def("finalize",   &DroNE::finalize, &DroNEWrap::default_finalize)
        .def("asTop",      &DroNE::asTop)
        .def("isTop",      &DroNE::isTop)
        .def("setEvtMax",  &DroNE::setEvtMax)
        .def("clearTasks", &DroNE::clearTasks)
        .def("clearAlgs",  &DroNE::clearAlgs)
        .def("clearSvcs",  &DroNE::clearSvcs)
        .def("remove",     &DroNE::remove)
        ;
}
