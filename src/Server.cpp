/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 20:22:06 by rlaforge          #+#    #+#             */
/*   Updated: 2023/08/31 17:31:03 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.cpp"
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
	std::string forbiddenChars = " ,*?!@.";

	if (test.empty() || test[0] == '$' || test[0] == ':' || test[0] == '#' || test[0] == '&')
		return false;
	for (size_t i = 0; i < test.size() - 1; i++) {
		if (forbiddenChars.find(test[i]) != std::string::npos)
			return false;
	}
	return true;
}

void sendMessage(const int &fd, std::string msg) {
	send(fd, msg.c_str(), msg.size(), 0);
}

void			Server::eraseChannel(std::map<std::string, Channel*>::iterator it) {
	std::cout << "Channel " << it->second->getChannelName() << " has been closed since it has no more members." << std::endl;
	channels.erase(it);
}

void Server::clientAuth(Client &unauthClient, char *msg) {
	std::string test = msg;
	test.erase(test.size() - 1);

	std::cout << "Client fd[" << unauthClient.getUserFd() << "] (not auth) sent : " << msg;
	if (!unauthClient.password)
	{
		if (test == _password)
		{
			unauthClient.password = true;
			sendMessage(unauthClient.getUserFd(), "You are connected successfully!\nPlease enter your nickname : ");
		}
		else
			sendMessage(unauthClient.getUserFd(), "Error. Password is wrong, please try again : ");
	}
	else if (unauthClient.password && !unauthClient.named)
	{
		if (!checkNickname(test)) {
			sendMessage(unauthClient.getUserFd(), "Error. Nickname is wrong, please try again : ");
			return ;
		}
		else {
			unauthClient.setNickname(test);
			unauthClient.named = true;
			sendMessage(unauthClient.getUserFd(), "Please enter your username : ");
		}
	}
	else if (unauthClient.password && unauthClient.named)
	{
		unauthClient.setUsername(test);
		unauthClient.authentified = true;
		sendMessage(unauthClient.getUserFd(), "You are logged successfully!\n");
	}
}

void Server::cmdJoin(Client &client, std::stringstream &msg) {
	std::string			args;

	msg.ignore(512, ' ');
	msg >> args;

	if (!args.empty() && args[0] == '#') {
		std::map<std::string, Channel*>::iterator it = channels.find(args);
		if (it != channels.end()) {
			std::cout << client.getNickname() << " is joining channel " << args << std::endl;
			it->second->addUser(client.getNickname(), client);
		}
		else {
			std::cout << "New channel " << args << " created." << std::endl;
			Channel	newChannel(args, client);
			channels.insert(std::pair<std::string, Channel*>(args, &newChannel));
			std::map<std::string, Channel*>::iterator it = channels.find(args);
			it->second->addUser(client.getNickname(), client);
			it->second->setOperator(client.getNickname());
		}
	}
	else
		sendMessage(client.getUserFd(), "Cannot join channel " + args + '\n');
}

void Server::cmdPrivMsg(Client &client, std::stringstream &msg) {
	std::string			args;
	std::string			text;

	msg.ignore(512, ' ');
	msg >> args;
	msg >> text;

	if (!args.empty() && args[0] == '#') {
		std::map<std::string, Channel*>::iterator it = channels.find(args);
		if (it != channels.end()) {
			it->second->sendMessageToAllMembers(text += '\n');
		}
		else
			sendMessage(client.getUserFd(), "Cannot send message to channel " + args + '\n');
	}
	else {
		std::map<std::string, Client>::iterator it = clientsList.find(args);
		if (it != clientsList.end()) {
			sendMessage(it->second.getUserFd(), text);
		}
		else
			sendMessage(client.getUserFd(), "Cannot send message to user " + args + '\n');
	}
}

void Server::cmdPart(Client &client, std::stringstream &msg) {
	std::string			args;

	msg.ignore(512, ' ');
	msg >> args;

	if (!args.empty() && args[0] == '#') {
		std::map<std::string, Channel*>::iterator it = channels.find(args);
		if (it != channels.end() && it->second->isUserMember(client.getNickname())) {
			it->second->eraseUser(client.getNickname());
			if (it->second->getUserCount() == 0)
				eraseChannel(it);
		}
		else
			sendMessage(client.getUserFd(), "Cannot part channel " + args + " cause you are not a member.\n");
	}
	else
		sendMessage(client.getUserFd(), "Wrong PART args : " + args + '\n');
}

void Server::handleClientMsg(Client &client, char *msg) {
	std::stringstream	message(msg);
	std::string			cmd;

	message >> cmd;
	if (cmd[0] == '/')
		cmd.erase(cmd.begin());

	std::map<std::string, void(Server::*)(Client&, std::stringstream &msg)>::iterator it = commandsChannels.find(cmd);
	if (it != commandsChannels.end())
		(this->*(it->second))(client, message);
	else
		std::cout << "Client " << client.getNickname() << " (" << client.getUsername() << ") fd[" << client.getUserFd() << "] : " << msg;
	return ;
}

void	Server::initEpoll(struct epoll_event &serverEvents) {
	// CREATE EPOLL FD
	_epoll_fd = epoll_create1(0);
	if (_epoll_fd == -1)
		throw Server::EpollCreationException();

	// ADDING SERVER FD TO EPOLL
	serverEvents.data.fd = _serverSocket;
	serverEvents.events = EPOLLIN | EPOLLOUT;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _serverSocket, &serverEvents) == -1)
		throw Server::EpollControlException();
}

void	Server::connectClient(struct epoll_event &serverEvents) {
	// CREATE CLIENT FD
	struct	sockaddr_in clientAddr;

	socklen_t addrlen = sizeof(clientAddr);
	int clientFd = accept(_serverSocket, (struct sockaddr *)&clientAddr, &addrlen);
	if (clientFd < 0)
		throw Server::AcceptException();
	fcntl(clientFd, F_SETFL, O_NONBLOCK); // A VERIFIER

	// ADD CLIENT FD TO EPOLL
	serverEvents.data.fd = clientFd;
	serverEvents.events = EPOLLIN;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, clientFd, &serverEvents) == -1)
		throw Server::EpollControlException();

	// CREATE CLIENT OBJECT AND ADD TO LIST
	Client newClient(clientFd);
	this->clientsList.insert(std::pair<std::string, Client>("", newClient));
	send(clientFd, "Please enter the password : ", 28, 0);

	std::cout << "New client connected" << std::endl;
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
	else if (client.authentified)
		clientAuth(client, msg);
	else
		handleClientMsg(client, msg);
}

void	Server::run(int _serverSocket)
{
	struct epoll_event	serverEvents;
	struct epoll_event	clientsEvents[50];

	initEpoll(serverEvents);
	signal(SIGINT, exitProgram);
	while (running)
	{
		// CREATING CLIENTS EPOLL EVENT STRUCT
		std::cout << "EPOLL WAIT D EZINIZ\n";
		int clientNbr = epoll_wait(_epoll_fd, clientsEvents, 50, -1);
		if (clientNbr == -1) {
			throw Server::EpollWaitException();
		}
		std::cout << "ITERATE DE ZINZ\n";
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
			if (fd == _serverSocket) { // SERVER ACTIONS, NEW CONNECTIONS
				std::cout << "JE CONNECTE LES CLIENTS\n";
				connectClient(serverEvents);
			}
			else { // CLIENT ACTIONS
				std::cout << "J:ecoute LES CLIENTS\n";
				liaiseClient(it->second, fd);
			}
		}
	}
}

int	Server::checkPort(const char *portStr)
{
	char* ptr;
	long int test = strtol(portStr, &ptr, 10);
	unsigned short port = 0;

	if (test > 65535 || test < 1023)
		throw Server::PortOverflowException();
	else if (*ptr == '\0')
		port = test; // Port is valid
	else
		throw Server::PortNotNumberException();
	return port;
}

Server::Server(unsigned short port, std::string password) : _port(port), _password(password)
{
	// CREATING SOCKET
	int	_serverSocket;

	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
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

//	int on = 1;
 //	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
	//	throw Server::CantListenOnPortException();

	if (listen(_serverSocket, serverAddr.sin_port) < 0)
		throw Server::CantListenOnPortException();

	commandsChannels.insert(std::pair<std::string, void(Server::*)(Client&, std::stringstream &msg)>("JOIN", &Server::cmdJoin));
	commandsChannels.insert(std::pair<std::string, void(Server::*)(Client&, std::stringstream &msg)>("PART", &Server::cmdPart));
	commandsChannels.insert(std::pair<std::string, void(Server::*)(Client&, std::stringstream &msg)>("PRIVMSG", &Server::cmdPrivMsg));

	std::cout << std::endl << "All good!" << std::endl << "port    : " << _port << std::endl << "password: " << _password << std::endl;
	run(_serverSocket);
}

Server::~Server(void) {
	return;
}
