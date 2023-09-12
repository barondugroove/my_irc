/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 17:10:03 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/12 18:06:13 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_HPP
#define DEFINE_HPP

//ERROR MESSAGES
#define ERR_NOTREGISTERED(client) (client + " :You have not registered\n")
#define ERR_CMDNOTFOUND(client) (client + " :Command not found\n")
#define ERR_NOSUCHCHANNEL(client, channel) (client + " " + channel + " :No such channel\n")
#define ERR_CHANOPRIVSNEEDED(client, channel) (client + " " + channel + " :You're not channel operator\n")
#define ERR_NOTONCHANNEL(client, channel) (client + " " + channel + " :You're not on that channel\n")
#define ERR_NEEDMOREPARAMS(client, cmd) (client + " " + cmd + " :Not enough parameters\n")
#define ERR_ALREADYREGISTERED(client) (client + " :You may not reregister\n")
#define ERR_NICKNAMEINUSE(client, nick) (client + " " + nick + " :Nickname is already in use\n")
#define ERR_CHANNELISFULL(client, channel) (client + " " + channel + " :Cannot join channel (+l)\n")
#define ERR_INVITEONLYCHAN(client, channel) (client + " " + channel + " :Cannot join channel (+i)\n")
#define ERR_BADCHANNELKEY(client, channel) (client + " " + channel + " :Cannot join channel (+k)\n")
#define ERR_USERNOTINCHANNEL(client, nick, channel) (client + " " + nick + " " + channel + " :They aren't on that channel\n")
#define ERR_CANNOTSENDTOCHAN(client, channel) (client + " " + channel + " :Cannot send to channel\n")
#define ERR_USERONCHANNEL(client, nick, channel) (client + " " + nick + " " + channel + " :is already on channel\n")
#define ERR_PASSWDMISMATCH(client) (client + " :Password incorrect\n")
#define ERR_UMODEUNKNOWNFLAG(client) (client + " :Unknown MODE flag\n")

//PRIVMSG BUILDERS
#define CHANNEL_MESSAGES(client, nick, msg) (":" + client + " PRIVMSG " + nick + " :" + msg + "\r\n")
#define USER_MESSAGES(client, msg) ("Message from " + client + " : \n" + msg + '\n')

//SERVER REPLIES
#define RPL_JOIN(client, channel) (":" + client + " JOIN " + channel + "\r\n")
#define RPL_PART(client, channel) (":" + client + " PART " + channel + "\r\n")
#define RPL_MODE(client, channel, mode) (":" + client + " " + channel + " " + mode + "\r\n")

#endif
