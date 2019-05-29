/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AVM.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 13:45:31 by jbeall            #+#    #+#             */
/*   Updated: 2019/05/27 15:06:54 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_HPP
# define  AVM_HPP
#include "Lexer.hpp"
#include "Operand.hpp"
#include "utility.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>
#include <stdexcept>

class RuntimeErrorException : public std::exception {
	std::string message;
	public:
		RuntimeErrorException(std::string m) {
			this->message = std::string(RED_ON) + "runtime error: " + std::string(RESET) + m;
		};
		virtual const char *what() const throw() {
			return message.c_str();
		}
};

class AVM {
	Lexer *L;
	std::vector<std::string> program;
	std::vector<const IOperand*> stack;
	void readProg(std::istream &);
	void push(IOperand const *);
	void pop(void);
	void dump(void);
	void assert(IOperand const *);
	void add(void);
	void sub(void);
	void mul(void);
	void div(void);
	void mod(void);
	void print(void);
	void exit(size_t i, const std::vector<Token*> &tokens);
	public:
		AVM(std::istream &);
		~AVM();
		void writeProg(std::ostream &);
		void run(bool);
};

#endif
