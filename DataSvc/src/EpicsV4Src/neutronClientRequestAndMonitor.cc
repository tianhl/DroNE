
#include "DataSvc/EpicsV4Hh/neutronClientRequestAndMonitor.hh"





using namespace epics::pvData;
using namespace epics::pvAccess;


/** Requester implementation,
 *  used as base for all the following *Requester
 */
void MyRequester::message(string const & message, MessageType messageType)
{
    cout << getMessageTypeName(messageType) << ": "
         << requester_name << " "
         << message << endl;
}

/** Requester for channel and status updates */


void MyChannelRequester::channelCreated(const Status& status, Channel::shared_pointer const & channel)
{
    cout << channel->getChannelName() << " created, " << status << endl;
}

void MyChannelRequester::channelStateChange(Channel::shared_pointer const & channel, Channel::ConnectionState connectionState)
{
    cout << channel->getChannelName() << " state: "
         << Channel::ConnectionStateNames[connectionState]
         << " (" << connectionState << ")" << endl;
    if (connectionState == Channel::CONNECTED)
        connect_event.signal();
}

/** Requester for 'getting' a single value */

void MyChannelGetRequester::channelGetConnect(const Status& status,
        ChannelGet::shared_pointer const & channelGet,
        Structure::const_shared_pointer const & structure)
{
    // Could inspect or memorize the channel's structure...
    if (status.isSuccess())
    {
        cout << "ChannelGet for " << channelGet->getChannel()->getChannelName()
             << " connected, " << status << endl;
        structure->dump(cout);

        channelGet->get();
    }
    else
    {
        cout << "ChannelGet for " << channelGet->getChannel()->getChannelName()
             << " problem, " << status << endl;
        done_event.signal();
    }
}

void MyChannelGetRequester::getDone(const Status& status,
        ChannelGet::shared_pointer const & channelGet,
        PVStructure::shared_pointer const & pvStructure,
        BitSet::shared_pointer const & bitSet)
{
    cout << "ChannelGet for " << channelGet->getChannel()->getChannelName()
         << " finished, " << status << endl;

    if (status.isSuccess())
    {
        pvStructure->dumpValue(cout);
        done_event.signal();
    }
}

/** Requester for 'monitoring' value changes of a channel */
void MyMonitorRequester::monitorConnect(Status const & status, MonitorPtr const & monitor, StructureConstPtr const & structure)
{
    cout << "Monitor connects, " << status << endl;
    if (status.isSuccess())
    {
        // Check the structure by using only the Structure API?
        // Need to navigate the hierarchy, won't get the overall PVStructure offset.
        // Easier: Create temporary PVStructure
        PVStructurePtr pvStructure = getPVDataCreate()->createPVStructure(structure);
        shared_ptr<PVInt> user_tag = pvStructure->getSubField<PVInt>("timeStamp.userTag");
        if (! user_tag)
        {
            cout << "No 'timeStamp.userTag'" << endl;
            return;
        }
        user_tag_offset = user_tag->getFieldOffset();

        shared_ptr<PVUIntArray> tof = pvStructure->getSubField<PVUIntArray>("time_of_flight.value");
        if (! tof)
        {
            cout << "No 'time_of_flight'" << endl;
            return;
        }
        tof_offset = tof->getFieldOffset();

        shared_ptr<PVUIntArray> pixel = pvStructure->getSubField<PVUIntArray>("pixel.value");
        if (! pixel)
        {
            cout << "No 'pixel'" << endl;
            return;
        }
        pixel_offset = pixel->getFieldOffset();

        // pvStructure is disposed; keep value_offset to read data from monitor's pvStructure

        monitor->start();
    }
}


