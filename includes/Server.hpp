/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 20:22:15 by rlaforge          #+#    #+#             */
/*   Updated: 2023/09/26 09:20:33 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include "define.hpp"
#include <string>
#include <map>
#include <algorithm>
#include <sys/epoll.h>
#include <sstream>
#include <iostream>

class Server {

	public:
		//Exceptions
		class PortNotNumberException : public std::exception {
					public :
						virtual const char* what() const throw();
		};

		class PortOverflowException : public std::exception {
					public :
						virtual const char* what() const throw();
		};

		class SocketErrorException : public std::exception {
					public :
						virtual const char* what() const throw();
		};

		class CantListenOnPortException : public std::exception {
					public :
						virtual const char* what() const throw();
		};

		class AcceptException : public std::exception {
					public :
						virtual const char* what() const throw();
		};

		class EpollCreationException : public std::exception {
					public :
						virtual const char* what() const throw();
		};

		class EpollWaitException : public std::exception {
					public :
						virtual const char* what() const throw();
		};

		class EpollControlException : public std::exception {
					public :
						virtual const char* what() const throw();
		};

		//Constructors
		Server(unsigned short port, std::string password);	//Constructor
		Server();											//Default Constructor
		~Server();											//Destructor

		//Functions
		void			run(int serverFd);
		void			initEpoll(struct epoll_event &serverEvents);
		void			connectClient(struct epoll_event &serverEvents);
		void			listenClient(Client &client, int fd);
		void			handleClientMsg(Client &Client, std::string msg);
		void			eraseChannel(std::map<std::string, Channel>::iterator it);
		void			sendMessage(const int &fd, std::string msg);
		void			closeFds();

		void			cmdPass(Client &unauthClient, std::stringstream &msg);
		void			cmdNick(Client &unauthClient, std::stringstream &msg);
		int				getFdByNickname(std::string &nickname);
		void			cmdUser(Client &unauthClient, std::stringstream &msg);

		void			cmdQuit(Client &client, std::stringstream &msg);
		void			cmdJoin(Client &client, std::stringstream &msg);
		void			cmdPart(Client &client, std::stringstream &msg);
		void			cmdPrivMsg(Client &client, std::stringstream &msg);
		void			cmdKick(Client &client, std::stringstream &msg);
		void			cmdInvite(Client &client, std::stringstream &msg);
		void			cmdTopic(Client &client, std::stringstream &msg);
		void			cmdMode(Client &client, std::stringstream &msg);

		void 			printModes(Client &client, Channel channel);
		void			modeI(Channel &channel, Client &client, char mode, std::string arg);
		void			modeT(Channel &channel, Client &client, char mode, std::string arg);
		void			modeK(Channel &channel, Client &client, char mode, std::string arg);
		void			modeO(Channel &channel, Client &client, char mode, std::string arg);
		void			modeL(Channel &channel, Client &client, char mode, std::string arg);

		static bool		checkPassword(std::string password);

	private :
		unsigned short		_port;
		const std::string	_password;
		int					_serverSocket;
		int					_epoll_fd;

		std::map<int, Client>			clientsList;
		std::map<std::string, Channel>	channels;

		std::map<std::string, void(Server::*)(Client& ,std::stringstream &msg)>								commandsChannels;
		std::map<char, void(Server::*)(Channel &channel, Client &client, char mode, std::string arg)>		commandsMode;
};

#endif
