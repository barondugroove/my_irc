/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 20:22:15 by rlaforge          #+#    #+#             */
/*   Updated: 2023/08/30 16:11:10 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <list>
#include <map>
#include <algorithm>
#include "Client.hpp"
#include "Channel.hpp"

class Server{

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
		void			clientAuth(Client &unauthClient, char *msg);
		void			handleClientMsg(Client &Client, char *msg);
		static int		checkPort(const char *portStr);
		void			cmdJoin(Client &client, std::stringstream &msg);
		void			cmdPart(Client &client, std::stringstream &msg);
		void			cmdPrivMsg(Client &client, std::stringstream &msg);
		void			eraseChannel(std::map<std::string, Channel*>::iterator it);

	private :
		//Consts
		std::map<std::string, void(Server::*)(Client&,std::stringstream &msg)> commandsChannels;
		std::map<std::string, void(Server::*)(Client&,std::stringstream &msg)> commandsAuth;
		unsigned short	_port;
		const std::string _password;
		std::map<std::string, Client> clientsList;
		std::map<std::string, Channel*> channels;
};

#endif
