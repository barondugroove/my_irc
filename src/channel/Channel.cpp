/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 14:48:28 by bchabot           #+#    #+#             */
/*   Updated: 2023/08/29 14:48:28 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include <iostream>

Channel::Channel(std::string name, Client &channelCreator) : _channelName(name) {
	std::cout<< "Channel Constructor called"<< std::endl;
	_userLimit = 1000;
	_inviteMode = false;
	_topicMode = false;
	_passwordMode = false;
	addUser(channelCreator.getNickname(), channelCreator);
	_operator.push_back(channelCreator.getNickname());

	for (std::map<std::string, Client*>::iterator it = _members.begin(); it != _members.end(); it++)
		std::cout << "member is : " << it->first << std::endl;
		
	for (std::vector<std::string>::iterator it = _operator.begin(); it != _operator.end(); it++)
		std::cout << "_operator is : " << *it << std::endl;
	return;
}

Channel::Channel(const Channel &src) {
	*this = src;
}

Channel & Channel::operator=(const Channel &rhs) {
	if (this != &rhs) {
		this->_channelName = rhs._channelName;
		this->_members = rhs._members;
		this->_operator = rhs._operator;
		this->_key = rhs._key;

		this->_inviteMode = rhs._inviteMode;
		this->_topicMode = rhs._topicMode;
		this->_passwordMode = rhs._passwordMode;
		this->_userLimit = rhs._userLimit;
	}
	return *this;
}

Channel::~Channel(void) {
	std::cout<< "Channel Destructor called"<< std::endl;
	return;
}

bool	Channel::isChannelFull() {
	return (_userLimit - _members.size()) >= 1;
}

void	Channel::sendMessageToAllMembers(std::string msg, std::string nickname) {
	std::map<std::string, Client*>::iterator it = _members.begin();
	for (; it != _members.end(); it++) {
		if (it->second->getNickname() != nickname)
			send(it->second->getUserFd(), msg.c_str(), msg.size(), 0);
	}
}

void	Channel::sendMessageToMember(std::map<std::string, Client*>::iterator it, std::string msg) {
	send(it->second->getUserFd(), msg.c_str(), msg.size(), 0);
}

bool	Channel::isUserMember(std::string &nickname) {
	std::map<std::string, Client*>::iterator it = _members.find(nickname);
	if (it == _members.end())
		return false;
	return true;
}

bool	Channel::isUserOperator(std::string &nickname) {
	std::vector<std::string>::iterator it = std::find(_operator.begin(), _operator.end(), nickname);
	if (it == _operator.end())
		return false;
	return true;
}

void	Channel::eraseUser(std::string &nickname) {
	std::string msg = nickname + " leave channel " + this->_channelName + "\n";
	sendMessageToAllMembers(msg, nickname);
	_members.erase(_members.find(nickname));
}

void	Channel::addUser(std::string &nickname, Client &client) {
	if ((int)_members.size() < _userLimit) {
		std::cout << "User " << nickname << " added to " << this->_channelName << std::endl;
		_members.insert(std::pair<std::string, Client*>(nickname, &client));
	}
	else
		send(client.getUserFd(), "Cannot join channel.\n", 21, 0);
}

void	Channel::addOperator(std::string &nickname) {
	_operator.push_back(nickname);
}

void	Channel::eraseOperator(std::string &nickname) {
	for (std::vector<std::string>::iterator it = _operator.begin(); it != _operator.end(); it++) {
		if (*it == nickname)
			_operator.erase(it);
	}
}

void	Channel::setTopic(std::string topic) {
	_topic = topic;
}

void	Channel::setInviteMode(bool status) {
	_inviteMode = status;
}

void	Channel::setChannelKey(std::string password) {
	_key = password;
}

void	Channel::setTopicMode(bool status) {
	_topicMode = status;
}

void	Channel::setUserLimit(int userLimit) {
	_userLimit = userLimit;
}

void	Channel::setKeyMode(bool status) {
	_passwordMode = status;	
}