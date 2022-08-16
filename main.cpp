#include "server.h"
#include "chat_handle.h"
#include "client_handle.h"

using namespace CHATSERVER::CHAT_PROTOCOL;


auto main(void) -> int {
	return server_run<Chat_Handler, Client_Handler>("0.0.0.0", "12347");
}