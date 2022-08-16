#pragma once
#ifndef CHAT_HANDLER_H
#define CHAT_HANDLER_H
#include "chat_structs.h"
#include <forward_list>
#include <map>
#include <mutex>
#include <unordered_map>

namespace CHATSERVER {
	namespace CHAT_PROTOCOL {
		class Chat_Handler
		{
			std::forward_list<CHATSERVER::CUSTOM_STRUCTS::Person> people;
			std::map<group_id, CHATSERVER::CUSTOM_STRUCTS::Group> groups;
			std::unordered_map<ip, CHATSERVER::CUSTOM_STRUCTS::Channel &> channels;
			std::mutex m;
			unsigned int next_id = 0;
			unsigned int guest_num = 0;
		public:
			void operator() ();
			void register_channel(const std::string & client_ip, CHATSERVER::CUSTOM_STRUCTS::Channel & channel);
			~Chat_Handler();

		};

	}

}
#endif