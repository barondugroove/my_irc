/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 16:41:57 by rlaforge          #+#    #+#             */
/*   Updated: 2023/08/24 15:18:43 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>

int main(int ac, char **av)
{

	if (ac != 3) {
		std::cout << "Please use the correct format:" << std::endl << "./ircserv <port> <password>" << std::endl;
		return (1);
	}

	try {
		std::cout << "IRC Server"<< std::endl;
		Server	server(av[1], av[2]);
	}
	catch (std::exception & e) {
		std::cerr << "Exception: "<< e.what() << std::endl;
	}

	return 0;
}
