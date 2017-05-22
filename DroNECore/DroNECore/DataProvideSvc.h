#ifndef DATAPROVIDERSVC_H
#define DATAPROVIDERSVC_H


#include <string>
#include <vector>
#include <fstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "SniperKernel/SvcBase.h"

class DataProvideSvc : public SvcBase{
	public:
		DataProvideSvc(const std::string& name);
		virtual ~DataProvideSvc();

		virtual bool   initialize();
		virtual bool   finalize();
		virtual bool   read(uint8_t* buff, uint32_t size);
		virtual size_t count() const;
};

#endif
