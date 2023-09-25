/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdInvite.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:15:57 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/25 15:44:14 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::cmdInvite(Client &client, std::stringstream &msg) {
	std::string		channel;
	std::string		user;

	msg >> channel;
	msg >> user;

	if (channel.empty() || user.empty() || channel[0] != '#') {
		sendMessage(client.getUserFd(), ERR_NEEDMOREPARAMS(client.getNickname(), channel));
		return ;
	}

	std::map<std::string, Channel>::iterator itChannel = channels.find(channel);
	if (itChannel == channels.end()) {
		sendMessage(client.getUserFd(), ERR_NOSUCHCHANNEL(client.getNickname(), channel));
		return ;
	}
	
	std::map<int, Client>::iterator itClient = clientsList.find(client.getUserFd());
	if (!itChannel->second.isUserMember(client.getNickname()) || itClient == clientsList.end()) {
		sendMessage(client.getUserFd(), ERR_NOTONCHANNEL(client.getNickname(), channel));
		return ;
	}
	if (!itChannel->second.isUserOperator(client.getNickname())) {
		sendMessage(client.getUserFd(), ERR_NOTONCHANNEL(client.getNickname(), channel));
		return ;
	}
	std::cout << client.getNickname() << " has invited " << user << " to channel " << channel << std::endl;
	itChannel->second.addInvitee(user);
	sendMessage(client.getUserFd(), RPL_INVITESNDR(client.getNickname(), user, channel));
	sendMessage(getFdByNickname(user), RPL_INVITERCVR(client.getNickname(), user, channel));
}
