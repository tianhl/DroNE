#ifndef NEUTRONCLIENTSTARTMONITOR_HH
#define NEUTRONCLIENTSTARTMONITOR_HH

#include <iostream>
#include <getopt.h>
#include "DataSvc/EpicsV4Hh/neutronClientRequestAndMonitor.hh"


using namespace std;
using namespace std::tr1;

//using std::tr1::shared_ptr;
using namespace epics::pvData;
using namespace epics::pvAccess;


class neutronClientStartMonitor
{
	public:

		neutronClientStartMonitor();
		typedef void (*P)(NeutronPulseData*); 
		void doMonitor(string const &name, string const &request, double timeout, short priority, int limit, bool quiet,P p);
	
		bool createFactory();

		void startMonitor(P p);


		string channel;
		string request;
		double timeout;
		bool monitor;
		bool quiet;
		short priority;
		int limit;

};

#endif
