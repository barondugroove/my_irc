/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdAuth.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:19:31 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/26 10:59:13 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

bool	checkName(std::string &str) {
	const std::string forbiddenChars = " ,*?!@.";

	if (str.empty() || str[0] == '$' || str[0] == ':' || str[0] == '#' || str[0] == '&')
		return false;
	for (size_t i = 0; i < str.size(); i++) {
		if (forbiddenChars.find(str[i]) != std::string::npos)
			return false;
	}
	return true;
}

void Server::cmdPass(Client &unauthClient, std::stringstream &msg) {
	std::string	password;
	std::getline(msg, password);

	if (!Server::checkPassword(password)) {
		sendMessage(unauthClient.getUserFd(), ERR_PASSWDMISMATCH(unauthClient.getNickname()));
		return ;
	}
	else if (unauthClient.isAuth()) {
		sendMessage(unauthClient.getUserFd(), ERR_ALREADYREGISTERED(unauthClient.getNickname()));
		return ;
	}
	else if (password.empty()) {
		sendMessage(unauthClient.getUserFd(), ERR_NEEDMOREPARAMS(unauthClient.getNickname(), "PASS"));
		return ;
	}
	unauthClient.setPassword(password);
	sendMessage(unauthClient.getUserFd(), unauthClient.getNickname() + " :password is set to " + password + "\n");
}

void Server::cmdNick(Client &unauthClient, std::stringstream &msg) {
	std::string	nickname;
	std::getline(msg, nickname);

	if (nickname.empty()) {
		sendMessage(unauthClient.getUserFd(), unauthClient.getNickname() + " :No nickname given\n");
		return ;
	}

	if (getFdByNickname(nickname) != -1 && getFdByNickname(nickname) != unauthClient.getUserFd()) {
		sendMessage(unauthClient.getUserFd(), ERR_NICKNAMEINUSE(unauthClient.getNickname(), nickname));
		return ;
	}

	if (!checkName(nickname)) {
		sendMessage(unauthClient.getUserFd(), ERR_ERRONEUSNICKNAME(unauthClient.getNickname(), nickname));
		return ;
	}

	if (!unauthClient.isAuth()) {
		sendMessage(unauthClient.getUserFd(), RPL_NICK(unauthClient.getNickname(), nickname));
		unauthClient.setNickname(nickname);
	}
}

void Server::cmdUser(Client &unauthClient, std::stringstream &msg) {
	std::string	username;
	std::getline(msg, username);

	if (unauthClient.getPassword().empty()) {
		sendMessage(unauthClient.getUserFd(), unauthClient.getUsername() + " :You may not set USER before using PASS\n");
		return ;
	}
	if (unauthClient.getNickname().empty()) {
		sendMessage(unauthClient.getUserFd(), unauthClient.getUsername() + " :You may not set USER before using NICK\n");
		return ;
	}

	if (username.empty()) {
		sendMessage(unauthClient.getUserFd(), unauthClient.getUsername() + " :No username given\n");
		return ;
	}

	if (!checkName(username)) {
		sendMessage(unauthClient.getUserFd(), ERR_ERRONEUSNICKNAME(unauthClient.getNickname(), username));
		return ;
	}

	if (!unauthClient.isAuth()) {
		unauthClient.setUsername(username);
		sendMessage(unauthClient.getUserFd(), "Requesting the new username " + username + "\n");
		if (unauthClient.getPassword() == _password && !unauthClient.getNickname().empty()) {
			unauthClient.setAuth(true);

			sendMessage(unauthClient.getUserFd(), RPL_WELCOME(unauthClient.getNickname()));
		}
		else {
			unauthClient.clearInfo();
			sendMessage(unauthClient.getUserFd(), ERR_PASSWDMISMATCH(unauthClient.getNickname()));
		}
	}
	else {
		sendMessage(unauthClient.getUserFd(), unauthClient.getUsername() + " changed his username to " + username);
		unauthClient.setUsername(username);
	}
}