void MyMonitorRequester::monitorEvent(MonitorPtr const & monitor)
{  

	cout << "pass pass go  go " <<endl;
	shared_ptr<MonitorElement> update;
    while ((update = monitor->poll()))
    {
        // TODO Simulate slow client -> overruns on client side
        // epicsThreadSleep(0.1);
        // 回调函数
        ++updates;

        checkUpdate(update->pvStructurePtr);
         epics::pvData::PVUIntArrayPtr pixelsPtr_client = update->pvStructurePtr->getSubField<epics::pvData::PVUIntArray>("pixel.value");
         epics::pvData::PVUIntArrayPtr tofPtr_client = update->pvStructurePtr->getSubField<epics::pvData::PVUIntArray>("time_of_flight.value");
         if(pixelsPtr_client && tofPtr_client) {
         
            pixelsLength = pixelsPtr_client->getLength();
            pixelsData = pixelsPtr_client->view();
            tofData = tofPtr_client->view();
            // ---------GET the DATA-----------  
            mNeutronPulseDataTem.mMonitorNeutronEventData.pTimeOfFlight = new uint32_t[pixelsLength];
            mNeutronPulseDataTem.mMonitorNeutronEventData.pPixelID = new uint32_t[pixelsLength];
            for(int list_num_pixel = 0; list_num_pixel<pixelsLength; list_num_pixel++){
            mNeutronPulseDataTem.mMonitorNeutronEventData.pTimeOfFlight[list_num_pixel]= tofData[list_num_pixel];
            mNeutronPulseDataTem.mMonitorNeutronEventData.pPixelID[list_num_pixel]= tofData[list_num_pixel];           
        
            mNeutronPulseDataTem.mHitCounts = pixelsLength;

	    pSetFunc(&mNeutronPulseDataTem);	
            
	   }
		
            //  ----- -- --- --- -- --------------------------
            cout << " the pixels Data is ::  "<< pixelsData[0] <<endl;
            cout <<  " the tof Data is ::" << tofData[0] << endl;
        }      

	//pSetFunc = ;
        //neutronV4Interface* mNeutronV4Interface= new neutronV4Interface();
        //mNeutronV4Interface->getPulseData(&mNeutronPulseDataTem);
        // update->changedBitSet indicates which elements have changed.
        // update->overrunBitSet indicates which elements have changed more than once,
        // i.e. we missed one (or more !) updates.

        if (! update->overrunBitSet->isEmpty())
            ++overruns;
        if (quiet)
        {
            epicsTime now(epicsTime::getCurrent());
            if (now >= next_run)
            {
                double received_perc = 100.0 * updates / (updates + missing_pulses);
         
                overruns = 0;
                missing_pulses = 0;
                updates = 0;
                array_size_differences = 0;

#               ifdef TIME_IT
                cout << "Time for value lookup: " << value_timer << endl;
#               endif

                next_run = now + 10.0;
            }
        }
        // else
        // {
        //     //  用于输出structure结构
        //     // cout << "Monitor:\n";

        //     // cout << "Changed: " << *update->changedBitSet.get() << endl;
        //     // cout << "Overrun: " << *update->overrunBitSet.get() << endl;
        //     // important point  here  here 
        //     //update->pvStructurePtr->dumpValue(cout);
        //     cout << endl;
        // }
        monitor->release(update);
        delete mNeutronPulseDataTem.mMonitorNeutronEventData.pTimeOfFlight;
        delete mNeutronPulseDataTem.mMonitorNeutronEventData.pPixelID;
    }
    ++ monitors;
     cout << " 222222...... "<<endl;
    if (limit > 0  &&  monitors >= limit)
    {
    	cout << "Received " << monitors << " monitors" << endl;
    	done_event.signal();
    }
    
    //cout << "pass pass go  go " <<endl;
}

// void MyMonitorRequester::getPulseData(NeutronPulseData *pNeutronPulseData)
// {

//    cout <<" print the structure::" <<*(pNeutronPulseData->pTimeOfFlight) <<endl;

// }


void MyMonitorRequester::checkUpdate(shared_ptr<PVStructure> const &pvStructure)
{
#   ifdef TIME_IT
    value_timer.start();
#   endif

    // Time for value lookup when re-using offset: 2us
    shared_ptr<PVInt> value = dynamic_pointer_cast<PVInt>(pvStructure->getSubField(user_tag_offset));

    // Compare: Time for value lookup when using name: 12us
    // shared_ptr<PVInt> value = pvStructure->getIntField("timeStamp.userTag");
    if (! value)
    {
        cout << "No 'timeStamp.userTag'" << endl;
        return;
    }

#   ifdef TIME_IT
    value_timer.stop();
#   endif

    // Check pulse ID for skipped updates
    uint64 pulse_id = static_cast<uint64>(value->get());
    if (last_pulse_id != 0)
    {
        int missing = pulse_id - 1 - last_pulse_id;
        if (missing > 0)
            missing_pulses += missing;
    }
    last_pulse_id = pulse_id;

    // Compare lengths of tof and pixel arrays
    shared_ptr<PVUIntArray> tof =
     dynamic_pointer_cast<PVUIntArray>(pvStructure->getSubField(tof_offset));
    epics::pvData::shared_vector<const epics::pvData::uint32> tofData = tof->view();
    if (!tof)
    {
        cout << "No 'time_of_flight' array" << endl;
        return;
    }

    shared_ptr<PVUIntArray> pixel = dynamic_pointer_cast<PVUIntArray>(pvStructure->getSubField(pixel_offset));
    if (!pixel)
    {
        cout << "No 'pixel' array" << endl;
        return;
    }

    if (tof->getLength() != pixel->getLength())
    {
        ++array_size_differences;
        if (! quiet)
        {
            cout << "time_of_flight: " << tof->getLength() << " elements" << endl;
            shared_vector<const uint32> tof_data;
            tof->getAs(tof_data);
            cout << tof_data << endl;

            cout << "pixel: " << pixel->getLength() << " elements" << endl;
            shared_vector<const uint32> pixel_data;
            pixel->getAs(pixel_data);
            cout << pixel_data << endl;
        }
    }
}


void MyMonitorRequester::unlisten(MonitorPtr const & monitor)
{
    cout << "Monitor unlistens" << endl;
}


