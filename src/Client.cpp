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
#include <cmath>

const char* Client::GradeTooHighException::what() const throw() {
	return ("Client grade is too high.");
}

const char* Client::GradeTooLowException::what() const throw() {
	return ("Client grade is too low.");
}

Client::Client(int clientFD) : _fd(clientFD) {
	std::cout<< "Client Constructor called"<< std::endl;
	this->password = false;
	this->named = false;
	this->authentified = false;
	return;
}

Client::~Client(void) {
	std::cout<< "Client Destructor called"<< std::endl;
	return;
}

std::string & Client::getNickname() {
	return _nickname;
}

std::string	& Client::getUsername() {
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
