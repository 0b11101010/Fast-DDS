/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file ParticipantDiscoveryProtocol.cpp
 *
 */

#include "eprosimartps/discovery/ParticipantDiscoveryProtocol.h"

namespace eprosima {
namespace rtps {

ParticipantDiscoveryProtocol::ParticipantDiscoveryProtocol(ParticipantImpl* p_part):
		mp_localDPData(NULL),
		mp_participant(p_part),
		mp_EDP(NULL)
{

	// TODO Auto-generated constructor stub

}

ParticipantDiscoveryProtocol::~ParticipantDiscoveryProtocol() {
	// TODO Auto-generated destructor stub
}




} /* namespace rtps */
} /* namespace eprosima */
