/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdJoin.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:14:38 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/22 19:27:55 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::cmdJoin(Client &client, std::stringstream &msg) {
	std::string	channelName;
	std::string	key;

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

	if (it->second.isUserMember(client.getNickname())) {
		std::cout << client.getNickname() << " can not join channel " << channelName << std::endl;
		sendMessage(client.getUserFd(), ERR_CHANNELISFULL(client.getNickname(), channelName));
		return ;
	}

	if (it->second.getPassMode() && (it->second.getPassword() != key)) {
		std::cout << client.getNickname() << " can not join channel " << channelName << std::endl;
		sendMessage(client.getUserFd(), ERR_BADCHANNELKEY(client.getNickname(), channelName));
		return ;
	}
	
	if (it->second.getInviteMode() && !it->second.isUserInvited(client.getNickname())) {
		std::cout << client.getNickname() << " can not join channel " << channelName << std::endl;
		sendMessage(client.getUserFd(), ERR_INVITEONLYCHAN(client.getNickname(), channelName));
		return ;
	}
	
	if (it->second.isChannelFull()) {
		std::cout << client.getNickname() << " can not join channel " << channelName << std::endl;
		sendMessage(client.getUserFd(), ERR_CHANNELISFULL(client.getNickname(), channelName));
		return ;
	}
	std::cout << client.getNickname() << " is joining channel " << channelName << std::endl;
	it->second.addUser(client.getNickname(), client);
	sendMessage(client.getUserFd(), RPL_JOIN(client.getNickname(), channelName));
	it->second.sendMessageToAllMembers(RPL_JOIN(client.getNickname(), channelName));
}
