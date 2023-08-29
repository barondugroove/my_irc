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

		class GradeTooHighException : public std::exception {
					public :
						virtual const char* what() const throw();
		};
		class GradeTooLowException : public std::exception {
					public :
						virtual const char* what() const throw();
		};

		Client(int clientFd);	//Constructor
		~Client();				//Destructor

		void Authentify(std::string name);

		std::string	&getNickname();
		std::string	&getUsername();
		const int	&getUserFd() const;
		void		setNickname(std::string &nickname);
		void		setUsername(std::string &username);

		// The 3 level of authentication
		bool password;
		bool named;
		bool authentified;

	private :
		std::string _nickname;
		std::string _username;
		const int 	_fd;

};

#endif
