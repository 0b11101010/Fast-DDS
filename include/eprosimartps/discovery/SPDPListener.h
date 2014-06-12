/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file SPDPListener.h
 */

#ifndef SPDPLISTENER2_H_
#define SPDPLISTENER2_H_

#include "eprosimartps/dds/SubscriberListener.h"
#include "eprosimartps/qos/ParameterList.h"


namespace eprosima {

using namespace dds;

namespace rtps {

class SimplePDP;
class DiscoveredParticipantData;


/**
 * Class SPDPListener, specification of SubscriberListener used by the SPDP to perform the History check when a new message is received.
 * This class is implemented in order to use the same structure than with any other RTPSReader.
 * @ingroup DISCOVERYMODULE
 */
class SPDPListener: public SubscriberListener {
public:
	SPDPListener(SimplePDP* in_SPDP):mp_SPDP(in_SPDP){};
	virtual ~SPDPListener(){};
	//!Pointer to the associated mp_SPDP;
	SimplePDP* mp_SPDP;
	//!Method to be called when a new data message is received.
	void onNewDataMessage();
	//!Process a new added cache with this method.
	bool newAddedCache();

	bool processParameterList(ParameterList_t& param,DiscoveredParticipantData* pdata);

	void assignUserId(std::string& type,uint16_t userId,EntityId_t& entityId,DiscoveredParticipantData* pdata);
};

} /* namespace rtps */
} /* namespace eprosima */

#endif /* SPDPLISTENER2_H_ */


