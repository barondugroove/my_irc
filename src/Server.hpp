/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 20:22:15 by rlaforge          #+#    #+#             */
/*   Updated: 2023/08/29 15:30:36 by bchabot          ###   ########.fr       */
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
		Server(const char *port, const char *password);		//Constructor
		Server();											//Default Constructor
		~Server();											//Destructor

		//Consts
		std::list<Client> clientsList;
		std::map<std::string, Channel> channels;
		std::string password;
		unsigned short port;


		//Functions
		void run(int serverFd);
		void clientAuth(Client &unauthClient, char *msg);
		void handleClientMsg(Client &Client, char *msg);
};

#endif
