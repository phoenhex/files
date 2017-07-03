import Foundation
import XPC

let connection = xpc_connection_create_mach_service("com.apple.speech.speechsynthesisd", nil, 0)
xpc_connection_set_event_handler(connection) { print("Message received: " + $0.description) }
xpc_connection_resume(connection);

var msg = xpc_dictionary_create(nil, nil, 0)
xpc_dictionary_set_int64(msg, "msg", 120)
xpc_dictionary_set_data(msg, "voiceSpec", "1337", 4)
xpc_dictionary_set_bool(msg, "allVoices", true)

let reply = xpc_connection_send_message_with_reply_sync(connection, msg);
