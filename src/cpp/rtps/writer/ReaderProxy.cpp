// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file ReaderProxy.cpp
 *
 */


#include <fastrtps/rtps/writer/ReaderProxy.h>
#include <fastrtps/rtps/writer/StatefulWriter.h>
#include <fastrtps/utils/TimeConversion.h>
#include <fastrtps/rtps/writer/timedevent/NackResponseDelay.h>
#include <fastrtps/rtps/writer/timedevent/NackSupressionDuration.h>
#include <fastrtps/rtps/writer/timedevent/InitialHeartbeat.h>
#include <fastrtps/log/Log.h>
#include <fastrtps/rtps/resources/AsyncWriterThread.h>

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/lock_guard.hpp>

#include <cassert>

using namespace eprosima::fastrtps::rtps;


ReaderProxy::ReaderProxy(RemoteReaderAttributes& rdata,const WriterTimes& times,StatefulWriter* SW) :
    m_att(rdata), mp_SFW(SW),
    mp_nackResponse(nullptr), mp_nackSupression(nullptr), mp_initialHeartbeat(nullptr), m_lastAcknackCount(0),
    mp_mutex(new boost::recursive_mutex()), lastNackfragCount_(0)
{
    mp_nackResponse = new NackResponseDelay(this,TimeConv::Time_t2MilliSecondsDouble(times.nackResponseDelay));
    mp_nackSupression = new NackSupressionDuration(this,TimeConv::Time_t2MilliSecondsDouble(times.nackSupressionDuration));
    mp_initialHeartbeat = new InitialHeartbeat(this, TimeConv::Time_t2MilliSecondsDouble(times.initialHeartbeatDelay));
    logInfo(RTPS_WRITER,"Reader Proxy created");
}


ReaderProxy::~ReaderProxy()
{
    if(mp_nackResponse != nullptr)
        delete(mp_nackResponse);
    if(mp_nackSupression != nullptr)
        delete(mp_nackSupression);
    if(mp_initialHeartbeat != nullptr)
        delete(mp_initialHeartbeat);
    delete(mp_mutex);
}

void ReaderProxy::destroy_timers()
{
    delete(mp_nackResponse);
    mp_nackResponse = nullptr;
    delete(mp_nackSupression);
    mp_nackSupression = nullptr;
    delete(mp_initialHeartbeat);
    mp_initialHeartbeat = nullptr;
}

void ReaderProxy::addChange(const ChangeForReader_t& change)
{
    boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);

    assert(change.getSequenceNumber() > changesFromRLowMark_);
    assert(m_changesForReader.rbegin() != m_changesForReader.rend() ?
            change.getSequenceNumber() > m_changesForReader.rbegin()->getSequenceNumber() :
            true);

    if(m_changesForReader.size() == 0 && change.getStatus() == ACKNOWLEDGED)
    {
        changesFromRLowMark_ = change.getSequenceNumber();
        return;
    }

    m_changesForReader.insert(change);
    if (change.getStatus() == UNSENT)
        AsyncWriterThread::wakeUp(mp_SFW);
}

size_t ReaderProxy::countChangesForReader() const
{
    boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);
    return m_changesForReader.size();
}

bool ReaderProxy::change_is_acked(const SequenceNumber_t& sequence_number)
{
    boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);

    if(sequence_number <= changesFromRLowMark_)
        return true;

    auto chit = m_changesForReader.find(ChangeForReader_t(sequence_number));
    assert(chit != m_changesForReader.end());

    return !chit->isRelevant() || chit->getStatus() == ACKNOWLEDGED;
}

//TODO Return value for what?
bool ReaderProxy::acked_changes_set(const SequenceNumber_t& seqNum)
{
    boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);

    if(seqNum > changesFromRLowMark_)
    {
        auto chit = m_changesForReader.find(seqNum);
        m_changesForReader.erase(m_changesForReader.begin(), chit);
        changesFromRLowMark_ = seqNum - 1;
    }

    return m_changesForReader.size() == 0;
}

