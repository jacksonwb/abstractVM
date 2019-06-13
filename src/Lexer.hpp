/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jackson <jbeall@student.42.us.org>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 13:45:31 by jbeall            #+#    #+#             */
/*   Updated: 2019/06/12 20:58:06 by jackson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
# define  LEXER_HPP
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include "utility.hpp"

class Token {
	public:
		enum eTokenType {unknown, cmd, operand};
		enum class eCmdTokenType {invalid, push, pop, dump, assert, add, sub, mul, div, mod, print, exit};
		eTokenType t;
		Token();
		virtual ~Token() {};
		virtual void parseToken(std::string&) = 0;
		virtual void printToken() = 0;
		virtual bool needsOp() = 0;
		virtual enum eCmdTokenType getCmdType() const = 0;
};

class CmdToken : public Token {
	bool needs_op = 0;
	eCmdTokenType cmd_type = eCmdTokenType::invalid;
	public:
		CmdToken(std::string&);
		~CmdToken() {};
		std::string cmd;
		virtual void parseToken(std::string&);
		virtual void printToken();
		virtual bool needsOp();
		virtual enum eCmdTokenType getCmdType() const;
};

class OperandToken : public Token {
	public:
		OperandToken(std::string&);
		~OperandToken() {};
		std::string value;
		eOperandType op;
		virtual void parseToken(std::string&);
		void parseOperandVal(std::string&);
		virtual void printToken();
		virtual bool needsOp() {return false;}
		virtual enum eCmdTokenType getCmdType() const;
		virtual enum eOperandType getOpType() const;

};

class SyntaxErrorException : public std::exception {
	std::string message;
	public:
		SyntaxErrorException(std::string m) {
			this->message = std::string(RED_ON) + "syntax error: " + std::string(RESET) + m;
		};
		virtual const char *what() const throw() {
			return message.c_str();
		}
};

class Lexer {
	unsigned line_n = 0;
	void parseLine(std::stringstream &);
	std::vector<Token*> tokens;
	std::vector<std::string> errors;
	public:
		Lexer(std::vector<std::string> &);
		~Lexer();
		void printTokens(void);
		void printErrors(void);
		const std::vector<Token*> &getTokens (void) const;
		bool hasErrors(void) const;
};

#endif
