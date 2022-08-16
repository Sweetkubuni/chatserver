#include "client_handle.h"
#include "chat_structs.h"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <exception>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace websocket = boost::beast::websocket;  // from <boost/beast/websocket.hpp>


void CHATSERVER::CHAT_PROTOCOL::Client_Handler::operator()  (tcp::socket && socket)
{

	try
	{
		// Construct the stream by moving in the socket
		websocket::stream<tcp::socket> ws{ std::move(socket) };

		// Accept the websocket handshake
		ws.accept();

		handler.register_channel(socket.remote_endpoint().address().to_string(), mychannel);

		for (;;)
		{
			std::string rev;
			auto b = boost::asio::dynamic_buffer(rev);

			// Read a message
			ws.read(b);

			mychannel.income_chathandler.push(rev);

			auto content = mychannel.outgoing_chathandler.pop();
			auto j = json::parse(*content);
			if (j["type"] == "internal")
			{
				if (j["action"] == "DEAD")
				{
					return;
				}
			}
			else
			{
				auto respbuf = boost::asio::dynamic_buffer(*content);
				ws.text(true);
				ws.write(respbuf.data());
			}
		}
	}
	catch (boost::system::system_error const& se)
	{
		// This indicates that the session was closed
		if (se.code() != websocket::error::closed)
			std::cerr << "Error: " << se.code().message() << std::endl;
	}
	catch (std::exception const& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}
