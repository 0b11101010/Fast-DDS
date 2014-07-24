/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file StatelessWriter.h
 */


#ifndef STATELESSWRITER_H_
#define STATELESSWRITER_H_

#include "eprosimartps/common/types/Time_t.h"
#include "eprosimartps/writer/RTPSWriter.h"
#include "eprosimartps/writer/ReaderLocator.h"
#include "eprosimartps/dds/attributes/PublisherAttributes.h"

using namespace eprosima::dds;

namespace eprosima {
namespace rtps {

class ReaderProxyData;

/**
 * Class StatelessWriter, specialization of RTPSWriter that manages writers that don't keep state of the matched readers.
 * @ingroup WRITERMODULE
 */
class StatelessWriter : public RTPSWriter
{
public:
	//StatelessWriter();
	virtual ~StatelessWriter();
	StatelessWriter(const PublisherAttributes& wParam,
			const GuidPrefix_t&guidP, const EntityId_t& entId,DDSTopicDataType* ptype);

	/**
	 * Add a matched reader.
	 * @param rdata Pointer to the ReaderProxyData object added.
	 * @return True if added.
	 */
	bool matched_reader_add(ReaderProxyData* rdata);
	/**
	 * Remove a matched reader.
	 * @param rdata Pointer to the object to remove.
	 * @return True if removed.
	 */
	bool matched_reader_remove(ReaderProxyData* rdata);
	/**
	 * Add a ReaderLocator to the StatelessWriter.
	 * @param locator Locator to add
	 * @param expectsInlineQos Boolean variable indicating that the locator expects inline Qos.
	 * @return
	 */
	bool reader_locator_add(Locator_t& locator,bool expectsInlineQos);
	//!Reset the unsent changes.
	void unsent_changes_reset();
	/**
	 * Add a specific change to all ReaderLocators.
	 * @param p Pointer to the change.
	 */
	void unsent_change_add(CacheChange_t* p);
	/**
	 * Method to indicate that there are changes not sent in some of all ReaderLocator.
	 */
	void unsent_changes_not_empty();
	/**
	 * Remove the change with the minimum SequenceNumber
	 * @return True if removed.
	 */
	bool removeMinSeqCacheChange();
	/**
	 * Remove all changes from history
	 * @param n_removed Pointer to return the number of elements removed.
	 * @return True if correct.
	 */
	bool removeAllCacheChange(size_t* n_removed);
	//!Get the number of matched subscribers.
	size_t getMatchedSubscribers(){return reader_locator.size();}
	bool change_removed_by_history(CacheChange_t* a_change);

private:
	Duration_t resendDataPeriod; //FIXME: Not used yet.
	std::vector<ReaderLocator> reader_locator;
	std::vector<ReaderProxyData*> m_matched_readers;
	bool add_locator(ReaderProxyData* rdata,Locator_t& loc);
	bool remove_locator(Locator_t& loc);
};

} /* namespace rtps */
} /* namespace eprosima */

#endif /* STATELESSWRITER_H_ */
