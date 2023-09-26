/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdPrivmsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:17:37 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/26 09:33:36 by bchabot          ###   ########.fr       */
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
	std::string			name;
	std::string			text;


	std::getline(msg, name, ' ');
	std::getline(msg, text);

	if (name.empty()) {
		sendMessage(client.getUserFd(), ERR_NORECIPIENT(client.getNickname()));
		return ;
	}

	if (text.empty()) {
		sendMessage(client.getUserFd(), ERR_NOTEXTTOSEND(client.getNickname()));
		return ;
	}

	if (name[0] == '#') {
		std::map<std::string, Channel>::iterator it = channels.find(name);
		if (it == channels.end()) {
			sendMessage(client.getUserFd(), ERR_CANNOTSENDTOCHAN(client.getNickname(), name));
			return ;
		}
		if (!it->second.isUserMember(client.getNickname())) {
			sendMessage(client.getUserFd(), ERR_NOTONCHANNEL(client.getNickname(), name));
			return ;
		}
		it->second.sendMessageToAllMembers(CHANNEL_MESSAGES(client.getUsername() +'!'+ client.getNickname(), name, text), client.getNickname());
	}
	else {
		int fd = getFdByNickname(name);
		std::map<int, Client>::iterator it = clientsList.find(fd);
		if (it != clientsList.end())
			sendMessage(it->second.getUserFd(), USER_MESSAGES(client.getUsername() +'!'+ client.getNickname(), name, text));
		else
			sendMessage(client.getUserFd(), ERR_NOSUCHNICK(client.getNickname(), name));
	}
}
