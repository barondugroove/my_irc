/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 14:48:28 by bchabot           #+#    #+#             */
/*   Updated: 2023/08/29 14:48:28 by bchabot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <iostream>

Channel::Channel(std::string name, Client channelCreator) : _channelName(name) {
	std::cout<< "Channel Constructor called"<< std::endl;
	_operator = channelCreator.getNickname();
	_inviteMode = false;
	_topicMode = false;
	_passwordMode = false;
	_userLimit = -1;
	return;
}

Channel::~Channel(void) {
	std::cout<< "Channel Destructor called"<< std::endl;
	return;
}
