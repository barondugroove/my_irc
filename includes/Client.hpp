/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlaforge <rlaforge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 02:09:24 by rlaforge          #+#    #+#             */
/*   Updated: 2023/07/30 02:09:24 by rlaforge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client{
	public:
		Client(int clientFd);					//Constructor
		Client(const Client &src); 				//Copy Constructor
		Client & operator=(const Client &rhs);	//Assignement Value Operator
		~Client();								//Destructor

		std::string	&getNickname()		{return _nickname;};
		std::string	&getUsername()		{return _username;};
		const int	&getUserFd() const	{return _fd;};
		std::string	&getPassword()		{return _password;};

		bool		isAuth()			{return _authentified;};

		void		setNickname(std::string nickname);
		void		setUsername(std::string username);
		void		setPassword(std::string password);
		void		setAuth(bool status);
		void		clearInfo();

	private :
		bool		_authentified;
		const int 	_fd;

		std::string _nickname;
		std::string _username;
		std::string	_password;

};

#endif
