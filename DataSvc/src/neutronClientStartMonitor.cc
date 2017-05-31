// @(#)DroNE/DataSvc:$Id$
// Authors: K. ZHOU 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Control Group, CSNS                             *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  neutronClientStartMonitor.cc
//  
//
//  Created by Ke ZHOU 31th May 2017.
//
//
#include "DataSvc/EpicsV4Hh/neutronClientStartMonitor.hh"


using namespace std;
using namespace std::tr1;

using std::exception;
using std::runtime_error;

using namespace epics::pvData;
using namespace epics::pvAccess;


neutronClientStartMonitor::neutronClientStartMonitor()
{
  channel = "neutrons";
  request = "record[queueSize=100]field()";
  timeout = 2.0;
  monitor = false;
  quiet = false;
  priority = ChannelProvider::PRIORITY_DEFAULT;
  limit = 0;

}

bool neutronClientStartMonitor::createFactory()
{
	  try {
	    printf("Starting ClientFactory::start()\n");
	    epics::pvAccess::ClientFactory::start();
	  } catch (std::exception &e)  {
	    fprintf(stderr, "ERROR: Exception for ClientFactory::start(). Exception: %s\n", e.what());
	    return false;
	  }

	  epicsThreadSleep(1);

	  return true;

}


void neutronClientStartMonitor::doMonitor(string const &name, string const &request, double timeout, short priority, int limit, bool quiet,P p)
{
    ChannelProvider::shared_pointer channelProvider =
            getChannelProviderRegistry()->getProvider("pva");
    if (! channelProvider)
        THROW_EXCEPTION2(runtime_error, "No channel provider");

    shared_ptr<MyChannelRequester> channelRequester(new MyChannelRequester());
    shared_ptr<Channel> channel(channelProvider->createChannel(name, channelRequester, priority));
    channelRequester->waitUntilConnected(timeout);

    cout<<  "the request:::" << request << endl;
    shared_ptr<PVStructure> pvRequest = CreateRequest::create()->createRequest(request);
    shared_ptr<MyMonitorRequester> monitorRequester(new MyMonitorRequester(limit, quiet,p));
    shared_ptr<Monitor> monitor = channel->createMonitor(monitorRequester, pvRequest);
    // Wait until limit or forever..
  //  cout<<  "the first struct:::" << *(monitorRequester->getPulseData().pTimeOfFlight)<< endl;
    monitorRequester->waitUntilDone();
   
    //What to do for graceful shutdown of monitor?
    
    Status stat = monitor->stop();
    if (! stat.isSuccess())
        cout << "Cannot stop monitor, " << stat << endl;
    monitor->destroy();
    channel->destroy();
}


void neutronClientStartMonitor::startMonitor(P p)
{
	try
	{
	    createFactory();
	    doMonitor(channel, request, timeout, priority, limit, quiet,p);       
	    epics::pvAccess::ClientFactory::stop();
	}
	catch (exception &ex)
	{
	    fprintf(stderr, "Exception: %s\n", ex.what());
	    PRINT_EXCEPTION2(ex, stderr);
	    cout << SHOW_EXCEPTION(ex);
	}
}
