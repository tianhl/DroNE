// @(#)DroNE/DroNEeCore:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  binding/DroNEOnlineExp.cc
//  
//
//  Created by Haolai TIAN 10th Dec 2017.
//
//
#include "DroNECore/DroNEOnline.h"
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

struct DroNEOnlineWrap : DroNEOnline, bp::wrapper<DroNEOnline>
{
    DroNEOnlineWrap(const std::string& name)
        : DroNEOnline(name)
    {
    }

    bool handle(Incident& i) {
        if ( bp::override f = this->get_override("handle") ) return f(i);
        return DroNEOnline::handle(i);
    }

    bool default_handle(Incident& i) {
        return this->DroNEOnline::handle(i);
    }

    bool run() {
        if ( bp::override f = this->get_override("run") ) return f();
        return DroNEOnline::run();
    }

    bool default_run() {
        return this->DroNEOnline::run();
    }

    bool initialize() {
        if ( bp::override f = this->get_override("initialize") ) return f();
        return DroNEOnline::initialize();
    }

    bool default_initialize() {
        return this->DroNEOnline::initialize();
    }

    bool execute() {
        if ( bp::override f = this->get_override("execute") ) return f();
        return DroNEOnline::execute();
    }

    bool default_execute() {
        return this->DroNEOnline::execute();
    }

    bool finalize() {
        if ( bp::override f = this->get_override("finalize") ) return f();
        return DroNEOnline::finalize();
    }

    bool default_finalize() {
        return this->DroNEOnline::finalize();
    }
};

void export_DroNEOnline()
{
    using namespace boost::python;

    class_<DroNEOnlineWrap, bases<Task>, boost::noncopyable>
        ("DroNEOnline", init<const std::string&>())
        .def("createSvc",  &DroNEOnline::createSvc,
                return_value_policy<reference_existing_object>())
        .def("createAlg",  &DroNEOnline::createAlg,
                return_value_policy<reference_existing_object>())
        .def("createTask", &DroNEOnline::createTask,
                return_value_policy<reference_existing_object>())
        .def("find",       &DroNEOnline::find,
                return_value_policy<reference_existing_object>())
        .def("handle",     &DroNEOnline::handle, &DroNEOnlineWrap::default_handle)
        .def("run",        &DroNEOnline::run, &DroNEOnlineWrap::default_run)
        .def("initialize", &DroNEOnline::initialize, &DroNEOnlineWrap::default_initialize)
        .def("execute",    &DroNEOnline::execute, &DroNEOnlineWrap::default_execute)
        .def("finalize",   &DroNEOnline::finalize, &DroNEOnlineWrap::default_finalize)
        .def("asTop",      &DroNEOnline::asTop)
        .def("isTop",      &DroNEOnline::isTop)
        .def("setEvtMax",  &DroNEOnline::setEvtMax)
        .def("clearTasks", &DroNEOnline::clearTasks)
        .def("clearAlgs",  &DroNEOnline::clearAlgs)
        .def("clearSvcs",  &DroNEOnline::clearSvcs)
        .def("remove",     &DroNEOnline::remove)
        ;
}
