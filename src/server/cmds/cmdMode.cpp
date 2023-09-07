/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 18:15:01 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/07 19:02:54 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::modeI(Channel &channel, Client &client, bool status) {
	(void)channel;
	(void)client;
	(void)status;
	return ;
}

void Server::modeT(Channel &channel, Client &client, bool status) {
	(void)channel;
	(void)client;
	(void)status;
}

void Server::modeK(Channel &channel, Client &client, bool status) {
	(void)channel;
	(void)client;
	(void)status;
}

void Server::modeO(Channel &channel, Client &client, bool status) {
	(void)channel;
	(void)client;
	(void)status;
}

void Server::modeL(Channel &channel, Client &client, bool status) {
	(void)channel;
	(void)client;
	(void)status;
}

void Server::cmdMode(Client &client, std::stringstream &msg) {
	std::string	channelName;
	std::string	mode;

	msg.ignore(512, ' ');
	std::getline(msg, channelName, ' ');
	std::getline(msg, mode);

	if (!channelName.empty() && channelName[0] == '#') {
		std::map<std::string, Channel>::iterator it = channels.find(mode);
		if (it != channels.end()) {
			if (*mode.begin() == '+') {
				;
		}
		else
			sendMessage(client.getUserFd(), "Cannot send message to channel " + mode + '\n');
	}
	else {
		std::map<std::string, Client>::iterator it = clientsList.find(mode);
		if (it != clientsList.end()) {
			;
		}
		else
			sendMessage(client.getUserFd(), "Cannot send message to user " + mode + '\n');
	}
	}
}

