/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file Participant.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#if defined(_WIN32)
#include <process.h>
#else
#include <unistd.h>
#endif

#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>


#include "eprosimartps/dds/attributes/all_attributes.h"

#include "eprosimartps/resources/ResourceEvent.h"
#include "eprosimartps/resources/ResourceListen.h"
#include "eprosimartps/resources/ListenResource.h"
#include "eprosimartps/resources/ResourceSend.h"

#include "eprosimartps/qos/ReaderQos.h"
#include "eprosimartps/qos/WriterQos.h"

#include "eprosimartps/Endpoint.h"



#ifndef PARTICIPANT_H_
#define PARTICIPANT_H_

namespace eprosima {

namespace dds
{
class DomainParticipant;
class DDSTopicDataType;
class PublisherListener;
class SubscriberListener;



}

using namespace dds;

namespace rtps {

class StatelessWriter;
class StatelessReader;
class StatefulWriter;
class StatefulReader;
class RTPSReader;
class RTPSWriter;

class ParticipantDiscoveryProtocol;



/**
 * @brief Class ParticipantImpl, it contains the private implementation of the Participant functions and allows the creation and removal of writers and readers. It manages the send and receive threads.
 * @ingroup MANAGEMENTMODULE
 */
class ParticipantImpl
{
public:

	ParticipantImpl(const ParticipantAttributes &param,const GuidPrefix_t& guidP,uint32_t ID);
	virtual ~ParticipantImpl();


	/**
	 * Create a Reader in this Participant.
	 * @param Reader Pointer to pointer of the Reader, used as output. Only valid if return==true.
	 * @param RParam SubscriberAttributes to define the Reader.
	 * @param payload_size Maximum payload size.
	 * @param isBuiltin Bool value indicating if the Reader is builtin (Discovery or Liveliness protocol) or is created for the end user.
	 * @param kind STATEFUL or STATELESS.
	 * @param ptype Pointer to the DDSTOpicDataType object (optional).
	 * @param slisten Pointer to the SubscriberListener object (optional).
	 * @param entityId EntityId assigned to the Reader.
	 * @return True if the Reader was correctly created.
	 */
	bool createReader(RTPSReader** Reader,SubscriberAttributes& RParam,uint32_t payload_size,bool isBuiltin,StateKind_t kind,
			DDSTopicDataType* ptype = NULL,SubscriberListener* slisten=NULL,const EntityId_t& entityId = c_EntityId_Unknown);
	/**
	 * Create a Writer in this Participant.
	 * @param Writer Pointer to pointer of the Writer, used as output. Only valid if return==true.
	 * @param param PublisherAttributes to define the Writer.
	 * @param payload_size Maximum payload size.
	 * @param isBuiltin Bool value indicating if the Writer is builtin (Discovery or Liveliness protocol) or is created for the end user.
	 * @param kind STATELESS or STATEFUL
	 * @param ptype Pointer to the DDSTOpicDataType object (optional).
	 * @param plisten Pointer to the PublisherListener object (optional).
	 * @param entityId EntityId assigned to the Writer.
	 * @return True if the Writer was correctly created.
	 */
	bool createWriter(RTPSWriter** Writer,PublisherAttributes& param,uint32_t payload_size,bool isBuiltin,StateKind_t kind,
				DDSTopicDataType* ptype = NULL,PublisherListener* plisten=NULL,const EntityId_t& entityId = c_EntityId_Unknown);

	bool assignEndpointListenResources(Endpoint* endp,bool isBuiltin);
	bool assignLocator2ListenResources(Endpoint* pend,LocatorListIterator lit,bool isMulticast,bool isFixed);


	/**
	 * Remove Endpoint from the participant. It closes all entities related to them that are no longer in use.
	 * For example, if a ResourceListen is not useful anymore the thread is closed and the instance removed.
	 * @param[in] p_endpoint Pointer to the Endpoint that is going to be removed.
	 * @param[in] type Char indicating if it is Reader ('R') or Writer ('W')
	 * @return True if correct.
	 */
	bool deleteUserEndpoint(Endpoint* p_endpoint,char type);


	std::vector<RTPSReader*>::iterator userReadersListBegin(){return m_userReaderList.begin();};

	std::vector<RTPSReader*>::iterator userReadersListEnd(){return m_userReaderList.end();};

	std::vector<RTPSWriter*>::iterator userWritersListBegin(){return m_userWriterList.begin();};

