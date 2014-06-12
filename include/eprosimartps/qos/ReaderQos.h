/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file ReaderQos.h
 *
*/

#ifndef READERQOS_H_
#define READERQOS_H_
#include "eprosimartps/qos/DDSQosPolicies.h"
namespace eprosima {
namespace dds {

/**
 * ReaderQos class contains all the possible Qos that can be set for a determined Subscriber/Reader.
 * Although this values can be set and are transmitted in the Discovery the behaviour associated with them is yet to be implemented.
 */
class ReaderQos{
public:
	ReaderQos(){};
	virtual ~ReaderQos(){};
	DurabilityQosPolicy m_durability;
		DeadlineQosPolicy m_deadline;
		LatencyBudgetQosPolicy m_latencyBudget;
		LivelinessQosPolicy m_liveliness;
		ReliabilityQosPolicy m_reliability;
		OwnershipQosPolicy m_ownership;
		DestinationOrderQosPolicy m_destinationOrder;
		UserDataQosPolicy m_userData;
		TimeBasedFilterQosPolicy m_timeBasedFilter;
		PresentationQosPolicy m_presentation;
		PartitionQosPolicy m_partition;
		TopicDataQosPolicy m_topicData;
		GroupDataQosPolicy m_groupData;
		DurabilityServiceQosPolicy m_durabilityService;
		LifespanQosPolicy m_lifespan;
		void setQos( ReaderQos& readerqos, bool first_time);
};



} /* namespace dds */
} /* namespace eprosima */

#endif /* READERQOS_H_ */
