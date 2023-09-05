/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 14:26:42 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/05 19:03:40 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <string>
#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>

class Channel {
	public:
		Channel(std::string name, Client &channelCreator);		//Constructor
		Channel(const Channel &src); //Copy Constructor
		Channel & operator=(const Channel &rhs); //Assignement Value Operator
		~Channel();												//Destructor

		std::string getChannelName() const;
		std::string	&getTopic();
		bool		getTopicMode();
		int			getUserCount() const;
		void		setTopic(std::string topic);
		void		setOperator(std::string &nickname);

		bool		isUserMember(std::string &nickName);
		bool		isUserOperator(std::string &nickName);

		void		addUser(std::string &nickName, Client &client);
		void		eraseUser(std::string &nickName);
		void		sendMessageToAllMembers(std::string msg);
		void		sendMessageToMember(std::map<std::string, Client*>::iterator it, std::string msg);
		void		changeInviteMode(bool status);


	private :
		std::string _channelName;
		std::string _topic;
		std::map<std::string, Client*> _members;

		std::vector<std::string>	_operator;
		std::string					_key;
		bool						_inviteMode;
		bool						_topicMode;
		bool						_passwordMode;
		int							_userLimit;
};

#endif