	std::vector<RTPSWriter*>::iterator userWritersListEnd(){return m_userWriterList.end();};


	//!Used for tests
	void loose_next_change(){m_send_thr.loose_next();};
	//! Announce ParticipantState (force the sending of a DPD message.)
	void announceParticipantState();
	//!Stop the Participant Announcement (used in tests to avoid multiple packets being send)
	void stopParticipantAnnouncement();
	//!Reset to timer to make periodic Participant Announcements.
	void resetParticipantAnnouncement();
	/**
	 * Get the GUID_t of the Participant.
	 * @return GUID_t of the Participant.
	 */
	const GUID_t& getGuid() const {
		return m_guid;
	}
	/**
	 * Get the Participant Name.
	 * @return String with the participant Name.
	 */
	const std::string& getParticipantName() const {
		return m_participantName;
	}

	//!Default listening addresses.
	LocatorList_t m_defaultUnicastLocatorList;
	//!Default listening addresses.
	LocatorList_t m_defaultMulticastLocatorList;

	void ResourceSemaphorePost();

	void ResourceSemaphoreWait();

	const DiscoveryAttributes& getDiscoveryAttributes() const {
		return m_discovery;
	}

	ResourceEvent* getEventResource()
	{
		return &m_event_thr;
	}

	uint32_t getParticipantId() const {
		return m_participantID;
	}

	void setParticipantId(uint32_t participantId) {
		m_participantID = participantId;
	}

private:
	//SimpleParticipantDiscoveryProtocol m_SPDP;
	const std::string m_participantName;
	//StaticEndpointDiscoveryProtocol m_StaticEDP;

	//!Guid of the participant.
	const GUID_t m_guid;

	//! Sending resources.
	ResourceSend m_send_thr;

	ResourceEvent m_event_thr;

	//!Semaphore to wait for the listen thread creation.
	boost::interprocess::interprocess_semaphore* mp_ResourceSemaphore;
	//!Id counter to correctly assign the ids to writers and readers.
	uint32_t IdCounter;
	//!Writer List.
	std::vector<RTPSWriter*> m_allWriterList;
	//!Reader List
	std::vector<RTPSReader*> m_allReaderList;
	//!Listen thread list.
	//!Writer List.
	std::vector<RTPSWriter*> m_userWriterList;
	//!Reader List
	std::vector<RTPSReader*> m_userReaderList;
	//!Listen thread list.
	//std::vector<ResourceListen*> m_threadListenList;

	std::vector<ListenResource*> m_listenResourceList;
	/*!
	 * Assign a given Endpoint to one of the current listen thread or create a new one.
	 * @param[in] endpoint Pointer to the Endpoint to add.
	 * @param[in] type Type of the Endpoint (R or W)(Reader or Writer).
	 * @param[in] isBuiltin Indicates if the endpoint is Builtin or not.
	 * @return True if correct.
	 */
	bool assignEnpointToListenResources(Endpoint* endpoint,char type,bool isBuiltin);
	/*!
	 * Create a new listen thread in the specified locator.
	 * @param[in] loc Locator to use.
	 * @param[out] listenthread Pointer to pointer of this class to correctly initialize the listening recourse.
	 * @param[in] isMulticast To indicate whether the new lsited thread is multicast.
	 * @param[in] isBuiltin Indicates that the endpoint is builtin.
	 * @return True if correct.
	 */
	bool addNewListenResource(Locator_t& loc,ResourceListen** listenthread,bool isMulticast,bool isBuiltin);

	ParticipantDiscoveryProtocol* mp_PDP;

	DiscoveryAttributes m_discovery;

	uint32_t m_participantID;



};
/**
 * @brief Class Participant, contains the public API for a Participant.
 * @ingroup MANAGEMENTMODULE
 */
class RTPS_DllAPI Participant
{
public:
	Participant(ParticipantImpl* pimpl):mp_impl(pimpl){};
	virtual ~ Participant(){};
	const GUID_t& getGuid(){return mp_impl->getGuid();};
	void announceParticipantState(){return mp_impl->announceParticipantState();};
	void loose_next_change(){return mp_impl->loose_next_change();};
	void stopParticipantAnnouncement(){return mp_impl->stopParticipantAnnouncement();};
	void resetParticipantAnnouncement(){return mp_impl->resetParticipantAnnouncement();};
	private:
ParticipantImpl* mp_impl;
};


} /* namespace rtps */
} /* namespace eprosima */

#endif /* PARTICIPANT_H_ */





