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

#include "Channel.hpp"
#include "Server.hpp"
#include <iostream>

Channel::Channel(std::string name, Client &channelCreator) : _channelName(name) {
	std::cout<< "Channel Constructor called"<< std::endl;
	_operator = channelCreator.getNickname();
	_inviteMode = false;
	_topicMode = false;
	_passwordMode = false;
	_userLimit = -1;
	return;
}

Channel::~Channel(void) {
	std::cout<< "Channel Destructor called"<< std::endl;
	return;
}

std::string Channel::getChannelName() const {
	return _channelName;
}

int Channel::getUserCount() const {
	return _members.size();
}

void	Channel::sendMessageToAllMembers(std::string &msg) {
	std::map<std::string, Client*>::iterator it = _members.begin();
	for (; it != _members.end(); it++) {
		send(it->second->getUserFd(), msg.c_str(), msg.size(), 0);
	}
}

void	Channel::sendMessageToMember(std::map<std::string, Client*>::iterator it, std::string &msg) {
	send(it->second->getUserFd(), msg.c_str(), msg.size(), 0);
}

bool	Channel::isUserMember(std::string &nickName) {
	std::map<std::string, Client*>::iterator it = _members.find(nickName);
	if (it == _members.end())
		return false;
	return true;
}

void	Channel::eraseUser(std::string &nickName) {
	std::string msg = nickName + " leave channel " + this->_channelName + "\n";
	sendMessageToAllMembers(msg);
	_members.erase(_members.find(nickName));
}

void	Channel::addUser(std::string &nickName, Client &client) {
	if (_members.size() < _userLimit)
		_members.insert(std::pair<std::string, Client*>(nickName, &client));
	else
		send(client.getUserFd(), "Cannot join channel.\n", 21, 0);
}

void	Channel::setOperator(std::string &nickname) {
	_operator = nickname;
}