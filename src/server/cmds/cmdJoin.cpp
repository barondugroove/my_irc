/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdJoin.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:14:38 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/12 18:06:16 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::cmdJoin(Client &client, std::stringstream &msg) {
	std::string	channelName;
	std::string	key;

	msg.ignore(512, ' ');
	std::getline(msg, channelName, ' ');
	std::getline(msg, key);

	if (channelName.empty() || channelName[0] != '#') {
		sendMessage(client.getUserFd(), ERR_NEEDMOREPARAMS(client.getNickname(), channelName));
		return ;
	}

	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end()) {
		std::cout << "New channel " << channelName << " created." << std::endl;
		Channel	newChannel(channelName, client);
		channels.insert(std::pair<std::string, Channel>(channelName, newChannel));
		sendMessage(client.getUserFd(), RPL_JOIN(client.getNickname(), channelName));
		return ;
	}
	std::cout << "channelName is : " << it->first << std::endl;

	if (it->second.isChannelFull()) {
		std::cout << client.getNickname() << " can not join channel " << channelName << std::endl;
		sendMessage(client.getUserFd(), ERR_CHANNELISFULL(client.getNickname(), channelName));
	}
	else {
		std::cout << client.getNickname() << " is joining channel " << channelName << std::endl;
		it->second.addUser(client.getNickname(), client);
		sendMessage(client.getUserFd(), RPL_JOIN(client.getNickname(), channelName));
	}
}
