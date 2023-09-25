/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 17:10:03 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/22 20:14:50 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_HPP
#define DEFINE_HPP

//MAX CLIENTS
#define MAXCLIENTS 1000

//ERROR MESSAGES
#define ERR_NOTREGISTERED(client) (client + " :You have not registered\r\n")
#define ERR_CMDNOTFOUND(client) (client + " :Command not found\r\n")
#define ERR_NOSUCHCHANNEL(client, channel) (client + " " + channel + " :No such channel\r\n")
#define ERR_CHANOPRIVSNEEDED(client, channel) (client + " " + channel + " :You're not channel operator\r\n")
#define ERR_NOTONCHANNEL(client, channel) (client + " " + channel + " :You're not on that channel\r\n")
#define ERR_NEEDMOREPARAMS(client, cmd) (client + " " + cmd + " :Not enough parameters\r\n")
#define ERR_TOOMUCHPARAMS(client, cmd) (client + " " + cmd + " :Too much parameters\r\n")
#define ERR_ALREADYREGISTERED(client) (client + " :You may not reregister\r\n")
#define ERR_NICKNAMEINUSE(client, nick) (client + " " + nick + " :Nickname is already in use\r\n")
#define ERR_CHANNELISFULL(client, channel) (client + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_INVITEONLYCHAN(client, channel) (client + " " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(client, channel) (client + " " + channel + " :Cannot join channel (+k)\r\n")
#define ERR_USERNOTINCHANNEL(client, nick, channel) (client + " " + nick + " " + channel + " :They aren't on that channel\r\n")
#define ERR_CANNOTSENDTOCHAN(client, channel) (client + " " + channel + " :Cannot send to channel\r\n")
#define ERR_USERONCHANNEL(client, nick, channel) (client + " " + nick + " " + channel + " :is already on channel\r\n")
#define ERR_PASSWDMISMATCH(client) (client + " :Password incorrect\r\n")
#define ERR_UMODEUNKNOWNFLAG(client) (client + " :Unknown MODE flag\r\n")
#define ERR_ERRONEUSNAME(client, nickname, name) (client + " " + nickname + " :Erroneus " + name + "\r\n")
#define ERR_NOTEXTTOSEND(client) (client + " :No text to send\r\n")
#define ERR_NORECIPIENT(client) (client + " :No recipient given PRIVMSG\r\n")

//PRIVMSG BUILDERS
#define CHANNEL_MESSAGES(client, channel, msg) (":" + client + " PRIVMSG " + channel + " :" + msg + "\r\n")
#define USER_MESSAGES(client, msg) ("Message from " + client + " : \n" + msg + '\n')

//SERVER REPLIES
#define RPL_JOIN(client, channel) (":" + client + " JOIN " + channel + "\r\n")
#define RPL_PART(client, channel) (":" + client + " PART " + channel + "\r\n")
#define RPL_MODE(client, channel, mode) (":" + client + " MODE " + channel + " " + mode + "\r\n")
#define RPL_TOPIC(client, channel, topic) (":" + client + " TOPIC " + channel + ": " + topic + "\r\n")
#define RPL_NOTOPIC(client, channel) (client + " " + channel + " :No topic is set\r\n")
#define RPL_INVITESNDR(client, invitee, channel) (":" + client + " " + invitee + " " + channel + "\r\n")
#define RPL_INVITERCVR(client, invitee, channel) (":" + client + " INVITE " + invitee + " :" + channel + "\r\n")

#endif
