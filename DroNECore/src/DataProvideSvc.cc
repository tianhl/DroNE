#include "DroNECore/DataProvideSvc.h"

#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"


DECLARE_SERVICE(DataProvideSvc);

DataProvideSvc::DataProvideSvc(const std::string& name)
	: SvcBase(name) {
}

DataProvideSvc::~DataProvideSvc(){
}

bool DataProvideSvc::initialize(){
	return true;
}

bool DataProvideSvc::finalize(){
	return true;
}

bool DataProvideSvc::read(uint8_t* buff, uint32_t size){
	LogTest << " DataProvideSvc read" << std::endl;
	return true;
}

size_t DataProvideSvc::count() const{
	return (size_t)NULL;
}
