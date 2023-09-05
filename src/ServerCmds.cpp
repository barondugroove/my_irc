/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 14:45:15 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/05 19:09:53 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>

void Server::cmdJoin(Client &client, std::stringstream &msg) {
	std::string			args;

	msg.ignore(512, ' ');
	std::getline(msg, args);

	std::cout << "args is " << args << std::endl;
	if (!args.empty() && args[0] == '#') {
		std::map<std::string, Channel>::iterator it = channels.find(args);
		if (it != channels.end()) {
			std::cout << client.getNickname() << " is joining channel " << args << std::endl;
			it->second.addUser(client.getNickname(), client);
		}
		else {
			std::cout << "New channel " << args << " created." << std::endl;
			Channel	newChannel(args, client);
			channels.insert(std::pair<std::string, Channel>(args, newChannel));
			std::map<std::string, Channel>::iterator it = channels.find(args);
			it->second.addUser(client.getNickname(), client);
			it->second.setOperator(client.getNickname());
		}
	}
	else
		sendMessage(client.getUserFd(), "Cannot join channel " + args + '\n');
}

void Server::cmdInvite(Client &client, std::stringstream &msg) {
	std::string			channel;
	std::string			user;

	msg.ignore(512, ' ');
	msg >> channel;
	msg >> user;

	if (!channel.empty() && channel[0] == '#') {
		std::map<std::string, Channel>::iterator itChannel = channels.find(channel);
		std::map<std::string, Client>::iterator itClient = clientsList.find(user);
		if (itChannel != channels.end()  && itClient != clientsList.end()) {
			std::cout << client.getNickname() << " has invited " << user << " to channel " << channel << std::endl;
			itChannel->second.addUser(itClient->second.getNickname(), itClient->second);
		}
		else
			sendMessage(client.getUserFd(), "Cannot invite " + user + " to channel " + channel + ".\n");
	}
	else
		sendMessage(client.getUserFd(), "Cannot invite to channel " + channel + '\n');
}

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
			it->second.sendMessageToAllMembers(CHANNEL_MESSAGES(client.getNickname(), args, text));
		}
		else
			sendMessage(client.getUserFd(), "Cannot send message to channel " + args + '\n');
	}
	else {
		std::map<std::string, Client>::iterator it = clientsList.find(args);
		if (it != clientsList.end()) {
			sendMessage(it->second.getUserFd(), USER_MESSAGES(client.getNickname(), text));
		}
		else
			sendMessage(client.getUserFd(), "Cannot send message to user " + args + '\n');
	}
}

void Server::cmdPart(Client &client, std::stringstream &msg) {
	std::string			channel;

	msg.ignore(512, ' ');
	msg >> channel;

	if (!channel.empty() && channel[0] == '#') {
		std::map<std::string, Channel>::iterator it = channels.find(channel);
		if (it != channels.end() && it->second.isUserMember(client.getNickname())) {
			it->second.eraseUser(client.getNickname());
			if (it->second.getUserCount() == 0)
				eraseChannel(it);
		}
		else
			sendMessage(client.getUserFd(), ERR_NOTONCHANNEL(client.getNickname(), channel));
	}
	else
		sendMessage(client.getUserFd(), "Wrong PART channel : " + channel + '\n');
}

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
		it->second.sendMessageToAllMembers(msg);
	}
	else {
		it->second.setTopic(text);
		std::string msg = "Topic for " + channel + " has been changed to " + it->second.getTopic() + "\n";
		it->second.sendMessageToAllMembers(msg);
	}
}

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

void Server::cmdPass(Client &unauthClient, std::stringstream &msg) {
	std::string	password;

	msg.ignore(512, ' ');
	msg >> password;
	if (unauthClient.isAuth()) {
		sendMessage(unauthClient.getUserFd(), ERR_ALREADYREGISTERED(unauthClient.getNickname()));
		return ;
	}

	if (password.empty()) {
		sendMessage(unauthClient.getUserFd(), ERR_NEEDMOREPARAMS(unauthClient.getNickname(), "PASS"));
		return ;
	}
	unauthClient.setPassword(password);
}

void Server::cmdNick(Client &unauthClient, std::stringstream &msg) {
	std::string	nickname;

	msg.ignore(512, ' ');
	msg >> nickname;

	if (nickname.empty()) {
		sendMessage(unauthClient.getUserFd(), unauthClient.getNickname() + " :No nickname given\n");
		return ;
	}

	if (clientsList.find(nickname) != clientsList.end()) {
		sendMessage(unauthClient.getUserFd(), ERR_NICKNAMEINUSE(unauthClient.getNickname(), nickname));
		return ;
	}

	if (!unauthClient.isAuth()) {
		if (unauthClient.getPassword().empty()) {
			sendMessage(unauthClient.getUserFd(), unauthClient.getNickname() + " :You may not set NICK before using PASS\n");
			return ;
		}
		unauthClient.setNickname(nickname);
		sendMessage(unauthClient.getUserFd(), "Requesting the new nick " + nickname + "\n");
	}
	else {
		sendMessage(unauthClient.getUserFd(), unauthClient.getNickname() + " changed his nickname to " + nickname);
		unauthClient.setNickname(nickname);
	}
}

void Server::cmdUser(Client &unauthClient, std::stringstream &msg) {
	std::string	username;

	msg.ignore(512, ' ');
	msg >> username;

	if (username.empty()) {
		sendMessage(unauthClient.getUserFd(), unauthClient.getUsername() + " :No username given\n");
		return ;
	}

	if (!unauthClient.isAuth()) {
		if (unauthClient.getPassword().empty()) {
			sendMessage(unauthClient.getUserFd(), unauthClient.getUsername() + " :You may not set USER before using PASS\n");
			return ;
		}
		unauthClient.setUsername(username);
		sendMessage(unauthClient.getUserFd(), "Requesting the new username " + username + "\n");
		if (unauthClient.getPassword() == _password && !unauthClient.getNickname().empty()) {
			unauthClient.setAuth(true);
			sendMessage(unauthClient.getUserFd(), unauthClient.getNickname() + " is now authentified\n");
		}
		else
			unauthClient.clearInfo();
	}
	else {
		sendMessage(unauthClient.getUserFd(), unauthClient.getUsername() + " changed his username to " + username);
		unauthClient.setUsername(username);
	}
}
