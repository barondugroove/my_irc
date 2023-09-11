/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 18:15:01 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/11 16:40:43 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::modeI(Channel &channel, Client &client, std::stringstream &msg) {
	std::string	mode;
	bool	status = false;

	msg >> mode;

	if (mode[0] == '+')
		status = true;
	channel.setInviteMode(status);
	sendMessage(client.getUserFd(), RPL_INVITEMODE(client.getNickname(), channel.getChannelName(), mode));
	return ;
}

void Server::modeT(Channel &channel, Client &client, std::stringstream &msg) {
	std::string	mode;
	bool	status = false;

	msg >> mode;

	if (mode[0] == '+')
		status = true;
	channel.setTopicMode(status);
	sendMessage(client.getUserFd(), RPL_TOPICMODE(client.getNickname(), channel.getChannelName(), mode));
	return ;
}

void Server::modeK(Channel &channel, Client &client, std::stringstream &msg) {
	std::string	mode;
	std::string	password;

	msg >> mode;
	msg >> password;

	if (mode[0] == '+') {
		channel.setKeyMode(true);
		channel.setChannelKey("");
	}
	else {
		channel.setKeyMode(false);
		channel.setChannelKey(password);
	}
	sendMessage(client.getUserFd(), RPL_KEYMODE(client.getNickname(), channel.getChannelName(), mode));
	return ;
}

void Server::modeO(Channel &channel, Client &client, std::stringstream &msg) {
	std::string	mode;
	std::string	nickname;

	msg >> mode;
	std::getline(msg, nickname);
	if (mode[0] == '+')
		channel.addOperator(nickname);
	else
		channel.eraseOperator(nickname);
	sendMessage(client.getUserFd(), RPL_OPERATORMODE(client.getNickname(), channel.getChannelName(), mode));
	return ;
}

// C PAS FINI CA
void Server::modeL(Channel &channel, Client &client, std::stringstream &msg) {
	std::string	mode;
	std::string	nbr;
	int	userLimit;

	msg >> mode;
	msg >> nbr;
	userLimit = atoi(nbr.c_str());
	channel.setUserLimit(userLimit);
	sendMessage(client.getUserFd(), RPL_OPERATORMODE(client.getNickname(), channel.getChannelName(), mode));
	return ;
}

void Server::cmdMode(Client &client, std::stringstream &msg) {
	std::string	channelName;
	std::string	mode;

	msg.ignore(512, ' ');
	std::getline(msg, channelName, ' ');
	std::getline(msg, mode, ' ');
	if (mode[0] == '+' || mode[0] == '-')
		mode.erase(mode.begin());

	std::cout << "channel name is : " << channelName << " and mode is : " << mode << "." << std::endl;

	if (channelName.empty() || channelName[0] != '#') {
		sendMessage(client.getUserFd(), ERR_NEEDMOREPARAMS(client.getNickname(), channelName));
		return ;
	}

	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end()) {
		sendMessage(client.getUserFd(), ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
		return ;
	}

	std::map<std::string, void(Server::*)(Channel &channel, Client &client, std::stringstream &msg)>::iterator itMode = commandsMode.find(mode);
	if (itMode == commandsMode.end()) {
		sendMessage(client.getUserFd(), ERR_UMODEUNKNOWNFLAG(client.getNickname()));
		return ;
	}

	if (!it->second.isUserMember(client.getNickname())) {
		sendMessage(client.getUserFd(), ERR_NOTONCHANNEL(client.getNickname(), channelName));
		return ;
	}

	if (!it->second.isUserOperator(client.getNickname())) {
		sendMessage(client.getUserFd(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
		return ;
	}
	((*this).*itMode->second)(it->second, client, msg);
}

