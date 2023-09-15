/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdTopic.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:18:27 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/07 16:18:57 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::cmdTopic(Client &client, std::stringstream &msg) {
	std::string			channel;
	std::string			text;

	msg.ignore(512, ' ');
	msg >> channel;
	std::getline(msg, text);

	std::map<std::string, Channel>::iterator it = channels.find(channel);
	if (it == channels.end()) {
		sendMessage(client.getUserFd(), ERR_NOSUCHCHANNEL(client.getNickname(), channel));
		return ;
	}
	std::cout << it->second.getChannelName() << std::endl;

	if (text.empty()) {
		sendMessage(client.getUserFd(), "Topic for " + channel + " is " + it->second.getTopic() + "\n");
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

	if (text == ":") {
		it->second.setTopic("");
		std::string msg = "Topic for " + channel + " has been cleared\n";
		it->second.sendMessageToAllMembers(msg, client.getNickname());
	}
	else {
		it->second.setTopic(text);
		std::string msg = "Topic for " + channel + " has been changed to " + it->second.getTopic() + "\n";
		it->second.sendMessageToAllMembers(msg, client.getNickname());
	}
}
