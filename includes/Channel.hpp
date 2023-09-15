/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlaforge <rlaforge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 14:26:42 by bchabot           #+#    #+#             */
/*   Updated: 2023/09/13 16:40:31 by rlaforge         ###   ########.fr       */
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
		Channel(std::string name, Client &channelCreator);	//Constructor
		Channel(const Channel &src); 						//Copy Constructor
		Channel & operator=(const Channel &rhs); 			//Assignement Value Operator
		~Channel();											//Destructor

		std::string getChannelName() const	{return _channelName;};
		std::string	&getTopic()				{return _topic;};
		bool		getTopicMode()			{return _topicMode;};
		int			getUserCount()			{return _members.size();};

		void		setTopic(std::string topic);
		void		setChannelPass(std::string password);
		void		setInviteMode(bool status);
		void		setTopicMode(bool status);
		void		setUserLimit(int userLimit);
		void		setPassMode(bool status);

		bool		isChannelFull();
		bool		isUserMember(std::string &nickName);
		bool		isUserOperator(std::string &nickName);
		bool		isInviteLocked(std::string &nickName);

		void		addUser(std::string &nickName, Client &client);
		void		addOperator(std::string &nickName);
		void		eraseUser(std::string &nickName);
		void		eraseOperator(std::string &nickName);
		void		sendMessageToAllMembers(std::string msg, std::string nickName);
		void		sendMessageToMember(Client &client, std::string msg);



	private :
		std::string						_channelName;
		std::string						_topic;
		std::string						_password;

		std::map<std::string, Client*>	_members;
		std::vector<std::string>		_operator;
		std::vector<std::string>		_invitees;
		
		bool							_inviteMode;
		bool							_topicMode;
		bool							_passwordMode;
		int								_userLimit;
};

#endif