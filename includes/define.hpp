/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 17:10:03 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/26 04:14:27 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_HPP
#define DEFINE_HPP

//MAX CLIENTS
#define MAXCLIENTS 1000

//ERROR MESSAGES
#define ERR_NOTREGISTERED(client) (": 451 " + client + " :You have not registered\r\n")
#define ERR_CMDNOTFOUND(client) (": 421 " + client + " :Command not found\r\n")
#define ERR_NOSUCHCHANNEL(client, channel) (": 403 " + client + " " + channel + " :No such channel\r\n")
#define ERR_NOSUCHNICK(client, nickname) (": 401 " + client + " " + nickname + " :No such nickname\r\n")
#define ERR_CHANOPRIVSNEEDED(client, channel) (": 482 " + client + " " + channel + " :You're not channel operator\r\n")
#define ERR_NOTONCHANNEL(client, channel) (": 442 " + client + " " + channel + " :You're not on that channel\r\n")
#define ERR_NEEDMOREPARAMS(client, cmd) (": 461 " + client + " " + cmd + " :Not enough parameters\r\n")
#define ERR_TOOMUCHPARAMS(client, cmd) (client + " " + cmd + " :Too much parameters\r\n")
#define ERR_ALREADYREGISTERED(client) (": 462 " + client + " :You may not reregister\r\n")
#define ERR_NICKNAMEINUSE(client, nick) (": 433 * " + client + " " + nick + " :Nickname is already in use\r\n")
#define ERR_CHANNELISFULL(client, channel) (": 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_INVITEONLYCHAN(client, channel) (": 473 " + client + " " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(client, channel) (": 475 " + client + " " + channel + " :Cannot join channel (+k)\r\n")
#define ERR_USERNOTINCHANNEL(client, nick, channel) (": 441 " + client + " " + nick + " " + channel + " :They aren't on that channel\r\n")
#define ERR_CANNOTSENDTOCHAN(client, channel) (": 404 " + client + " " + channel + " :Cannot send to channel\r\n")
#define ERR_USERONCHANNEL(client, nick, channel) (": 303 " + client + " " + nick + " " + channel + " :is already on channel\r\n")
#define ERR_PASSWDMISMATCH(client) (": 464 " + client + " :Password incorrect\r\n")
#define ERR_UMODEUNKNOWNFLAG(client) (": 501 " + client + " :Unknown MODE flag\r\n")
#define ERR_ERRONEUSNAME(client, nickname, name) (": 432 " + client + " " + nickname + " :Erroneus " + name + "\r\n")
#define ERR_NOTEXTTOSEND(client) (": 412 " + client + " :No text to send\r\n")
#define ERR_NORECIPIENT(client) (": 411 " + client + " :No recipient given PRIVMSG\r\n")

//PRIVMSG BUILDERS
#define CHANNEL_MESSAGES(client, channel, msg) (":" + client + " PRIVMSG " + channel + " :" + msg + "\r\n")
#define USER_MESSAGES(client, target, msg) (":" + client + " PRIVMSG " + target + " :" +  msg + "\r\n")

//SERVER REPLIES
#define RPL_JOIN(nick, user, channel) (":" + nick + "!" + user + "@" + "" + " JOIN " + channel + "\n")
#define RPL_WELCOME(client, channel) (": 332 " + client + " " + channel + " :Welcome in channel " + channel + "\r\n")
#define RPL_PART(client, channel) (":" + client + " PART " + channel + "\r\n")
#define RPL_MODE(client, channel, mode) (":" + client + " MODE " + channel + " " + mode + "\r\n")
#define RPL_CHANNELMODEIS(client, channel, mode) (": 324 " + client + " MODE " + channel + " " + mode + "\r\n")
#define RPL_KICK(target, channel, client) ("["+ target + "] You were kicked from " + channel + " by " + client + "\r\n")
#define RPL_TOPIC(client, channel, topic) (":" + client + " TOPIC " + channel + " :" + topic + "\r\n")
#define RPL_NOTOPIC(client, channel) (": 331 " + client + " " + channel + " :No topic is set\r\n")
#define RPL_INVITESNDR(client, invitee, channel) (":" + client + " " + invitee + " " + channel + "\r\n")
#define RPL_INVITERCVR(client, invitee, channel) (":" + client + " INVITE " + invitee + " :" + channel + "\r\n")
#define RPL_NICK(oldNick, newNick) (":" + oldNick + " NICK " + newNick + "\r\n")

#endif
