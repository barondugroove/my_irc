/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdPart.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:17:57 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/25 18:34:07 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::cmdPart(Client &client, std::stringstream &msg) {
	std::string			channel;

	std::getline(msg, channel);

	if (channel.empty()) {
		sendMessage(client.getUserFd(), ERR_NORECIPIENT(client.getNickname()));
		return ;
	}
	
	std::map<std::string, Channel>::iterator it = channels.find(channel);
	if (it == channels.end()) {
		sendMessage(client.getUserFd(), ERR_NOSUCHCHANNEL(client.getNickname(), channel));
		return ;
	}
	if (it->second.isUserMember(client.getNickname())) {
		it->second.eraseUser(client.getNickname());
		sendMessage(client.getUserFd(), RPL_PART(client.getNickname(), it->first));
		if (it->second.getUserCount() == 0)
			eraseChannel(it);
	}
	else
		sendMessage(client.getUserFd(), ERR_NOTONCHANNEL(client.getNickname(), channel));
}
