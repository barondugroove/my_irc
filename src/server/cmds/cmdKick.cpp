/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdKick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:19:09 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/26 12:51:44 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

void Server::cmdKick(Client &client, std::stringstream &msg) {
	std::string			channel;
	std::string			user;
	std::string			reason;

	std::getline(msg, channel, ' ');
	std::getline(msg, user, ' ');
	std::getline(msg, reason);

	if (user[0] == ':')
		user.erase(user.begin());


	//NEED MORE PARAMS
	if (channel.empty() || channel[0] != '#' || user.empty()) {
		sendMessage(client.getUserFd(), ERR_NEEDMOREPARAMS(client.getNickname(), channel));
		return ;
	}


	//CHANNEL DOES NOT EXIST
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

	//TARGET NOT ON THE CHANNEL
	if (!it->second.isUserMember(user)) {
		sendMessage(client.getUserFd(), ERR_NOTONCHANNEL(user, channel));
		return ;
	}

	//CANT KICK YOURSELF
	if (client.getNickname() == user) {
		sendMessage(client.getUserFd(), ERR_CANNOTKICKYSLF(client.getNickname(), channel));
		return ;
	}

	//USER IS NOT OPERATOR ON THE CHANNEL
	if (!it->second.isUserOperator(client.getNickname())) {
		sendMessage(client.getUserFd(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channel));
		return ;
	}
	it->second.sendMessageToAllMembers(RPL_KICK(client.getNickname(), channel, user, reason), "");
	it->second.eraseUser(user);
	if (it->second.getUserCount() == 0)
		eraseChannel(it);
}
