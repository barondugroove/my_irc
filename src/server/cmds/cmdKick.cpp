/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdKick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:19:09 by bchabot           #+#    #+#             */
<<<<<<< Updated upstream
/*   Updated: 2023/09/25 15:28:22 by rlaforge         ###   ########.fr       */
=======
/*   Updated: 2023/09/25 16:12:34 by bchabot          ###   ########.fr       */
>>>>>>> Stashed changes
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::cmdKick(Client &client, std::stringstream &msg) {
	std::string			channel;
	std::string			user;
	std::string			arg;

	std::getline(msg, channel, ' ');
	std::getline(msg, user, ' ');
	std::getline(msg, arg);

<<<<<<< Updated upstream
	if (!arg.empty()) {
		sendMessage(client.getUserFd(), ERR_TOOMUCHPARAMS(client.getNickname(), channel));
		return ;
	}
	if (!channel.empty() && channel[0] == '#') {
		std::map<std::string, Channel>::iterator it = channels.find(channel);
		if (it != channels.end() && it->second.isUserMember(user)) {
			
			if (!it->second.isUserOperator(client.getNickname())) {
				sendMessage(client.getUserFd(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channel));
				return ;
			}
			it->second.eraseUser(user);
		}
		else
			sendMessage(client.getUserFd(), ERR_NOTONCHANNEL(client.getNickname(), channel));
=======
	if (channel.empty() || channel[0] != '#' || user.empty() || !arg.empty()) {
		sendMessage(client.getUserFd(), ERR_NEEDMOREPARAMS(client.getNickname(), channel));
		return ;
>>>>>>> Stashed changes
	}

	std::map<std::string, Channel>::iterator it = channels.find(channel);
	if (it == channels.end()) {
		sendMessage(client.getUserFd(), ERR_NOSUCHCHANNEL(client.getNickname(), channel));
		return ;
	}

	//USER NOT ON THE CHANNEL
	if (!it->second.isUserMember(client.getNickname())) {
		sendMessage(client.getUserFd(), ERR_NOTONCHANNEL(client.getNickname(), channel));
		return ;
	}

	//USER IS NOT OPERATOR ON THE CHANNEL
	if (!it->second.isUserOperator(client.getNickname())) {
		sendMessage(client.getUserFd(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channel));
		return ;
	}
	it->second.eraseUser(user);
	sendMessage()
}
