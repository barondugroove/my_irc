/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 18:15:01 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/20 16:58:16 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::modeI(Channel &channel, Client &client, char mode, std::string arg) {
	(void)arg;
	if (mode == '+')
		channel.setInviteMode(true);
	else
		channel.setInviteMode(false);

	sendMessage(client.getUserFd(), RPL_MODE(client.getNickname(), channel.getChannelName(), mode + 'i'));
	return ;
}

void Server::modeT(Channel &channel, Client &client, char mode, std::string arg) {

	(void)arg;
	if (mode == '+')
		channel.setTopicMode(true);
	else
		channel.setTopicMode(false);
	sendMessage(client.getUserFd(), RPL_MODE(client.getNickname(), channel.getChannelName(), mode + 't'));
	return ;
}

void Server::modeK(Channel &channel, Client &client, char mode, std::string arg) {

	if (mode == '+') {
		channel.setPassMode(true);
		channel.setChannelPass(arg);
	}
	else {
		channel.setPassMode(false);
		channel.setChannelPass("");
	}
	sendMessage(client.getUserFd(), RPL_MODE(client.getNickname(), channel.getChannelName(), mode + 'k'));
	return ;
}

void Server::modeO(Channel &channel, Client &client, char mode, std::string arg) {

	if (mode == '+')
		channel.addOperator(arg);
	else
		channel.eraseOperator(arg);
	sendMessage(client.getUserFd(), RPL_MODE(client.getNickname(), channel.getChannelName(), mode + 'o'));
	return ;
}

// C PAS FINI CA
void Server::modeL(Channel &channel, Client &client, char mode, std::string arg) {
	int	userLimit;

	userLimit = atoi(arg.c_str());
	channel.setUserLimit(userLimit);
	sendMessage(client.getUserFd(), RPL_MODE(client.getNickname(), channel.getChannelName(), mode + 'l'));
	return ;
}

void Server::cmdMode(Client &client, std::stringstream &msg) {
	std::string	channelName;
	std::string	mode;
	std::string arg;

	std::getline(msg, channelName, ' ');
	std::getline(msg, mode, ' ');
	std::getline(msg, arg);



	std::cout << "channel name is : " << channelName << " and mode is : " << mode << " and argument is : " << arg << std::endl;

	//NOT ENOUGH PARAMETERS
	if (channelName.empty() || mode.empty() || channelName[0] != '#') {
		sendMessage(client.getUserFd(), ERR_NEEDMOREPARAMS(client.getNickname(), channelName));
		return ;
	}

	//TOO MUCH PARAMETERS
	if (mode[2] != '\0') {
		sendMessage(client.getUserFd(), ERR_TOOMUCHPARAMS(client.getNickname(), channelName));
		return ;
	}

	//CHANNEL DOES NOT EXIST
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end()) {
		sendMessage(client.getUserFd(), ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
		return ;
	}

	//MODE DOES NOT EXIST OR IS FORMATED WRONG
	std::map<char, void(Server::*)(Channel &channel, Client &client, char mode, std::string arg)>::iterator itMode = commandsMode.find(mode[1]);
	if (itMode == commandsMode.end() || (mode[0] != '+' && mode[0] != '-')) {
		sendMessage(client.getUserFd(), ERR_UMODEUNKNOWNFLAG(client.getNickname()));
		return ;
	}

	//USER NOT ON THE CHANNEL
	if (!it->second.isUserMember(client.getNickname())) {
		sendMessage(client.getUserFd(), ERR_NOTONCHANNEL(client.getNickname(), channelName));
		return ;
	}

	//USER IS NOT OPERATOR ON THE CHANNEL
	if (!it->second.isUserOperator(client.getNickname())) {
		sendMessage(client.getUserFd(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
		return ;
	}
	((*this).*itMode->second)(it->second, client, mode[0], arg);
}

