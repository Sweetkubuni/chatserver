#pragma once
#ifndef CLIENT_HANDLE_H
#define CLIENT_HANDLE_H
#include "chat_handle.h"
#include <boost/asio/ip/tcp.hpp>
#include <mutex>

using tcp = boost::asio::ip::tcp;
namespace CHATSERVER {
	namespace CHAT_PROTOCOL {
		class Client_Handler
		{
			CHATSERVER::CUSTOM_STRUCTS::Channel mychannel;
			CHATSERVER::CHAT_PROTOCOL::Chat_Handler & handler;
		public:
			Client_Handler(Chat_Handler & handler) :handler(handler) { }
			void operator() (tcp::socket && socket);
		};

	}

}
#endif