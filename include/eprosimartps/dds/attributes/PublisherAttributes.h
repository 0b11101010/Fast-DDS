/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file PublisherAttributes.h	
 */

#ifndef PUBLISHERPARAMETERS_H_
#define PUBLISHERPARAMETERS_H_

#include "eprosimartps/common/types/Locator.h"
#include "eprosimartps/common/types/Time_t.h"
#include "eprosimartps/dds/attributes/TopicAttributes.h"
#include "eprosimartps/qos/WriterQos.h"

using namespace eprosima::rtps;

namespace eprosima {
namespace dds {


class PublisherTimes{
public:
	//!Period to send HB.
	Duration_t heartbeatPeriod;
	//!Delay response to a negative ack from a reader.
	Duration_t nackResponseDelay;
	//!Allows the reader to deny a response to a nack that arrives too soon after the change is sent.
	Duration_t nackSupressionDuration;
	//!The writer sends data periodically to all ReaderLocators (only in BEST_EFFORT - STATELESS combination).
	Duration_t resendDataPeriod;

	//uint8_t hb_per_max_samples;
	PublisherTimes(){
		heartbeatPeriod.seconds = 3;
		nackResponseDelay.fraction = 200*1000*1000;
	}
	~PublisherTimes(){};
};



/**
 * Class PublisherAttributes, used by the user to define the attributes of a Publisher.
 * @ingroup ATTRIBUTESMODULE
 */
class PublisherAttributes {

public:
	PublisherAttributes()
{
		pushMode = true;
		historyMaxSize = 10;
		userDefinedId = -1;
		payloadMaxSize = 500;
};
	virtual ~PublisherAttributes(){};
	//! If set to true the Publisher will send the data directly, if set to false it will send
	//! a Heartbeat message and wait for ACKNACK messages (option only available for RELIABLE Publishers.)
	//! to send the data.
	bool pushMode;
	/**
	 * Maximum size of the History.
	 */
	uint16_t historyMaxSize;
	//! Unicast LocatorList where the writer should be listening for responses (RELIABLE only).
	LocatorList_t unicastLocatorList;
	//!MulticastLocatorList where the writer should be listening for responses (RELIABLE only).
	LocatorList_t multicastLocatorList;
	//!Reliability parameters for the Publisher
	PublisherTimes times;
	//!Topic Attributes for the Publisher
	TopicAttributes topic;
	//! User defined Id for this Publisher (only needed in STATICEDP)
	int16_t userDefinedId;
	WriterQos qos;

	uint32_t payloadMaxSize;

};

} /* namespace rtps */
} /* namespace eprosima */

#endif /* PUBLISHERPARAMETERS_H_ */
