/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdPart.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:17:57 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/08 15:59:52 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::cmdPart(Client &client, std::stringstream &msg) {
	std::string			channel;

	msg.ignore(512, ' ');
	std::getline(msg, channel, ' ');

	std::map<std::string, Channel>::iterator it = channels.find(channel);
	if (it == channels.end()) {
		sendMessage(client.getUserFd(), ERR_NOSUCHCHANNEL(client.getNickname(), channel));
		return ;
	}
	if (it->second.isUserMember(client.getNickname())) {
		it->second.eraseUser(client.getNickname());
		if (it->second.getUserCount() == 0)
			eraseChannel(it);
	}
	else
		sendMessage(client.getUserFd(), ERR_NOTONCHANNEL(client.getNickname(), channel));
}
