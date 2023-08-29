/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 14:26:42 by bchabot           #+#    #+#             */
/*   Updated: 2023/08/29 15:28:02 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <string>
#include <map>

class Channel {
	public:
		Channel(std::string name, Client channelCreator);		//Constructor
		~Channel();						//Destructor

	private :
		std::string _channelName;
		std::map<std::string, Client> _members;

		std::string		_operator;
		std::string		_password;
		bool			_inviteMode;
		bool			_topicMode;
		bool			_passwordMode;
		int				_userLimit;
};

#endif
