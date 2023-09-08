/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 18:15:01 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/08 17:21:43 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::modeI(Channel &channel, Client &client, std::string mode) {
	bool	status = false;

	if (mode[0] == '+')
		status = true;

	channel.setInviteMode(status);
	sendMessage(client.getUserFd(), RPL_INVITEMODE(client.getNickname(), channel.getChannelName(), mode));
	return ;
}

void Server::modeT(Channel &channel, Client &client, std::string mode) {
	bool	status = false;

	if (mode[0] == '+')
		status = true;

	channel.setTopicMode(status);
	sendMessage(client.getUserFd(), RPL_TOPICMODE(client.getNickname(), channel.getChannelName(), mode));
	return ;
}

void Server::modeK(Channel &channel, Client &client, std::string mode) {
	bool	status = false;

	if (mode[0] == '+')
		status = true;

	channel.setTopicMode(status);
	sendMessage(client.getUserFd(), RPL_KEYMODE(client.getNickname(), channel.getChannelName(), mode));
	return ;
}

void Server::modeO(Channel &channel, Client &client, std::string mode) {
	bool	status = false;

	if (mode[0] == '+')
		status = true;

	channel.setOperatorMode(status);
	sendMessage(client.getUserFd(), RPL_OPERATORMODE(client.getNickname(), channel.getChannelName(), mode));
	return ;
}

// C PAS FINI CA
void Server::modeL(Channel &channel, Client &client, std::string mode) {
	int	userLimit = 100;

	channel.setUserLimit(userLimit);
	sendMessage(client.getUserFd(), RPL_OPERATORMODE(client.getNickname(), channel.getChannelName(), mode));
	return ;
}

void Server::cmdMode(Client &client, std::stringstream &msg) {
	std::string	channelName;
	std::string	mode;

	msg.ignore(512, ' ');
	std::getline(msg, channelName, ' ');
	std::getline(msg, mode);

	if (channelName.empty() || channelName[0] != '#') {
		sendMessage(client.getUserFd(), ERR_NEEDMOREPARAMS(client.getNickname(), channelName));
		return ;
	}

	std::map<std::string, Channel>::iterator it = channels.find(mode);
	if (it == channels.end()) {
		sendMessage(client.getUserFd(), ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
		return ;
	}

	std::map<std::string, void(Server::*)(Channel &channel, Client &client, std::string mode)>::iterator itMode = commandsMode.find(mode);
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
	((*this).*itMode->second)(it->second, client, mode);
}

