/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file EDPStaticXML.h
 *
 */

#ifndef EDPSTATICXML_H_
#define EDPSTATICXML_H_

#include <set>
#include <vector>
#include <cstdint>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using boost::property_tree::ptree;

namespace eprosima {
namespace rtps {

class ReaderProxyData;
class WriterProxyData;


/**
 * Class StaticParticipantInfo, contains the ifnormation of writers and readers loaded from the XML file.
 * @ingroup DISCOVERYMODULE
 */
class StaticParticipantInfo{
public:
	StaticParticipantInfo(){};
	virtual ~StaticParticipantInfo(){};
	std::string m_participantName;
	std::vector<ReaderProxyData*> m_readers;
	std::vector<WriterProxyData*> m_writers;
};

/**
 * Class EDPStaticXML used to parse the XML file that contains information about remote endpoints.
 * @ingroup DISCVOERYMODULE
 */
class EDPStaticXML {
public:
	EDPStaticXML();
	virtual ~EDPStaticXML();
	/**
	 * Load the XML file
	 * @param filename Name of the file to load and parse.
	 * @return True if correct.
	 */
	bool loadXMLFile(std::string& filename);
	/**
	 * Load a Reader endpoint.
	 * @param xml_endpoint Reference of a tree child for a reader.
	 * @param pdata Pointer to the participantInfo where the reader must be added.
	 * @return True if correctly added.
	 */
	bool loadXMLReaderEndpoint(ptree::value_type& xml_endpoint,StaticParticipantInfo* pdata);
	/**
	 * Load a Writer endpoint.
	 * @param xml_endpoint Reference of a tree child for a writer.
	 * @param pdata Pointer to the participantInfo where the reader must be added.
	 * @return True if correctly added.
	 */
	bool loadXMLWriterEndpoint(ptree::value_type& xml_endpoint,StaticParticipantInfo* pdata);
	/**
	 * Look for a reader in the previously loaded endpoints.
	 * @param[in] partname Participant name
	 * @param[in] id Id of the reader
	 * @param[out] rdataptr Pointer to pointer to return the information.
	 * @return True if found.
	 */
	bool lookforReader(std::string partname,uint16_t id,ReaderProxyData** rdataptr);
	/**
	 * Look for a writer in the previously loaded endpoints.
	 * @param[in] partname Participant name
	 * @param[in] id Id of the writer
	 * @param[out] wdataptr Pointer to pointer to return the information.
	 * @return
	 */
	bool lookforWriter(std::string partname,uint16_t id,WriterProxyData** wdataptr);

private:
	std::set<int16_t> m_endpointIds;

	std::vector<StaticParticipantInfo*> m_participants;
};

} /* namespace rtps */
} /* namespace eprosima */

#endif /* EDPSTATICXML_H_ */
