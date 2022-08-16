#pragma once
#ifndef SERVER_H
#define SERVER_H
#include <boost/asio/ip/tcp.hpp>
#include "scoped_thread.h"
#include <functional>
#include <iostream>
#include <exception>

using tcp = boost::asio::ip::tcp;

template<class P, class C>
int server_run(const std::string & Addr, const std::string & Port)
{
	try
	{
		auto const address = boost::asio::ip::make_address(Addr);
		auto const port = static_cast<unsigned short>(std::atoi(Port.c_str()));

		// The io_context is required for all I/O
		boost::asio::io_context ioc{ 1 };

		P proc;

		scoped_thread scoped_t{ std::thread([&proc] { proc(); }) };

		// The acceptor receives incoming connections
		tcp::acceptor acceptor{ ioc,{ address, port } };
		for (;;)
		{
			// This will receive the new connection
			tcp::socket socket{ ioc };

			// Block until we get a connection
			acceptor.accept(socket);

			C c(proc);

			// Launch the session, transferring ownership of the socket
			std::thread{ [&c, &socket] { c(std::move(socket)); } }.detach();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}
#endif