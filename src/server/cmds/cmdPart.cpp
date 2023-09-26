/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdPart.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:17:57 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/26 09:23:34 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::cmdPart(Client &client, std::stringstream &msg) {
	std::string			channel;

	std::getline(msg, channel);

	// NO CHANNEL IN PARAMETER
	if (channel.empty()) {
		sendMessage(client.getUserFd(), ERR_NORECIPIENT(client.getNickname()));
		return ;
	}

	std::map<std::string, Channel>::iterator it = channels.find(channel);

	// CHANNEL DOES NOT EXIST
	if (it == channels.end()) {
		sendMessage(client.getUserFd(), ERR_NOSUCHCHANNEL(client.getNickname(), channel));
		return ;
	}

	// USER IS NOT ON CHANNEL
	if (!it->second.isUserMember(client.getNickname())) {
		sendMessage(client.getUserFd(), ERR_NOTONCHANNEL(client.getNickname(), channel));
		return ;
	}

	it->second.eraseUser(client.getNickname());
	sendMessage(client.getUserFd(), RPL_PART(client.getNickname(), it->first));
	if (it->second.getUserCount() == 0)
		eraseChannel(it);
}
