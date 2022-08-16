#include "chat_handle.h"

#include <algorithm>
#include <chrono>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>

using json = nlohmann::json;
using person = CHATSERVER::CUSTOM_STRUCTS::Person;

void CHATSERVER::CHAT_PROTOCOL::Chat_Handler::operator() ()
{/*
	using namespace std::chrono_literals;
	// check if any anybody exist
	for (auto & person : people)
	{
		std::this_thread::sleep_for(2s);
		std::lock_guard<std::mutex> guard(m);
		auto message = channels[person.myip].income_chathandler.pop();

		if (message)
		{
			auto j = json::parse(*message);

			if (j["request"] == "CHANGE_USERNAME")
			{
				auto name = j["name"].get<std::string>();
				if (find_if(people.begin(), people.end(), [&name](auto & ele) { return name == ele.name; }) != people.end() )
				{
					person.name = name;
					json res;
					res["type"] = "response";
					res["status"] = "OK";
					channels[person.myip].outgoing_chathandler.push(res.dump());
				}
				else
				{
					json res;
					res["type"] = "status";
					res["response"] = "BAD";
					channels[person.myip].outgoing_chathandler.push(res.dump());
				}
			}
			else if (j["request"] == "CREATE_GROUP")
			{
				std::vector<CHATSERVER::CUSTOM_STRUCTS::Person> myfriends;
				auto friends = j["people"].get<std::vector<std::string>>();
				for (auto name : friends)
				{
					auto registered_friend = find_if(people.begin(), people.end(), [&name](auto & ele) { return name == ele.name; });
					if (registered_friend != people.end())
					{
						myfriends.push_back(*registered_friend);
					}
				}
				groups.emplace(next_id, CHATSERVER::CUSTOM_STRUCTS::Group{ myfriends, next_id });
				++next_id;

				json res;
				res["type"] = "response";
				res["status"] = "OK";
				channels[person.myip].outgoing_chathandler.push(res.dump());
			}
			else if (j["request"] == "LEAVE_GROUP")
			{

			}
			else if (j["request"] == "MESSAGE")
			{
				auto message = j["message"].get<std::string>();
				for (auto & myfriend : groups[person.group_ids[j["group_id"].get<unsigned int>()]].people)
				{
					json res;
					res["type"] = "notification";
					res["group_id"] = j["group_id"].get<unsigned int>();
					res["message"] = message;
					channels[myfriend.myip].outgoing_chathandler.push(res.dump());
				}
			}
			else // there was some error with request 
			{
			}
		}
	}*/
}

void CHATSERVER::CHAT_PROTOCOL::Chat_Handler::register_channel(const std::string & client_ip,  CHATSERVER::CUSTOM_STRUCTS::Channel & mychannel)
{
	std::lock_guard<std::mutex> guard(m);

	auto client = find_if(people.begin(), people.end(), [&client_ip](auto & ele) { return client_ip == ele.myip; });

	if (client == people.end())
	{
		person new_person;
		new_person.myip = client_ip;
		new_person.name = std::string("guest") + std::to_string(guest_num);
		channels.emplace(client_ip, mychannel);
		people.emplace_front(new_person);

		++guest_num;
	}
	else
	{

	}
}

CHATSERVER::CHAT_PROTOCOL::Chat_Handler::~Chat_Handler()
{
	json command;
	command["type"] = "internal";
	command["action"] = "DEAD";
	for (auto & kv : channels)
	{
		kv.second.outgoing_chathandler.push(command.dump());
	}
}