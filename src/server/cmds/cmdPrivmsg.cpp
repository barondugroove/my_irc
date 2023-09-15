/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdPrivmsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlaforge <rlaforge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:17:37 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/15 23:09:25 by rlaforge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::cmdPrivMsg(Client &client, std::stringstream &msg) {
	std::string			args;
	std::string			text;

	msg.ignore(512, ' ');
	msg >> args;
	msg.ignore(512, ' ');
	std::getline(msg, text);

	if (!args.empty() && args[0] == '#') {
		std::map<std::string, Channel>::iterator it = channels.find(args);
		if (it != channels.end()) {
			if (*text.begin() == ':')
				it->second.sendMessageToAllMembers(CHANNEL_MESSAGES(client.getNickname(), args, text), client.getNickname());
			else
				it->second.sendMessageToAllMembers(CHANNEL_MESSAGES(client.getNickname(), args, text), "");
		}
		else
			sendMessage(client.getUserFd(), "Cannot send message to channel " + args + '\n');
	}
	else {
		std::map<std::string, Client>::iterator it = clientsList.find(args);
		if (it != clientsList.end())
			sendMessage(it->second.getUserFd(), USER_MESSAGES(client.getNickname(), text));
		else
			sendMessage(client.getUserFd(), "Cannot send message to user " + args + '\n');
	}
}
