/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlaforge <rlaforge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 16:41:57 by rlaforge          #+#    #+#             */
/*   Updated: 2023/09/25 19:27:38 by rlaforge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <iostream>

void	checkPort(char *portStr) {
	char* ptr;
	long int test = strtol(portStr, &ptr, 10);

	if (test > 65535 || test < 1023)
		throw Server::PortOverflowException();
	else if (*ptr == '\0')
		return ; // Port is valid
	else
		throw Server::PortNotNumberException();
}



bool	checkInputs(char *port, char *password) {
	checkPort(port);
	if (!Server::checkPassword(password)) {
		std::cerr << "Error. Password format incorrect." << std::endl;	
		return false;
	}
	return true;
}

int main(int ac, char **av)
{

	if (ac != 3 || !av[1] || !av[2]) {
		std::cerr << "Error. Please use the correct format:" << std::endl << "./ircserv <port> <password>" << std::endl;
		return (1);
	}

	try {
		if (!checkInputs(av[1], av[2]))
			return (1);
		std::cout << "IRC Server"<< std::endl;
		std::string password = av[2];
		Server	server(atoi(av[1]), password);
	}
	catch (std::exception & e) {
		std::cerr << "Exception: "<< e.what() << std::endl;
	}

	return 0;
}
