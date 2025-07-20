// Copyright 2020 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

#include "Host.hpp"

#include <fstream>
#include <string>

namespace eprosima {

Host::Host()
{
#ifdef HAVE_FIXED_HOST_ID
    if (!compute_machine_id(id_))
    {
        // Fallback to IP-based computation if machine-id fails
        EPROSIMA_LOG_WARNING(UTILS, "Failed to compute machine ID. Falling back to IP based ID");
        fastdds::rtps::LocatorList loc;
        fastrtps::rtps::IPFinder::getIP4Address(&loc);
        id_ = compute_id(loc);
    }
#else
    // Compute the host id
    fastdds::rtps::LocatorList loc;
    fastrtps::rtps::IPFinder::getIP4Address(&loc);
    id_ = compute_id(loc);
#endif // HAVE_FIXED_HOST_ID

    // Compute the MAC id
    std::vector<fastrtps::rtps::IPFinder::info_MAC> macs;
    if (fastrtps::rtps::IPFinder::getAllMACAddress(&macs) &&
            macs.size() > 0)
    {
        MD5 md5;
        for (auto& m : macs)
        {
            md5.update(m.address, sizeof(m.address));
        }
        md5.finalize();
        for (size_t i = 0, j = 0; i < sizeof(md5.digest); ++i, ++j)
        {
            if (j >= mac_id_length)
            {
                j = 0;
            }
            mac_id_.value[j] ^= md5.digest[i];
        }
    }
    else
    {
        EPROSIMA_LOG_WARNING(UTILS, "Cannot get MAC addresses. Failing back to IP based ID");
        for (size_t i = 0; i < mac_id_length; i += 2)
        {
            mac_id_.value[i] = (id_ >> 8);
            mac_id_.value[i + 1] = (id_ & 0xFF);
        }
    }
}

#ifdef HAVE_FIXED_HOST_ID
bool Host::compute_machine_id(uint16_t& id)
{
    // Compute machine id using /etc/machine-id
    const std::string machine_id_path = "/etc/machine-id";
    std::ifstream machine_id_file(machine_id_path);

    if (!machine_id_file.is_open())
    {
        EPROSIMA_LOG_WARNING(UTILS, "Cannot open /etc/machine-id file");
        return false;
    }

    std::string machine_id_str;
    std::getline(machine_id_file, machine_id_str);
    machine_id_file.close();

    if (machine_id_str.empty())
    {
        EPROSIMA_LOG_WARNING(UTILS, "Empty machine-id file: " << machine_id_path);
        return false;
    }

    // Verify we have a valid 32-character hex string
    if (machine_id_str.length() != 32)
    {
        EPROSIMA_LOG_WARNING(UTILS, "Invalid machine-id format. Expected 32 hex characters, got "
                            << machine_id_str.length());
        return false;
    }

    // Verify all characters are hex
    for (char c : machine_id_str)
    {
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')))
        {
            EPROSIMA_LOG_WARNING(UTILS, "Invalid machine-id format. Contains non-hex characters");
            return false;
        }
    }

    // Use first 4 hex characters (2 bytes) directly from machine-id string
    // std::string hex_substr = machine_id_str.substr(0, 4);
    // id = static_cast<uint16_t>(std::stoul(hex_substr, nullptr, 16));

    // Calculate MD5 hash of the machine-id string
    MD5 md5;
    md5.update(machine_id_str.c_str(), machine_id_str.length());
    md5.finalize();

    // // Use first two bytes of MD5 digest as uint16_t
    id = static_cast<uint16_t>((static_cast<uint32_t>(md5.digest[1]) << 8) | static_cast<uint32_t>(md5.digest[0]));

    return true;
}
#endif // HAVE_FIXED_HOST_ID

} // eprosima
