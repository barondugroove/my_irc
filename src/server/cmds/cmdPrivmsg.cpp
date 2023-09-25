/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdPrivmsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlaforge <rlaforge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:17:37 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/25 23:04:29 by rlaforge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

int	Server::getFdByNickname(std::string &nickname) {
	std::map<int, Client>::iterator it = clientsList.begin();
	for (; it != clientsList.end(); it++) {
		if (nickname == it->second.getNickname())
			return it->second.getUserFd();
	}
	return -1;
}

void Server::cmdPrivMsg(Client &client, std::stringstream &msg) {
	std::string			args;
	std::string			text;

	msg >> args;
	msg.ignore(512, ' ');
	std::getline(msg, text);

	if (args.empty()) {
		sendMessage(client.getUserFd(), ERR_NORECIPIENT(client.getNickname()));
		return ;
	}

	if (text.empty()) {
		sendMessage(client.getUserFd(), ERR_NOTEXTTOSEND(client.getNickname()));
		return ;	
	}

	if (args[0] == '#') {
		std::map<std::string, Channel>::iterator it = channels.find(args);
		if (it == channels.end()) {
			sendMessage(client.getUserFd(), ERR_CANNOTSENDTOCHAN(client.getNickname(), args));
			return ;
		}
		if (!it->second.isUserMember(client.getNickname())) {
			sendMessage(client.getUserFd(), ERR_NOTONCHANNEL(client.getNickname(), args));
			return ;	
		}
		it->second.sendMessageToAllMembers(CHANNEL_MESSAGES(client.getNickname(), args, text), client.getNickname());
	}
	else {
		int fd = getFdByNickname(args);
		std::map<int, Client>::iterator it = clientsList.find(fd);
		if (it != clientsList.end())
			sendMessage(it->second.getUserFd(), USER_MESSAGES(client.getNickname(), args, text));
		else
			sendMessage(client.getUserFd(), ERR_ERRONEUSNAME(client.getNickname(), args, "nickname"));
	}
}
