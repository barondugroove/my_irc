/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 16:41:57 by rlaforge          #+#    #+#             */
/*   Updated: 2023/09/04 22:51:12 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
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
	return ;
}

bool	checkPassword(char *password) {
	if (!password)
		return false;
	return true;
}

void	checkInputs(char *port, char *password) {
	checkPort(port);
	if (!checkPassword(password))
		std::cout << "C LA MERDE IL FAUT GERER LERREUR DES MDPS!!" << std::endl;
}

int main(int ac, char **av)
{

	if (ac != 3 || !av[1] || !av[2]) {
		std::cout << "Please use the correct format:" << std::endl << "./ircserv <port> <password>" << std::endl;
		return (1);
	}

	try {
		checkInputs(av[1], av[2]);
		std::cout << "IRC Server"<< std::endl;
		std::string password = av[2];
		Server	server(atoi(av[1]), password);
	}
	catch (std::exception & e) {
		std::cerr << "Exception: "<< e.what() << std::endl;
	}

	return 0;
}
