/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdInvite.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:15:57 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/26 04:20:37 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::cmdInvite(Client &client, std::stringstream &msg) {
	std::string		channel;
	std::string		user;

	std::getline(msg, channel, ' ');
	std::getline(msg, user);

	if (channel.empty() || channel[0] != '#' || user.empty()) {
		sendMessage(client.getUserFd(), ERR_NEEDMOREPARAMS(client.getNickname(), channel));
		return ;
	}

	std::map<std::string, Channel>::iterator itChannel = channels.find(channel);
	if (itChannel == channels.end()) {
		sendMessage(client.getUserFd(), ERR_NOSUCHCHANNEL(client.getNickname(), channel));
		return ;
	}
	
	if (itChannel->second.isUserMember(user)) {
		sendMessage(client.getUserFd(), ERR_USERONCHANNEL(client.getNickname(), user, channel));
		return ;
	}
	
	if (!itChannel->second.isUserOperator(client.getNickname())) {
		sendMessage(client.getUserFd(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channel));
		return ;
	}

	std::map<int, Client>::iterator itClient = clientsList.find(getFdByNickname(user));
	if (itClient == clientsList.end()) {
		sendMessage(client.getUserFd(), ERR_NOSUCHNICK(client.getNickname(), channel));
		return ;
	}
	std::cout << client.getNickname() << " has invited " << user << " to channel " << channel << std::endl;
	itChannel->second.addInvitee(user);
	sendMessage(client.getUserFd(), RPL_INVITESNDR(client.getNickname(), user, channel));
	sendMessage(getFdByNickname(user), RPL_INVITERCVR(client.getNickname(), user, channel));
}
