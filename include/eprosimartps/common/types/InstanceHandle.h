/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file InstanceHandle.h 	
 */

#ifndef INSTANCEHANDLE_H_
#define INSTANCEHANDLE_H_

#include "eprosimartps/common/types/common_types.h"
#include "eprosimartps/common/types/Guid.h"

namespace eprosima{
namespace rtps{

/**
 * Struct InstanceHandle_t, used to contain the key for WITH_KEY topics.
 */
struct InstanceHandle_t{
	octet value[16];
	InstanceHandle_t()
	{
		for(uint8_t i=0;i<16;i++)
			value[i] = 0;
	}
	InstanceHandle_t& operator=(const InstanceHandle_t& ihandle){

		for(uint8_t i =0;i<16;i++)
		{
			value[i] = ihandle.value[i];
		}
		return *this;
	}
	InstanceHandle_t& operator=(const GUID_t& guid)
	{
		for(uint8_t i =0;i<16;i++)
		{
			if(i<12)
				value[i] = guid.guidPrefix.value[i];
			else
				value[i] = guid.entityId.value[i-12];
		}
		return *this;
	}

	bool isDefined()
	{
		for(uint8_t i=0;i<16;++i)
		{
			if(value[i]!=0)
				return true;
		}
		return false;
	}
};

inline bool operator==(const InstanceHandle_t & ihandle1, const InstanceHandle_t& ihandle2)
{
	for(uint8_t i =0;i<16;++i)
	{
		if(ihandle1.value[i] != ihandle2.value[i])
			return false;
	}
	return true;
}

inline void iHandle2GUID(GUID_t& guid,const InstanceHandle_t& ihandle)
{
	for(uint8_t i = 0;i<16;++i)
	{
		if(i<12)
			guid.guidPrefix.value[i] = ihandle.value[i];
		else
			guid.entityId.value[i-12] = ihandle.value[i];
	}
	return;
}

inline GUID_t iHandle2GUID(const InstanceHandle_t& ihandle)
{
	GUID_t guid;
	for(uint8_t i = 0;i<16;++i)
	{
		if(i<12)
			guid.guidPrefix.value[i] = ihandle.value[i];
		else
			guid.entityId.value[i-12] = ihandle.value[i];
	}
	return guid;
}

inline std::ostream& operator<<(std::ostream& output,const InstanceHandle_t& iHandle)
{
	output << std::hex;
	for(uint8_t i =0;i<16;++i)
		output << std::hex << (int)iHandle.value[i] <<std::dec<< ".";
	output<<std::dec;
	return output;
}

}
}

#endif /* INSTANCEHANDLE_H_ */
