#ifndef DRONE_H
#define DRONE_H

#include "SniperKernel/Task.h"

class DroNE : public Task{
   public:
      DroNE(const std::string& name);
      virtual ~DroNE();
      virtual bool execute();
   private:
      bool m_onlinemode;
};

#endif
