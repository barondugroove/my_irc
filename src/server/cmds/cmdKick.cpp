/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdKick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:19:09 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/07 16:19:18 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::cmdKick(Client &client, std::stringstream &msg) {
	std::string			channel;
	std::string			user;

	msg.ignore(512, ' ');
	msg >> channel;
	msg >> user;

	if (!channel.empty() && channel[0] == '#') {
		std::map<std::string, Channel>::iterator it = channels.find(channel);
		if (it != channels.end() && it->second.isUserMember(client.getNickname())) {
			it->second.eraseUser(client.getNickname());
		}
		else
			sendMessage(client.getUserFd(), ERR_NOTONCHANNEL(client.getNickname(), channel));
	}
	else
		sendMessage(client.getUserFd(), "Wrong channel name : " + channel + '\n');
}
