// Copyright 2025 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

#ifndef FASTDDS_XTYPES_DYNAMIC_TYPES_IDL_PARSER_CUSTOMRULES_HPP
#define FASTDDS_XTYPES_DYNAMIC_TYPES_IDL_PARSER_CUSTOMRULES_HPP

#include "pegtl.hpp"

#include <fastdds/dds/log/Log.hpp>

#include "IdlParserContext.hpp"

namespace eprosima {
namespace fastdds {
namespace dds {
namespace idlparser {

using namespace tao::TAO_PEGTL_NAMESPACE;

struct context_checker
{
    using analyze_t = analysis::generic< analysis::rule_type::opt >;

    template< tao::pegtl::apply_mode A,
             tao::pegtl::rewind_mode M,
             template< typename... > class Action,
             template< typename... > class Control,
             typename ParseInput>
    static bool match(
            ParseInput& in,
            Context* ctx,
            std::map<std::string, std::string>& /*state*/,
            std::vector<traits<DynamicData>::ref_type>& /*operands*/)
    {
        if (ctx)
        {
            if (!ctx->should_continue)
            {
                EPROSIMA_LOG_INFO(IDLPARSER, "Stopping parsing due to context flag.");
                // Consume the entire input to avoid further parsing
                in.bump(in.size());
            }
        }

        return true;
    }
};

template< typename Rule >
struct check_context_after : seq< Rule, context_checker > {};

} // namespace idlparser
} // namespace dds
} // namespace fastdds
} // namespace eprosima

#endif // FASTDDS_XTYPES_DYNAMIC_TYPES_IDL_PARSER_CUSTOMRULES_HPP