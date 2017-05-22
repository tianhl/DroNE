#ifndef DATA_INPUT_SVC_H
#define DATA_INPUT_SVC_H

#include <vector>
#include <string>

#include "SniperKernel/SvcBase.h"


class DataInputSvc : public SvcBase
{
    public :

      DataInputSvc(const std::string& name);
      virtual ~DataInputSvc();

      virtual bool initialize();
      virtual bool finalize();
      virtual bool next();

};


#endif
