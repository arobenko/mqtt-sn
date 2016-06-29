//
// Copyright 2016 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <type_traits>
#include <functional>
#include <cassert>

#include "cc_plugin/protocol/field.h"
#include "Subscribe.h"

namespace cc = comms_champion;

namespace mqttsn
{

namespace cc_plugin
{

namespace protocol
{

namespace message
{

namespace
{

QVariantMap createProps_topicIdOpt()
{
    return
        cc::property::field::Optional()
            .name("TopicId")
            .field(field::createProps_topicId())
            .uncheckable()
            .asMap();
}

QVariantMap createProps_topicNameOpt()
{
    return
        cc::property::field::Optional()
            .name("TopicName")
            .field(field::createProps_topicName())
            .uncheckable()
            .asMap();
}

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(field::createProps_flags());
    props.append(field::createProps_msgId());
    props.append(createProps_topicIdOpt());
    props.append(createProps_topicNameOpt());

    assert(props.size() == Subscribe::FieldIdx_numOfValues);
    return props;
}

}  // namespace

const char* Subscribe::nameImpl() const
{
    static const char* Str = "SUBSCRIBE";
    return Str;
}

const QVariantList& Subscribe::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}

}  // namespace message

}  // namespace protocol

}  // namespace cc_plugin

}  // namespace mqttsn

