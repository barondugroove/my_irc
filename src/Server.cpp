/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 20:22:06 by rlaforge          #+#    #+#             */
/*   Updated: 2023/08/28 19:50:43 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.cpp"
#include <iostream>
#include <unistd.h>
#include <limits>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <sys/epoll.h>
#include <iterator>

const char* Server::PortNotNumberException::what() const throw() {
	return ("Port is not a number.");
}

const char* Server::PortOverflowException::what() const throw() {
	return ("Port number is out of range.");
}

const char* Server::SocketErrorException::what() const throw() {
	return ("Socket error.");
}

const char* Server::CantListenOnPortException::what() const throw() {
	return ("Can't listen on this port.");
}

const char* Server::AcceptException::what() const throw() {
	return ("Error on client acceptation.");
}

const char* Server::EpollCreationException::what() const throw() {
	return ("Error on epoll creation.");
}

const char* Server::EpollWaitException::what() const throw() {
	return ("Error on epoll wait.");
}

const char* Server::EpollControlException::what() const throw() {
	return ("Error on epoll control.");
}

bool checkNickname(std::string &test) {
	std::string forbiddenChars = " ,*?!@.";

	if (test.empty() || test[0] == '$' || test[0] == ':' || test[0] == '#' || test[0] == '&')
		return false;
	for (size_t i = 0; i < test.size() - 1; i++) {
		if (forbiddenChars.find(test[i]) != std::string::npos)
			return false;
	}
	return true;
}

void errorMessage(const int &fd, std::string msg) {
	send(fd, msg.c_str(), msg.size(), 0);
}

void Server::clientAuth(Client &unauthClient, char *msg) {
	std::string test = msg;
	test.erase(test.size() - 1);

	std::cout << "Client fd[" << unauthClient.getUserFd() << "] (not auth) sent : " << msg;
	if (!unauthClient.password)
	{
		if (strcmp(msg, this->password.c_str()) == 0)
		{
			unauthClient.password = true;
			send(unauthClient.getUserFd(), "You are connected successfully!\nPlease enter your nickname : ", 62, 0);
		}
		else
			errorMessage(unauthClient.getUserFd(), "Error. Password is wrong, please try again : ");

	}
	else if (unauthClient.password && !unauthClient.named)
	{
		if (!checkNickname(test)) {
			errorMessage(unauthClient.getUserFd(), "Error. Nickname is wrong, please try again : ");
			return ;
		}
		else {
			unauthClient.setNickname(test);
			unauthClient.named = true;
			send(unauthClient.getUserFd(), "Please enter your username : ", 29, 0);
		}
	}
	else if (unauthClient.password && unauthClient.named)
	{
		unauthClient.setUsername(test);
		unauthClient.authentified = true;
		send(unauthClient.getUserFd(), "You are logged successfully!\n", 29, 0);
	}
}

void Server::handleClientMsg(Client &client, char *msg) {
	std::string test = msg;
	test.erase(test.size() - 1);

	std::cout << "Client " << client.getNickname() << " (" << client.getUsername() << ") fd[" << client.getUserFd() << "] : " << msg;
	return ;
}

void Server::run(int serverFd)
{
	// CREATE EPOLL FD
	int epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
		throw Server::EpollCreationException();

	// ADDING SERVER FD TO EPOLL
	struct epoll_event serverEvents;
	serverEvents.data.fd = serverFd;
	serverEvents.events = EPOLLIN;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serverFd, &serverEvents) == -1)
		throw Server::EpollControlException();

	while (1)
	{
		// CREATING CLIENTS EPOLL EVENT STRUCT
		struct epoll_event clientsEvents[50];
		int clientNbr = epoll_wait(epoll_fd, clientsEvents, 50, -1);
		if (clientNbr == -1)
			throw Server::EpollWaitException();

		// ITERATE ON ALL CONNECTED CLIENTS FDS
		for (int i = 0; i < clientNbr; ++i)
		{
			int fd = clientsEvents[i].data.fd;

			// SETTING ITERATOR
			std::list<Client>::iterator it = this->clientsList.begin();
			while (it != this->clientsList.end())
			{
				if (it->getUserFd() == fd)
					break;
				it++;
			}

			if (fd == serverFd) // SERVER ACTIONS, NEW CONNECTIONS
			{
				// CREATE CLIENT FD
				struct	sockaddr_in clientAddr;
				socklen_t addrlen = sizeof(clientAddr);

				int clientFd = accept(serverFd, (struct sockaddr *)&clientAddr, &addrlen);
				if (clientFd < 0)
					throw Server::AcceptException();

				// ADD CLIENT FD TO EPOLL
				serverEvents.data.fd = clientFd;
				serverEvents.events = EPOLLIN;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientFd, &serverEvents) == -1)
					throw Server::EpollControlException();

				// CREATE CLIENT OBJECT AND ADD TO LIST
				Client *newClient = new Client(clientFd);
				this->clientsList.push_back(*newClient);

				send(clientFd, "Please enter the password : ", 28, 0);

				std::cout << "New client connected" << std::endl;
			}
			else // CLIENT ACTIONS
			{
				char msg[512];
				memset(&msg, 0, sizeof(msg));
				int bytes_received = recv(fd, msg, sizeof(msg), 0);

				// REMOVE CLIENT
				if (bytes_received <= 0)
				{
					std::cout << "Client disconnected" << std::endl;
					if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1)
						throw Server::EpollControlException();
					close(fd);
					this->clientsList.erase(it++); // WEIRD IT++
				}
				else if (!it->authentified)
					clientAuth(*it, msg);
				else
					handleClientMsg(*it, msg);
			}
		}
	}
}

Server::Server(const char *portStr, const char *password)
{
	// PORT CHECKING
	char* ptr;
	long int port = strtol(portStr, &ptr, 10);

	if (port > 65535 || port < 1023)
		throw Server::PortOverflowException();
	else if (*ptr == '\0')
		this->port = port; // Port is valid
	else
		throw Server::PortNotNumberException();

	// PASSWORD CHECKING
	this->password = password;
	this->password.append("\n");

	// CREATING SOCKET
	int	serverFd;

	serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFd < 0)
		throw Server::SocketErrorException();

	// SETTING SERVER ADDR
	struct	sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(port);

	// BINDING SOCKET ADDR
	if (bind(serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
		throw Server::CantListenOnPortException();

	if (listen(serverFd, serverAddr.sin_port) < 0)
		throw Server::CantListenOnPortException();

	std::cout << std::endl << "All good!" << std::endl << "port    : " << port << std::endl << "password: " << password << std::endl;
	run(serverFd);
}

Server::~Server(void) {
	return;
}
