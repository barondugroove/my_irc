/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdAuth.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:19:31 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/19 15:44:49 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Server.hpp"

bool checkName(std::string &test) {
	const std::string forbiddenChars = " ,*?!@.";

	if (test.empty() || test[0] == '$' || test[0] == ':' || test[0] == '#' || test[0] == '&')
		return false;
	for (size_t i = 0; i < test.size() - 1; i++) {
		if (forbiddenChars.find(test[i]) != std::string::npos)
			return false;
	}
	return true;
}

void Server::cmdPass(Client &unauthClient, std::stringstream &msg) {
	std::string	password;
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
	sendMessage(unauthClient.getUserFd(), unauthClient.getNickname() + " :password is set to " + password + "\n");
}

void Server::cmdNick(Client &unauthClient, std::stringstream &msg) {
	std::string	nickname;
	msg >> nickname;

	if (nickname.empty()) {
		sendMessage(unauthClient.getUserFd(), unauthClient.getNickname() + " :No nickname given\n");
		return ;
	}

	if (getFdByNickname(nickname) != -1) {
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
			sendMessage(unauthClient.getUserFd(), unauthClient.getNickname() + " :is now authentified\n");
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
