/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdInvite.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:15:57 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/07 16:17:25 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::cmdInvite(Client &client, std::stringstream &msg) {
	std::string			channel;
	std::string			user;

	msg.ignore(512, ' ');
	msg >> channel;
	msg >> user;

	if (!channel.empty() && channel[0] == '#') {
		std::map<std::string, Channel>::iterator itChannel = channels.find(channel);
		std::map<std::string, Client>::iterator itClient = clientsList.find(user);
		if (itChannel != channels.end()  && itClient != clientsList.end()) {
			std::cout << client.getNickname() << " has invited " << user << " to channel " << channel << std::endl;
			itChannel->second.addUser(itClient->second.getNickname(), itClient->second);
		}
		else
			sendMessage(client.getUserFd(), "Cannot invite " + user + " to channel " + channel + ".\n");
	}
	else
		sendMessage(client.getUserFd(), "Cannot invite to channel " + channel + '\n');
}