bool ReaderProxy::requested_changes_set(std::vector<SequenceNumber_t>& seqNumSet)
{
    bool isSomeoneWasSetRequested = false;
    boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);

    for(std::vector<SequenceNumber_t>::iterator sit=seqNumSet.begin();sit!=seqNumSet.end();++sit)
    {
        auto chit = m_changesForReader.find(ChangeForReader_t(*sit));

        if(chit != m_changesForReader.end() && chit->isValid())
        {
            ChangeForReader_t newch(*chit);
            newch.setStatus(REQUESTED);
            newch.markAllFragmentsAsUnsent();

            auto hint = m_changesForReader.erase(chit);

            m_changesForReader.insert(hint, newch);

            isSomeoneWasSetRequested = true;
        }
    }

    if(isSomeoneWasSetRequested)
    {
        logInfo(RTPS_WRITER,"Requested Changes: " << seqNumSet);
    }

    return isSomeoneWasSetRequested;
}


std::vector<const ChangeForReader_t*> ReaderProxy::get_unsent_changes() const
{
    std::vector<const ChangeForReader_t*> unsent_changes;
    boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);

    for(auto &change_for_reader : m_changesForReader)
        if(change_for_reader.getStatus() == UNSENT)
            unsent_changes.push_back(const_cast<ChangeForReader_t*>(&change_for_reader));

    return unsent_changes;
}

std::vector<const ChangeForReader_t*> ReaderProxy::get_requested_changes() const
{
    std::vector<const ChangeForReader_t*> unsent_changes;
    boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);

    auto it = m_changesForReader.begin();
    for (; it!= m_changesForReader.end(); ++it)
        if(it->getStatus() == REQUESTED)
            unsent_changes.push_back(&(*it));

    return unsent_changes;
}

void ReaderProxy::set_change_to_status(const SequenceNumber_t& seq_num, ChangeForReaderStatus_t status)
{
    if(seq_num <= changesFromRLowMark_)
        return;

    auto it = m_changesForReader.find(ChangeForReader_t(seq_num));
    bool mustWakeUpAsyncThread = false;

    if(it != m_changesForReader.end())
    {
        if(status == ACKNOWLEDGED && it == m_changesForReader.begin())
        {
            m_changesForReader.erase(it);
            changesFromRLowMark_ = seq_num;
        }
        else
        {
            ChangeForReader_t newch(*it);
            newch.setStatus(status);
            if (status == UNSENT) mustWakeUpAsyncThread = true;
            auto hint = m_changesForReader.erase(it);
            m_changesForReader.insert(hint, newch);
        }
    }

    if (mustWakeUpAsyncThread)
        AsyncWriterThread::wakeUp(mp_SFW);
}

void ReaderProxy::mark_fragments_as_sent_for_change(const CacheChange_t* change, FragmentNumberSet_t fragments)
{

    bool mustWakeUpAsyncThread = false; 
    for (auto it = m_changesForReader.begin(); it!= m_changesForReader.end(); ++it)
    {
        if (it->getChange() == change){
            ChangeForReader_t newch(*it);
            newch.markFragmentsAsSent(fragments);
            if (newch.getUnsentFragments().isSetEmpty()) 
                newch.setStatus(UNDERWAY);
            else
                mustWakeUpAsyncThread = true;
            auto hint = m_changesForReader.erase(it);
            m_changesForReader.insert(hint, newch);
            break;
        }
    }

    if (mustWakeUpAsyncThread)
        AsyncWriterThread::wakeUp(mp_SFW);
}

