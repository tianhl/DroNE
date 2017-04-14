#include "DataSvc/DataInputSvc.h"

#include "SniperKernel/Incident.h"
#include "DataSvc/DecodeRawData.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"


DECLARE_SERVICE(DataInputSvc);

DataInputSvc::DataInputSvc(const std::string& name)
: SvcBase(name) {
}

DataInputSvc::~DataInputSvc() {
}

bool DataInputSvc::initialize() {
	return true;
}



bool DataInputSvc::finalize() {
	return true;
}

bool DataInputSvc::next() {
	return true;
}
