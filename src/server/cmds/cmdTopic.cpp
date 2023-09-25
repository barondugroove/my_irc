/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdTopic.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:18:27 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/25 20:07:26 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::cmdTopic(Client &client, std::stringstream &msg) {
	std::string			channel;
	std::string			text;

	msg >> channel;
	std::getline(msg, text);

	std::map<std::string, Channel>::iterator it = channels.find(channel);
	if (it == channels.end()) {
		sendMessage(client.getUserFd(), ERR_NOSUCHCHANNEL(client.getNickname(), channel));
		return ;
	}

	if (it->second.getTopicMode() && !it->second.isUserOperator(client.getNickname())) {
		sendMessage(client.getUserFd(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channel));
		return ;
	}
	
	if (!it->second.getTopicMode() && !it->second.isUserMember(client.getNickname())) {
		sendMessage(client.getUserFd(), ERR_NOTONCHANNEL(client.getNickname(), channel));
		return ;
	}

	if (text.empty()) {
		if (!it->second.getTopic().empty())
			sendMessage(client.getUserFd(), RPL_TOPIC(client.getNickname(), channel, it->second.getTopic()));
		else
			sendMessage(client.getUserFd(), RPL_NOTOPIC(client.getNickname(), channel));
	}
	else if (text == ":") {
		it->second.setTopic("");
		std::string msg = client.getNickname() + " " + channel + " :No topic is set\r\n";
		sendMessage(client.getUserFd(), RPL_NOTOPIC(client.getNickname(), channel));
		it->second.sendMessageToAllMembers(RPL_NOTOPIC(client.getNickname(), channel), client.getNickname());
	}
	else {
		it->second.setTopic(text);
		std::string msg = client.getNickname() + " " + channel + " :" + text + "\r\n";
		sendMessage(client.getUserFd(), RPL_TOPIC(client.getNickname(), channel, it->second.getTopic()));
		it->second.sendMessageToAllMembers(RPL_TOPIC(client.getNickname(), channel, it->second.getTopic()), client.getNickname());
	}
}

