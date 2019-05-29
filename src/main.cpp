/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 13:44:10 by jbeall            #+#    #+#             */
/*   Updated: 2019/05/27 21:20:23 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "AVM.hpp"

int main(int argc, char **argv)
{
	bool debug = false;
	std::istream *in;
	if (argc > 3) {
		std::cout << "usage: ./abstractVM [-d] [file]" << std::endl;
		exit(0);
	}
	else if (argc == 3) {
		if(!std::string(argv[1]).compare("-d")) {
			debug = true;
			in = new std::ifstream(argv[2]);
		} else {
			std::cout << "usage: ./abstractVM [-d] [file]" << std::endl;
			exit(0);
		}
	}
	else if (argc == 2)
		in = new std::ifstream(argv[1]);
	else
		in = &std::cin;
	AVM vm(*in);
	vm.run(debug);
	return (0);
}