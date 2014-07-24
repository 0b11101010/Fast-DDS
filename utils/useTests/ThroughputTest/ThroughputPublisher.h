/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file ThroughputPublisher.h
 *
 */

#ifndef THROUGHPUTPUBLISHER_H_
#define THROUGHPUTPUBLISHER_H_

#include <vector>
#include <string>


using namespace std;

#include "ThroughputTypes.h"



class ThroughputPublisher
{
public:
	ThroughputPublisher();
	virtual ~ThroughputPublisher();
	Participant* mp_par;
	Publisher* mp_datapub;
	Publisher* mp_commandpub;
	Subscriber* mp_commandsub;
	eClock m_Clock;
	Time_t m_t1,m_t2;
	uint64_t m_overhead;
	boost::interprocess::interprocess_semaphore sema;
	class DataPubListener:public PublisherListener
	{
	public:
		DataPubListener(ThroughputPublisher& up);
		virtual ~DataPubListener();
		ThroughputPublisher& m_up;
		void onPublicationMatched(MatchingInfo info);
	}m_DataPubListener;

	class CommandSubListener:public SubscriberListener
	{
	public:
		CommandSubListener(ThroughputPublisher& up);
		virtual ~CommandSubListener();
		ThroughputPublisher& m_up;
		void onSubscriptionMatched(MatchingInfo info);
	}m_CommandSubListener;
	class CommandPubListener:public PublisherListener
	{
	public:
		CommandPubListener(ThroughputPublisher& up);
		virtual ~CommandPubListener();
		ThroughputPublisher& m_up;
		void onPublicationMatched(MatchingInfo info);
	}m_CommandPubListener;


	bool ready;

	void run(std::vector<uint32_t>& demand);
	void test(uint32_t demand);
	std::vector<TroughputTimeStats> m_timeStats;
};



#endif /* THROUGHPUTPUBLISHER_H_ */
