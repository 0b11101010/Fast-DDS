/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file ReaderProxyData.cpp
 *
 */

#include "eprosimartps/writer/ReaderProxyData.h"

#include "eprosimartps/common/types/CDRMessage_t.h"

#include "eprosimartps/utils/RTPSLog.h"

namespace eprosima {
namespace rtps {

ReaderProxyData::ReaderProxyData():
											m_expectsInlineQos(false),
											m_userDefinedId(0),
											m_isAlive(false),
											m_topicKind(NO_KEY)

{

}

ReaderProxyData::~ReaderProxyData()
{
	m_parameterList.deleteParams();
}

bool ReaderProxyData::toParameterList()
{
	m_parameterList.deleteParams();
	for(LocatorListIterator lit = m_unicastLocatorList.begin();
			lit!=m_unicastLocatorList.end();++lit)
	{
		ParameterLocator_t* p = new ParameterLocator_t(PID_UNICAST_LOCATOR,PARAMETER_LOCATOR_LENGTH,*lit);
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	for(LocatorListIterator lit = m_multicastLocatorList.begin();
			lit!=m_multicastLocatorList.end();++lit)
	{
		ParameterLocator_t* p = new ParameterLocator_t(PID_MULTICAST_LOCATOR,PARAMETER_LOCATOR_LENGTH,*lit);
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	{
		ParameterBool_t * p = new ParameterBool_t(PID_EXPECTS_INLINE_QOS,PARAMETER_BOOL_LENGTH,m_expectsInlineQos);
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	{
		ParameterGuid_t* p = new ParameterGuid_t(PID_PARTICIPANT_GUID,PARAMETER_GUID_LENGTH,m_participantKey);
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	{
		ParameterString_t * p = new ParameterString_t(PID_TOPIC_NAME,0,m_topicName);
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	{
		ParameterString_t * p = new ParameterString_t(PID_TYPE_NAME,0,m_typeName);
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	{
		ParameterKey_t * p = new ParameterKey_t(PID_KEY_HASH,16,m_key);
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	{
		ParameterGuid_t * p = new ParameterGuid_t(PID_ENDPOINT_GUID,16,m_guid);
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	if(m_qos.m_durability.sendAlways() || m_qos.m_durability.hasChanged)
	{
		DurabilityQosPolicy*p = new DurabilityQosPolicy();
		*p = m_qos.m_durability;
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	if(m_qos.m_durabilityService.sendAlways() || m_qos.m_durabilityService.hasChanged)
	{
		DurabilityServiceQosPolicy*p = new DurabilityServiceQosPolicy();
		*p = m_qos.m_durabilityService;
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	if(m_qos.m_deadline.sendAlways() || m_qos.m_deadline.hasChanged)
	{
		DeadlineQosPolicy*p = new DeadlineQosPolicy();
		*p = m_qos.m_deadline;
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	if(m_qos.m_latencyBudget.sendAlways() || m_qos.m_latencyBudget.hasChanged)
	{
		LatencyBudgetQosPolicy*p = new LatencyBudgetQosPolicy();
		*p = m_qos.m_latencyBudget;
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	if(m_qos.m_liveliness.sendAlways() || m_qos.m_liveliness.hasChanged)
	{
		LivelinessQosPolicy*p = new LivelinessQosPolicy();
		*p = m_qos.m_liveliness;
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	if(m_qos.m_reliability.sendAlways() || m_qos.m_reliability.hasChanged)
	{
		ReliabilityQosPolicy*p = new ReliabilityQosPolicy();
		*p = m_qos.m_reliability;
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	if(m_qos.m_lifespan.sendAlways() || m_qos.m_lifespan.hasChanged)
	{
		LifespanQosPolicy*p = new LifespanQosPolicy();
		*p = m_qos.m_lifespan;
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	if(m_qos.m_userData.sendAlways() || m_qos.m_userData.hasChanged)
	{
		UserDataQosPolicy*p = new UserDataQosPolicy();
		*p = m_qos.m_userData;
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	if(m_qos.m_timeBasedFilter.sendAlways() || m_qos.m_timeBasedFilter.hasChanged)
	{
		TimeBasedFilterQosPolicy*p = new TimeBasedFilterQosPolicy();
		*p = m_qos.m_timeBasedFilter;
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	if(m_qos.m_ownership.sendAlways() || m_qos.m_ownership.hasChanged)
	{
		OwnershipQosPolicy*p = new OwnershipQosPolicy();
		*p = m_qos.m_ownership;
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	if(m_qos.m_destinationOrder.sendAlways() || m_qos.m_destinationOrder.hasChanged)
	{
		DestinationOrderQosPolicy*p = new DestinationOrderQosPolicy();
		*p = m_qos.m_destinationOrder;
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	if(m_qos.m_presentation.sendAlways() || m_qos.m_presentation.hasChanged)
	{
		PresentationQosPolicy*p = new PresentationQosPolicy();
		*p = m_qos.m_presentation;
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	if(m_qos.m_partition.sendAlways() || m_qos.m_partition.hasChanged)
	{
		PartitionQosPolicy*p = new PartitionQosPolicy();
		*p = m_qos.m_partition;
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	if(m_qos.m_topicData.sendAlways() || m_qos.m_topicData.hasChanged)
	{
		TopicDataQosPolicy*p = new TopicDataQosPolicy();
		*p = m_qos.m_topicData;
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	if(m_qos.m_groupData.sendAlways() || m_qos.m_groupData.hasChanged)
	{
		GroupDataQosPolicy*p = new GroupDataQosPolicy();
		*p = m_qos.m_groupData;
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	if(m_qos.m_timeBasedFilter.sendAlways() || m_qos.m_timeBasedFilter.hasChanged)
	{
		TimeBasedFilterQosPolicy*p = new TimeBasedFilterQosPolicy();
		*p = m_qos.m_timeBasedFilter;
		m_parameterList.m_parameters.push_back((Parameter_t*)p);
	}
	pDebugInfo(RTPS_CYAN << "DiscoveredReaderData converted to ParameterList with " << m_parameterList.m_parameters.size()<< " parameters"<<endl);
	return true;
}

bool ReaderProxyData::readFromCDRMessage(CDRMessage_t* msg)
{
	if(ParameterList::readParameterListfromCDRMsg(msg,&m_parameterList,NULL,NULL)>0)
	{
		for(std::vector<Parameter_t*>::iterator it = m_parameterList.m_parameters.begin();
				it!=m_parameterList.m_parameters.end();++it)
		{
			switch((*it)->Pid)
			{
			case PID_DURABILITY:
			{
				DurabilityQosPolicy * p = (DurabilityQosPolicy*)(*it);
				m_qos.m_durability = *p;
				break;
			}
			case PID_DURABILITY_SERVICE:
			{
				DurabilityServiceQosPolicy * p = (DurabilityServiceQosPolicy*)(*it);
				m_qos.m_durabilityService = *p;
				break;
			}
			case PID_DEADLINE:
			{
				DeadlineQosPolicy * p = (DeadlineQosPolicy*)(*it);
				m_qos.m_deadline = *p;
				break;
			}
			case PID_LATENCY_BUDGET:
			{
				LatencyBudgetQosPolicy * p = (LatencyBudgetQosPolicy*)(*it);
				m_qos.m_latencyBudget = *p;
				break;
			}
			case PID_LIVELINESS:
			{
				LivelinessQosPolicy * p = (LivelinessQosPolicy*)(*it);
				m_qos.m_liveliness = *p;
				break;
			}
			case PID_RELIABILITY:
			{
				ReliabilityQosPolicy * p = (ReliabilityQosPolicy*)(*it);
				m_qos.m_reliability = *p;
				break;
			}
			case PID_LIFESPAN:
			{

				LifespanQosPolicy * p = (LifespanQosPolicy*)(*it);
				m_qos.m_lifespan = *p;
				break;
			}
			case PID_USER_DATA:
			{
				UserDataQosPolicy * p = (UserDataQosPolicy*)(*it);
				m_qos.m_userData = *p;
				break;
			}
			case PID_TIME_BASED_FILTER:
			{
				TimeBasedFilterQosPolicy * p = (TimeBasedFilterQosPolicy*)(*it);
				m_qos.m_timeBasedFilter = *p;
				break;
			}
			case PID_OWNERSHIP:
			{
				OwnershipQosPolicy * p = (OwnershipQosPolicy*)(*it);
				m_qos.m_ownership = *p;
				break;
			}
			case PID_DESTINATION_ORDER:
			{
				DestinationOrderQosPolicy * p = (DestinationOrderQosPolicy*)(*it);
				m_qos.m_destinationOrder = *p;
				break;
			}

			case PID_PRESENTATION:
			{
				PresentationQosPolicy * p = (PresentationQosPolicy*)(*it);
				m_qos.m_presentation = *p;
				break;
			}
			case PID_PARTITION:
			{
				PartitionQosPolicy * p = (PartitionQosPolicy*)(*it);
				m_qos.m_partition = *p;
				break;
			}
			case PID_TOPIC_DATA:
			{
				TopicDataQosPolicy * p = (TopicDataQosPolicy*)(*it);
				m_qos.m_topicData = *p;
				break;
			}
			case PID_GROUP_DATA:
			{

				GroupDataQosPolicy * p = (GroupDataQosPolicy*)(*it);
				m_qos.m_groupData = *p;
				break;
			}
			case PID_TOPIC_NAME:
			{
				ParameterString_t*p = (ParameterString_t*)(*it);
				m_topicName = p->m_string;
				break;
			}
			case PID_TYPE_NAME:
			{
				ParameterString_t*p = (ParameterString_t*)(*it);
				m_typeName = p->m_string;
				break;
			}
			case PID_PARTICIPANT_GUID:
			{
				ParameterGuid_t * p = (ParameterGuid_t*)(*it);
				for(uint8_t i =0;i<16;++i)
				{
					if(i<12)
						m_participantKey.value[i] = p->guid.guidPrefix.value[i];
					else
						m_participantKey.value[i] = p->guid.entityId.value[i];
				}
				break;
			}
			case PID_ENDPOINT_GUID:
			{
				ParameterGuid_t * p = (ParameterGuid_t*)(*it);
				m_guid = p->guid;
				for(uint8_t i=0;i<16;++i)
				{
					if(i<12)
						m_key.value[i] = p->guid.guidPrefix.value[i];
					else
						m_key.value[i] = p->guid.entityId.value[i-12];
				}
				break;
			}
			case PID_UNICAST_LOCATOR:
			{
				ParameterLocator_t* p = (ParameterLocator_t*)(*it);
				m_unicastLocatorList.push_back(p->locator);
				break;
			}
			case PID_MULTICAST_LOCATOR:
			{
				ParameterLocator_t* p = (ParameterLocator_t*)(*it);
				m_multicastLocatorList.push_back(p->locator);
				break;
			}
			case PID_EXPECTS_INLINE_QOS:
			{
				ParameterBool_t*p =(ParameterBool_t*)(*it);
				m_expectsInlineQos = p->value;
				break;
			}
			case PID_KEY_HASH:
			{
				ParameterKey_t*p=(ParameterKey_t*)(*it);
				m_key = p->key;
				iHandle2GUID(m_guid,m_key);
				break;
			}
			default:
			{
				pInfo("Parameter with ID: "  <<(uint16_t)(*it)->Pid << " NOT CONSIDERED"<< endl);
				break;
			}
			}
		}
		if(m_guid.entityId.value[3] == 0x04)
			m_topicKind = NO_KEY;
		else if(m_guid.entityId.value[3] == 0x07)
			m_topicKind = WITH_KEY;

		return true;
	}
	return false;
}

void ReaderProxyData::clear()
{
	m_expectsInlineQos = false;
	m_guid = c_Guid_Unknown;
	m_unicastLocatorList.clear();
	m_multicastLocatorList.clear();
	m_key = InstanceHandle_t();
	m_participantKey = InstanceHandle_t();
	m_typeName = "";
	m_topicName = "";
	m_userDefinedId = 0;
	m_qos = ReaderQos();
	m_isAlive = false;
	m_topicKind = NO_KEY;


	m_parameterList.deleteParams();
	m_parameterList.resetList();
}

void ReaderProxyData::update(ReaderProxyData* rdata)
{
	m_unicastLocatorList = rdata->m_unicastLocatorList;
	m_multicastLocatorList = rdata->m_multicastLocatorList;
	m_qos.setQos(rdata->m_qos,false);
	m_isAlive = rdata->m_isAlive;
}

void ReaderProxyData::copy(ReaderProxyData* rdata)
{
	m_guid = rdata->m_guid;
	m_unicastLocatorList = rdata->m_unicastLocatorList;
	m_multicastLocatorList = rdata->m_multicastLocatorList;
	m_key = rdata->m_key;
	m_participantKey = rdata->m_participantKey;
	m_typeName = rdata->m_typeName;
	m_topicName = rdata->m_topicName;
	m_userDefinedId = rdata->m_userDefinedId;
	m_qos = rdata->m_qos;
	m_expectsInlineQos = rdata->m_expectsInlineQos;
	m_isAlive = rdata->m_isAlive;
	m_topicKind = rdata->m_topicKind;
}


} /* namespace rtps */
} /* namespace eprosima */


