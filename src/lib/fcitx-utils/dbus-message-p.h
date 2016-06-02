/*
 * Copyright (C) 2016~2016 by CSSlayer
 * wengxt@gmail.com
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; see the file COPYING. If not,
 * see <http://www.gnu.org/licenses/>.
 */
#ifndef _FCITX_UTILS_DBUS_MESSAGE_P_H_
#define _FCITX_UTILS_DBUS_MESSAGE_P_H_

#include "dbus-message.h"
#include <systemd/sd-bus.h>

namespace fcitx {
namespace dbus {

class MessagePrivate {
public:
    MessagePrivate() : type(MessageType::Invalid), msg(nullptr) {}

    ~MessagePrivate() { sd_bus_message_unref(msg); }

    static Message fromSDBusMessage(sd_bus_message *sdmsg, bool ref = true) {
        Message message;
        message.d_ptr->msg = ref ? sd_bus_message_ref(sdmsg) : sdmsg;
        uint8_t type = 0;
        MessageType msgType = MessageType::Invalid;
        sd_bus_message_get_type(sdmsg, &type);
        switch (type) {
        case SD_BUS_MESSAGE_METHOD_CALL:
            msgType = MessageType::MethodCall;
            break;
        case SD_BUS_MESSAGE_METHOD_RETURN:
            msgType = MessageType::Reply;
            break;
        case SD_BUS_MESSAGE_METHOD_ERROR:
            msgType = MessageType::Error;
            break;
        case SD_BUS_MESSAGE_SIGNAL:
            msgType = MessageType::Signal;
            break;
        }

        message.d_ptr->type = msgType;

        return message;
    }

    MessageType type;
    sd_bus_message *msg;
};
}
}

#endif // _FCITX_UTILS_DBUS_MESSAGE_P_H_
