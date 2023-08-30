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
	this->password = false;
	this->named = false;
	this->authentified = false;
	return;
}

Client::Client(const Client &src) : _fd(src.getUserFd()) {
	*this = src;
}

Client & Client::operator=(const Client &rhs) {
	if (this != &rhs) {
		this->password = rhs.password;
		this->named = rhs.named;
		this->authentified = rhs.authentified;
		this->_nickname = rhs._nickname;
		this->_username = rhs._username;
	}
	return *this;
}

Client::~Client(void) {
	std::cout<< "Client Destructor called"<< std::endl;
	return;
}

std::string & Client::getNickname() {
	return _nickname;
}

std::string & Client::getUsername() {
	return _username;
}

const int & Client::getUserFd() const {
	return _fd;
}

void Client::setNickname(std::string &nickname) {
	_nickname = nickname;
}

void Client::setUsername(std::string &username) {
	_username = username;
}
