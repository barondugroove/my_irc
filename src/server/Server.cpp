/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 20:22:06 by rlaforge          #+#    #+#             */
/*   Updated: 2023/09/12 17:52:38 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include <iostream>
#include <unistd.h>
#include <limits>
#include <cstdlib>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <sys/epoll.h>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <csignal>
#include <netdb.h>

bool running = true;

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

void	exitProgram(int signal) {
	if (signal == SIGINT)
		running = false;
}

bool checkNickname(std::string &test) {
	const std::string forbiddenChars = " ,*?!@.";

	if (test.empty() || test[0] == '$' || test[0] == ':' || test[0] == '#' || test[0] == '&')
		return false;
	for (size_t i = 0; i < test.size() - 1; i++) {
		if (forbiddenChars.find(test[i]) != std::string::npos)
			return false;
	}
	return true;
}

void Server::sendMessage(const int &fd, std::string msg) {
	std::cout << "message to client fd[" << fd << "] : " << msg; 
	send(fd, msg.c_str(), msg.size(), 0);
}

void	Server::eraseChannel(std::map<std::string, Channel>::iterator it) {
	std::cout << "Channel " << it->second.getChannelName() << " has been closed since it has no more members." << std::endl;
	channels.erase(it);
}


void	Server::initEpoll(struct epoll_event &serverEvents) {
	// CREATE EPOLL FD
	_epoll_fd = epoll_create1(0);
	if (_epoll_fd == -1)
		throw Server::EpollCreationException();

	// ADDING SERVER FD TO EPOLL
	serverEvents.data.fd = _serverSocket;
	serverEvents.events = EPOLLIN | EPOLLET;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _serverSocket, &serverEvents) == -1)
		throw Server::EpollControlException();
}

void	Server::connectClient(struct epoll_event &serverEvents) {
	// CREATE CLIENT FD
	struct	sockaddr_in clientAddr;

	socklen_t addrlen = sizeof(clientAddr);
	int clientFd = -1;
	clientFd = accept(_serverSocket, (struct sockaddr *)&clientAddr, &addrlen);
	if (clientFd < 0)
		throw Server::AcceptException();
	fcntl(clientFd, F_SETFL, O_NONBLOCK); // A VERIFIER

	// ADD CLIENT FD TO EPOLL
	serverEvents.data.fd = clientFd;
	serverEvents.events = EPOLLIN | EPOLLET;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, clientFd, &serverEvents) == -1)
		throw Server::EpollControlException();

	// CREATE CLIENT OBJECT AND ADD TO LIST
	Client newClient(clientFd);
	this->clientsList.insert(std::pair<std::string, Client>("", newClient));
	std::cout << "New client connected" << std::endl;
}

void Server::handleClientMsg(Client &client, std::string msg) {

	msg.erase(msg.find("\r\n"));
	std::stringstream	message(msg);
	std::string			cmd;

	message >> cmd;
	if (cmd[0] == '/')
		cmd.erase(cmd.begin());

	std::map<std::string, void(Server::*)(Client&, std::stringstream &msg)>::iterator it = commandsChannels.find(cmd);
	std::cout << "Client " << client.getNickname() << " (" << client.getUsername() << ") fd[" << client.getUserFd() << "] : " << msg << std::endl;
	if (it == commandsChannels.end()) {
		sendMessage(client.getUserFd(), ERR_CMDNOTFOUND(client.getNickname()));
		return ;
	}
	if (!client.isAuth() && it->first != "PASS" && it->first != "USER"  && it->first != "NICK") {
		sendMessage(client.getUserFd(), ERR_NOTREGISTERED(client.getNickname()));
		return ;
	}
	else
		(this->*(it->second))(client, message);
	return ;
}

void	Server::liaiseClient(Client &client, int fd) {
	char msg[512];
	memset(&msg, 0, sizeof(msg));
	int bytes_received = recv(fd, msg, sizeof(msg), 0);

	// REMOVE CLIENT
	if (bytes_received <= 0) // == 0 = DISCONECTED // < 0 ERROR
	{
		std::cout << "Client disconnected" << std::endl;
		if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1)
			throw Server::EpollControlException();
		close(fd);
		this->clientsList.erase(client.getNickname()); // WEIRD IT++
	}
	else
		handleClientMsg(client, msg);
}

void	Server::run(int _serverSocket)
{
	struct epoll_event	serverEvents;

	initEpoll(serverEvents);
	signal(SIGINT, exitProgram);
	while (running)
	{
		struct epoll_event	clientsEvents[50];
		// CREATING CLIENTS EPOLL EVENT STRUCT
		int clientNbr = epoll_wait(_epoll_fd, clientsEvents, 50, -1);
		if (clientNbr == -1) {
			throw Server::EpollWaitException();
		}

		// ITERATE ON ALL CONNECTED CLIENTS FDS
		for (int i = 0; i < clientNbr; ++i)
		{
			int fd = clientsEvents[i].data.fd;
			// SETTING ITERATOR
			std::map<std::string, Client>::iterator it = this->clientsList.begin();
			while (it != this->clientsList.end())
			{
				if (it->second.getUserFd() == fd)
					break;
				it++;
			}
			if (fd == _serverSocket) // SERVER ACTIONS, NEW CONNECTIONS
				connectClient(serverEvents);
			else // CLIENT ACTIONS
				liaiseClient(it->second, fd);
		}
	}
}

Server::Server(unsigned short port, std::string password) : _port(port), _password(password)
{
	struct protoent	*proto;
	int				option = 1;

	proto = getprotobyname("tcp");
	_serverSocket = socket(AF_INET, SOCK_STREAM, proto->p_proto);
	if (_serverSocket < 0)
		throw Server::SocketErrorException();

	// SETTING SERVER ADDR
	struct	sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(_port);

	// BINDING SOCKET ADDR
	if (bind(_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
		throw Server::CantListenOnPortException();

	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
		throw Server::CantListenOnPortException();

	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEPORT, &option, sizeof(option)) < 0)
		throw Server::CantListenOnPortException();

	if (listen(_serverSocket, serverAddr.sin_port) < 0)
		throw Server::CantListenOnPortException();

	commandsChannels.insert(std::pair<std::string, void(Server::*)(Client&, std::stringstream &msg)>("JOIN", &Server::cmdJoin));
	commandsChannels.insert(std::pair<std::string, void(Server::*)(Client&, std::stringstream &msg)>("PART", &Server::cmdPart));
	commandsChannels.insert(std::pair<std::string, void(Server::*)(Client&, std::stringstream &msg)>("PRIVMSG", &Server::cmdPrivMsg));
	commandsChannels.insert(std::pair<std::string, void(Server::*)(Client&, std::stringstream &msg)>("KICK", &Server::cmdKick));
	commandsChannels.insert(std::pair<std::string, void(Server::*)(Client&, std::stringstream &msg)>("INVITE", &Server::cmdInvite));
	commandsChannels.insert(std::pair<std::string, void(Server::*)(Client&, std::stringstream &msg)>("TOPIC", &Server::cmdTopic));
	commandsChannels.insert(std::pair<std::string, void(Server::*)(Client&, std::stringstream &msg)>("MODE", &Server::cmdMode));
	commandsChannels.insert(std::pair<std::string, void(Server::*)(Client&, std::stringstream &msg)>("PASS", &Server::cmdPass));
	commandsChannels.insert(std::pair<std::string, void(Server::*)(Client&, std::stringstream &msg)>("NICK", &Server::cmdNick));
	commandsChannels.insert(std::pair<std::string, void(Server::*)(Client&, std::stringstream &msg)>("USER", &Server::cmdUser));

	commandsMode.insert(std::pair<std::string, void(Server::*)(Channel &channel, Client &client, std::stringstream &msg)>("i", &Server::modeI));
	commandsMode.insert(std::pair<std::string, void(Server::*)(Channel &channel, Client &client, std::stringstream &msg)>("t", &Server::modeT));
	commandsMode.insert(std::pair<std::string, void(Server::*)(Channel &channel, Client &client, std::stringstream &msg)>("k", &Server::modeK));
	commandsMode.insert(std::pair<std::string, void(Server::*)(Channel &channel, Client &client, std::stringstream &msg)>("o", &Server::modeO));
	commandsMode.insert(std::pair<std::string, void(Server::*)(Channel &channel, Client &client, std::stringstream &msg)>("l", &Server::modeL));


	std::cout << std::endl << "All good!" << std::endl << "port    : " << _port << std::endl << "password: " << _password << std::endl;
	run(_serverSocket);
}

Server::~Server(void) {
	return;
}
