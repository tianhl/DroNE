#ifndef NEUTRONCLIENTREQUESTANDMONITOR_HH
#define NEUTRONCLIENTREQUESTANDMONITOR_HH
#include <iostream>
#include <getopt.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>

#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "epicsThread.h"
#include "epicsTime.h"
#include "pv/epicsException.h"
#include "pv/createRequest.h"
#include "pv/event.h"
#include "pv/pvData.h"
#include "pv/clientFactory.h"
#include "pv/pvAccess.h"
#include "pv/monitor.h"
//#include "neutronV4Interface.hh"
//#include ""
//#define TIME_IT
#ifdef TIME_IT
#include "DataSvc/EpicsV4Hh/nanoTimer.hh"
#endif


using namespace std;
using namespace std::tr1;
using namespace epics::pvData;
using namespace epics::pvAccess;



typedef struct MonitorNeutronEventData{
  uint32_t *pTimeOfFlight;
  uint32_t *pPixelID;  
}MonitorNeutronEventData;

typedef struct NeutronPulseData{
  uint32_t mSpecID;   // 谱仪ID
  uint32_t mDetTypeID;   // 探测器类型ID
  double mProtonCharge;  // 质子流强
  uint64_t mPulseT0High;  // T0绝对时间的高八位
  uint64_t mPulseT0Low;   // T0绝对时间的低八位
  uint32_t mHitCounts;   // 单个Pulse中的中子事件数
  MonitorNeutronEventData mMonitorNeutronEventData;
}NeutronPulseData;


/** Requester implementation,
 *  used as base for all the following *Requester
 */
class MyRequester : public virtual Requester
{
    string requester_name;
public:
    MyRequester(string const &requester_name)
    : requester_name(requester_name)
    {}

    string getRequesterName()
    {   return requester_name; }

    void message(string const & message, MessageType messageType);
};



class MyChannelRequester : public virtual MyRequester, public virtual ChannelRequester
{
    Event connect_event;
public:
    MyChannelRequester() : MyRequester("MyChannelRequester")
    {}

    void channelCreated(const Status& status, Channel::shared_pointer const & channel);
    void channelStateChange(Channel::shared_pointer const & channel, Channel::ConnectionState connectionState);

    boolean waitUntilConnected(double timeOut)
    {
        return connect_event.wait(timeOut);
    }
};

class MyChannelGetRequester : public virtual MyRequester, public virtual ChannelGetRequester
{
    Event done_event;
public:
    MyChannelGetRequester() : MyRequester("MyChannelGetRequester")
    {}

    void channelGetConnect(const Status& status,
            ChannelGet::shared_pointer const & channelGet,
            Structure::const_shared_pointer const & structure);

    void getDone(const Status& status,
            ChannelGet::shared_pointer const & channelGet,
            PVStructure::shared_pointer const & pvStructure,
            BitSet::shared_pointer const & bitSet);

    boolean waitUntilDone(double timeOut)
    {   
        return done_event.wait(timeOut);
    }
};



class MyMonitorRequester : public virtual MyRequester, public virtual MonitorRequester
{
    int limit;
    bool quiet;
    Event done_event;

    epicsTime next_run;
#   ifdef TIME_IT
    NanoTimer value_timer;
#   endif
    size_t user_tag_offset;
    size_t tof_offset;
    size_t pixel_offset;
    int monitors;
    uint64 updates;
    uint64 overruns;
    uint64 last_pulse_id;
    uint64 missing_pulses;
    uint64 array_size_differences;
    typedef void (*P)(NeutronPulseData*);  
     P pSetFunc; 

    void checkUpdate(shared_ptr<PVStructure> const &structure);
public:
    MyMonitorRequester(int limit, bool quiet,P p)
    : MyRequester("MyMonitorRequester"),
      limit(limit), quiet(quiet),
      next_run(epicsTime::getCurrent()),
      user_tag_offset(-1), tof_offset(-1), pixel_offset(-1),
      monitors(0), updates(0), overruns(0), last_pulse_id(0), missing_pulses(0), array_size_differences(0)
    {
	pSetFunc = p;
       
	uint32_t tem = 0;
        uint32_t tem1 = 0;
        mNeutronPulseDataTem.mMonitorNeutronEventData.pTimeOfFlight = &tem;
        mNeutronPulseDataTem.mMonitorNeutronEventData.pPixelID = &tem1;
        mNeutronPulseDataTem.mPulseT0High = 99999999;
        mNeutronPulseDataTem.mPulseT0Low = 11111111;
        mNeutronPulseDataTem.mSpecID = 1;
        mNeutronPulseDataTem.mDetTypeID = 2;
}

    epics::pvData::uint32 pixelsLength;
    epics::pvData::shared_vector<const epics::pvData::uint32> tofData;
    epics::pvData::shared_vector<const epics::pvData::uint32> pixelsData;
    NeutronPulseData mNeutronPulseDataTem;
    //void setFunction(P);


    void monitorConnect(Status const & status, MonitorPtr const & monitor, StructureConstPtr const & structure);
    void monitorEvent(MonitorPtr const & monitor);
    void unlisten(MonitorPtr const & monitor);
    

    boolean waitUntilDone()
    {   
        return done_event.wait();
    }
};

#endif
