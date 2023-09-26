/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 18:15:01 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/26 12:12:11 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::printModes(Client &client, Channel channel) {
	std::string modes = "+";

	if(channel.getInviteMode())
		modes += "i";
	if(channel.getTopicMode())
		modes += "t";
	if(channel.getPassMode())
		modes += "k";

	sendMessage(client.getUserFd(), RPL_CHANNELMODEIS(client.getNickname(), channel.getChannelName(), modes));
}

void Server::modeI(Channel &channel, Client &client, char mode, std::string arg) {
	(void)arg;
	if (mode == '+')
		channel.setInviteMode(true);
	else
		channel.setInviteMode(false);

	sendMessage(client.getUserFd(), RPL_MODE(client.getNickname(), channel.getChannelName(), mode + 'i', ""));
	return ;
}

void Server::modeT(Channel &channel, Client &client, char mode, std::string arg) {
	(void)arg;
	if (mode == '+')
		channel.setTopicMode(true);
	else
		channel.setTopicMode(false);
	sendMessage(client.getUserFd(), RPL_MODE(client.getNickname(), channel.getChannelName(), mode + 't', ""));
	return ;
}

void Server::modeK(Channel &channel, Client &client, char mode, std::string arg) {

	if (mode == '+') {
		if (arg.empty()) {
			sendMessage(client.getUserFd(), "No password\r\n");
			return ;
		}
		if (Server::checkPassword(arg)) {
			channel.setPassMode(true);
			channel.setChannelPass(arg);
		} else {
			sendMessage(client.getUserFd(), "Invalid password\r\n");
			return ;
		}
	} else {
		channel.setPassMode(false);
		channel.setChannelPass("");
	}
	sendMessage(client.getUserFd(), RPL_MODE(client.getNickname(), channel.getChannelName(), mode + 'k', ""));
	return ;
}

void Server::modeO(Channel &channel, Client &client, char mode, std::string name) {
	//USER NOT ON THE CHANNEL
	if (!channel.isUserMember(name)) {
		sendMessage(client.getUserFd(), ERR_NOTONCHANNEL(name, channel.getChannelName()));
		return ;
	}

	if (mode == '+')
		channel.addOperator(name);
	else
		channel.eraseOperator(name);
	sendMessage(client.getUserFd(), RPL_MODE(client.getNickname(), channel.getChannelName(), mode + 'o', name));
	return ;
}

void Server::modeL(Channel &channel, Client &client, char mode, std::string arg) {
	int	userLimit;

	if (mode == '-') {
		channel.setUserLimit(1000);
		sendMessage(client.getUserFd(), RPL_MODE(client.getNickname(), channel.getChannelName(), mode + 'l', ""));
		return ;
	}

	userLimit = atoi(arg.c_str());

	if (userLimit < 1 || userLimit > MAXCLIENTS)
	{
		sendMessage(client.getUserFd(), "Number is out of range\r\n");
		return ;
	}
	channel.setUserLimit(userLimit);
	sendMessage(client.getUserFd(), RPL_MODE(client.getNickname(), channel.getChannelName(), mode + 'l', ""));
	return ;
}



void Server::cmdMode(Client &client, std::stringstream &msg) {
	std::string	channelName;
	std::string	mode;
	std::string arg;

	std::getline(msg, channelName, ' ');
	std::getline(msg, mode, ' ');
	std::getline(msg, arg);

	//NO CHANNEL
	if (channelName.empty() || channelName[0] != '#') {
		sendMessage(client.getUserFd(), ERR_NEEDMOREPARAMS(client.getNickname(), channelName));
		return ;
	}

	//CHANNEL DOES NOT EXIST
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end()) {
		sendMessage(client.getUserFd(), ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
		return ;
	}

	//PRINT MODE PARAMS
	if (mode.empty() && arg.empty()) {
		printModes(client, it->second);
		return ;
	}

	//TOO MUCH MODE PARAMETERS
	if (mode[2] != '\0') {
		sendMessage(client.getUserFd(), ERR_TOOMUCHPARAMS(client.getNickname(), channelName));
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
