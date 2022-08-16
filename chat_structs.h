#pragma once
#ifndef CHAT_STRUCT_H
#define CHAT_STRUCT_H
#include <string>
#include <thread>
#include <vector>
#include "queue.h"

using utility_queue = CHATSERVER::UTILITY::lock_free_queue<std::string>;
using username = std::string;
using ip = std::string;
using group_id = unsigned int;

namespace CHATSERVER {
	namespace CUSTOM_STRUCTS {
		struct Channel {
			utility_queue income_chathandler;
			utility_queue outgoing_chathandler;
		};

		struct Person {
			username name;
			ip myip;
			std::vector<group_id> group_ids;
		};

		struct Group {
			std::vector<Person> people;
			group_id id;
		};
	}
}
#endif