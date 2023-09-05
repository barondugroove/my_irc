/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlaforge <rlaforge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 02:09:21 by rlaforge          #+#    #+#             */
/*   Updated: 2023/07/30 02:09:21 by rlaforge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <iostream>

Client::Client(int clientFD) : _fd(clientFD) {
	std::cout<< "Client Constructor called"<< std::endl;
	this->_authentified = false;
	this->_nickname = "";
	this->_username = "";
	return;
}

Client::Client(const Client &src) : _fd(src.getUserFd()) {
	*this = src;
}

Client & Client::operator=(const Client &rhs) {
	if (this != &rhs) {
		this->_authentified = rhs._authentified;
		this->_nickname = rhs._nickname;
		this->_username = rhs._username;
	}
	return *this;
}

Client::~Client(void) {
	std::cout<< "Client Destructor called"<< std::endl;
	return;
}

void	Client::setNickname(std::string nickname) {
	_nickname = nickname;
}

void	Client::setUsername(std::string username) {
	_username = username;
}

void	Client::setPassword(std::string password) {
	_password = password;
}

void	Client::setAuth(bool status) {
	_authentified = status;
}

void	Client::clearInfo() {
	_password = "";
	_nickname = "";
	_username = "";
}
