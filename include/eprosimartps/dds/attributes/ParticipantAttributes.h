/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file ParticipantAttributes.h 	
 */

#ifndef PARTICIPANTPARAMETERS_H_
#define PARTICIPANTPARAMETERS_H_

#include "eprosimartps/common/types/Time_t.h"
#include "eprosimartps/common/types/Locator.h"

namespace eprosima {
namespace rtps {

class SimpleEDPAttributes{
public:
	bool use_PublicationWriterANDSubscriptionReader;
	bool use_PublicationReaderANDSubscriptionWriter;
	SimpleEDPAttributes():
		use_PublicationWriterANDSubscriptionReader(true),
		use_PublicationReaderANDSubscriptionWriter(true)
	{

	}
};



/**
 * Class BuiltinAttributes used to define the behavior of the Participant builtin protocols.
 * @ingroup ATTRIBUTESMODULE
 */
class BuiltinAttributes{
public:
	/**
	 * If set to false, NO discovery whatsoever would be used.
	 * Publisher and Subscriber defined with the same topic name would NOT be linked. All matching must be done
	 * manually through the addReaderLocator, addReaderProxy, addWriterProxy methods.
	 */
	bool use_SIMPLE_ParticipantDiscoveryProtocol;

	bool use_WriterLivelinessProtocol;
	/**
	 * If set to true, SimpleEDP would be used.
	 * This is NOT included in release 0.3.
	 */
	bool use_SIMPLE_EndpointDiscoveryProtocol;
	/**
	 * If set to true, StaticEDP based on an XML file would be implemented.
	 * The XML filename must be provided.
	 */
	bool use_STATIC_EndpointDiscoveryProtocol;

	//! StaticEDP XML filename, only necessary if use_STATIC_EndpointDiscoveryProtocol=true
	std::string m_staticEndpointXMLFilename;
	/**
	 * DomainId to be used by the Participant (80 by default).
	 */
	uint32_t domainId;
	//!Lease Duration of the participant, indicating how much time remote participants should consider this participant alive.
	Duration_t leaseDuration;
	/**
	 * The period for the Participant to send its Discovery Message to all other discovered Participants
	 * as well as to all Multicast ports.
	 */
	Duration_t leaseDuration_announcementperiod;
	SimpleEDPAttributes m_simpleEDP;

	BuiltinAttributes()
	{
		use_SIMPLE_ParticipantDiscoveryProtocol = true;
		use_SIMPLE_EndpointDiscoveryProtocol = true;
		use_STATIC_EndpointDiscoveryProtocol = false;
		leaseDuration_announcementperiod.seconds = 30;
		m_staticEndpointXMLFilename = "";
		domainId = 80;
		leaseDuration.seconds = 500;
		leaseDuration_announcementperiod.seconds = 250;
		use_WriterLivelinessProtocol = true;
	};
	virtual ~BuiltinAttributes(){};
};



/**
 * Class ParticipantParameters used to define different aspects of a participant.
 * @ingroup ATTRIBUTESMODULE
 */
class ParticipantAttributes {
public:
	ParticipantAttributes()
{
		defaultSendPort = 10040;
		name = "defaultParticipant";
		sendSocketBufferSize = 8712;
		listenSocketBufferSize = 17424;

}
	virtual ~ParticipantAttributes(){};
	/**
	 * Default list of Unicast Locators to be used for any Endpoint defined inside this participant in the case
	 * that it was defined with NO UnicastLocators. At least ONE locator should be included in this list.
	 */
	LocatorList_t defaultUnicastLocatorList;
	/**
	 * Default list of Multicast Locators to be used for any Endpoint defined inside this participant in the case
	 * that it was defined with NO UnicastLocators. This is usually left empty.
	 */
	LocatorList_t defaultMulticastLocatorList;
	/**
	 * Default send port that all Endpoints in the Participant would use to send messages.
	 * In this release all Endpoints use the same resource (socket) to send messages.
	 * THis will change in future releases.
	 */
	uint32_t defaultSendPort;
	//!Send socket buffer for the send resource.
	uint32_t sendSocketBufferSize;
	//!Listen socket buffer for all listen resources.
	uint32_t listenSocketBufferSize;
	/**
	 * Participant name.
	 */
	std::string name;
	//! Builtin parameters.
	BuiltinAttributes builtin;


};

} /* namespace rtps */
} /* namespace eprosima */

#endif /* PARTICIPANTPARAMETERS_H_ */