void ReaderProxy::convert_status_on_all_changes(ChangeForReaderStatus_t previous, ChangeForReaderStatus_t next)
{
    boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);
    bool mustWakeUpAsyncThread = false; 

    auto it = m_changesForReader.begin();
    while(it != m_changesForReader.end())
    {
        if(it->getStatus() == previous)
        {
            if(next == ACKNOWLEDGED && it == m_changesForReader.begin())
            {
                changesFromRLowMark_ = it->getSequenceNumber();
                it = m_changesForReader.erase(it);
                continue;
            }
            else
            {
                ChangeForReader_t newch(*it);
                newch.setStatus(next);
                if (next == UNSENT && previous != UNSENT)
                    mustWakeUpAsyncThread = true;
                auto hint = m_changesForReader.erase(it);

                it = m_changesForReader.insert(hint, newch);
            }
        }

        ++it;
    }

    if (mustWakeUpAsyncThread)
        AsyncWriterThread::wakeUp(mp_SFW);
}

void ReaderProxy::setNotValid(CacheChange_t* change)
{
    boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);

    // Check sequence number is in the container, because it was not clean up.
    if(m_changesForReader.size() == 0 || change->sequenceNumber < m_changesForReader.begin()->getSequenceNumber())
        return;

    auto chit = m_changesForReader.find(ChangeForReader_t(change));

    // Element must be in the container. In other case, bug.
    assert(chit != m_changesForReader.end());

    if(chit == m_changesForReader.begin())
    {
        // if it is the first element, set state to unacknowledge because from now reader has to confirm
        // it will not be expecting it.
        ChangeForReader_t newch(*chit);
        newch.setStatus(UNACKNOWLEDGED);
        newch.notValid();

        auto hint = m_changesForReader.erase(chit);

        m_changesForReader.insert(hint, newch);
    }
    else
    {
        // In case its state is not ACKNOWLEDGED, set it to UNACKNOWLEDGE because from now reader has to confirm
        // it will not be expecting it.
        ChangeForReader_t newch(*chit);
        if (chit->getStatus() != ACKNOWLEDGED)
            newch.setStatus(UNACKNOWLEDGED);
        newch.notValid();

        auto hint = m_changesForReader.erase(chit);

        m_changesForReader.insert(hint, newch);
    }
}

bool ReaderProxy::thereIsUnacknowledged() const
{
    bool returnedValue = false;
    boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);

    for(auto it = m_changesForReader.begin(); it!=m_changesForReader.end(); ++it)
    {
        if(it->getStatus() == UNACKNOWLEDGED)
        {
            returnedValue = true;
            break;
        }
    }

    return returnedValue;
}

bool change_min(const ChangeForReader_t* ch1, const ChangeForReader_t* ch2)
{
    return ch1->getSequenceNumber() < ch2->getSequenceNumber();
}

bool change_min2(const ChangeForReader_t ch1, const ChangeForReader_t ch2)
{
    return ch1.getSequenceNumber() < ch2.getSequenceNumber();
}

bool ReaderProxy::minChange(std::vector<ChangeForReader_t*>* Changes,
        ChangeForReader_t* changeForReader)
{
    boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);
    *changeForReader = **std::min_element(Changes->begin(),Changes->end(),change_min);
    return true;
}

bool ReaderProxy::requested_fragment_set(SequenceNumber_t sequence_number, const FragmentNumberSet_t& frag_set)
{
    boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);

    // Locate the outbound change referenced by the NACK_FRAG
    auto changeIter = std::find_if(m_changesForReader.begin(), m_changesForReader.end(), 
            [sequence_number](const ChangeForReader_t& change)
            {return change.getSequenceNumber() == sequence_number;});
    if (changeIter == m_changesForReader.end())
        return false;

    ChangeForReader_t newch(*changeIter);
    auto hint = m_changesForReader.erase(changeIter);
    newch.markFragmentsAsUnsent(frag_set);

    // If it was UNSENT, we shouldn't switch back to REQUESTED to prevent stalling.
    if (newch.getStatus() != UNSENT)
        newch.setStatus(REQUESTED);
    m_changesForReader.insert(hint, newch);

    return true;
}
