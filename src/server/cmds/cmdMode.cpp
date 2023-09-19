/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlaforge <rlaforge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 18:15:01 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/19 17:21:17 by rlaforge         ###   ########.fr       */
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
	sendMessage(client.getUserFd(), RPL_MODE(client.getNickname(), channel.getChannelName(), mode));
	return ;
}

void Server::modeT(Channel &channel, Client &client, std::stringstream &msg) {
	std::string	mode;
	bool	status = false;

	msg >> mode;

	if (mode[0] == '+')
		status = true;
	channel.setTopicMode(status);
	sendMessage(client.getUserFd(), RPL_MODE(client.getNickname(), channel.getChannelName(), mode));
	return ;
}

void Server::modeK(Channel &channel, Client &client, std::stringstream &msg) {
	std::string	mode;
	std::string	password;

	msg >> mode;
	msg >> password;

	if (mode[0] == '+') {
		channel.setPassMode(true);
		channel.setChannelPass("");
	}
	else {
		channel.setPassMode(false);
		channel.setChannelPass(password);
	}
	sendMessage(client.getUserFd(), RPL_MODE(client.getNickname(), channel.getChannelName(), mode));
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
	sendMessage(client.getUserFd(), RPL_MODE(client.getNickname(), channel.getChannelName(), mode));
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
	sendMessage(client.getUserFd(), RPL_MODE(client.getNickname(), channel.getChannelName(), mode));
	return ;
}

void Server::cmdMode(Client &client, std::stringstream &msg) {
	std::string	channelName;
	std::string	mode;
	std::string arg;

	msg.ignore(512, ' ');
	std::getline(msg, channelName, ' ');
	std::getline(msg, mode, ' ');
	std::getline(msg, arg);



	std::cout << "channel name is : " << channelName << " and mode is : " << mode << " and agrument is : " << arg << std::endl;

	if (channelName.empty() || mode.empty() || channelName[0] != '#') {
		sendMessage(client.getUserFd(), ERR_NEEDMOREPARAMS(client.getNickname(), channelName));
		return ;
	}



	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end()) {
		sendMessage(client.getUserFd(), ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
		return ;
	}

	std::map<std::string, void(Server::*)(Channel &channel, Client &client, std::stringstream &msg)>::iterator itMode = commandsMode.find(getString(mode[1]));
	if (itMode == commandsMode.end() || (mode[0] != '+' || mode[0] != '-')) {
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

