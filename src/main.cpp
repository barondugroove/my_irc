/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 16:41:57 by rlaforge          #+#    #+#             */
/*   Updated: 2023/09/04 17:21:33 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "errors.cpp"
#include <iostream>

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
