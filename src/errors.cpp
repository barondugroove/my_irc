/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:54:21 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/04 17:27:13 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <algorithm>
#include <iostream>

void	checkPort(char *portStr) {
	char* ptr;
	long int test = strtol(portStr, &ptr, 10);
	unsigned short port = 0;

	if (test > 65535 || test < 1023)
		throw Server::PortOverflowException();
	else if (*ptr == '\0')
		port = test; // Port is valid
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
