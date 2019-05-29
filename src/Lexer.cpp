/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 13:45:31 by jbeall            #+#    #+#             */
/*   Updated: 2019/05/27 14:18:42 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

Token::Token() : t(Token::unknown) {};

CmdToken::CmdToken(std::string &in) {
	t = Token::cmd;
	parseToken(in);
};

void CmdToken::parseToken(std::string &in) {
	bool valid = 0;
	std::string cmds[] = {
		"push",
		"pop",
		"dump",
		"assert",
		"add",
		"sub",
		"mul",
		"div",
		"mod",
		"print",
		"exit" };
		for (int i = 0; i < 11; i++) {
			if (in == cmds[i]) {
				valid = true;
				cmd_type = eCmdTokenType(i + 1);
			}
		}
		if (!valid)
			throw SyntaxErrorException("invalid command");
		this->cmd = in;
		if (cmd == "push" || cmd == "assert")
			needs_op = true;
}

void CmdToken::printToken(void) {
	std::cout << "TOKEN" << '[' << '"' << "cmd" << "\", \"" << cmd << "\"]"
		<< std::endl;
}

bool CmdToken::needsOp(void) {
	return needs_op;
}

enum Token::eCmdTokenType CmdToken::getCmdType() const {
	return cmd_type;
}

OperandToken::OperandToken(std::string &in) {
	t = Token::operand;
	parseToken(in);
}

void OperandToken::parseOperandVal(std::string &in) {
	size_t i = 0;
	if (in[i] == '-')
		i++;
	while (isdigit(in[i]) && i < in.length())
		i++;
	if (this->op == eOperandType::Float || this->op == eOperandType::Double) {
		if (in[i] == '.') {
			i++;
			while (isdigit(in[i]) && i < in.length())
				i++;
		}
	}
	this->value = in.substr(0, i);
	in.erase(0, i);
}

void OperandToken::parseToken(std::string &in) {
	bool valid = 0;
	std::string ops[] = {
		"int8",
		"int16",
		"int32",
		"float",
		"double"
	};
	size_t i = 0;
	while (isalnum(in[i]) && i < in.length())
		i++;
	std::string sub = in.substr(0, i);
	int j = 1;
	for (auto &opStr : ops) {
		if (sub == opStr) {
			valid = 1;
			this->op = eOperandType(j);
			break;
		}
		j++;
	}
	if (!valid)
		throw SyntaxErrorException("invalid operand type");
	if (valid && in[i] == '(') {
		in.erase(0, i + 1);
		parseOperandVal(in);
	}
	else
		throw SyntaxErrorException("expected '('");
	if (in.length() > 1)
		throw SyntaxErrorException("invalid value character");
}

void OperandToken::printToken() {
	std::cout << "TOKEN" << '[' << '"' << "operand" << "\", \"" << (int)op
		<< "\", \"" << value << "\"]" << std::endl;
}

enum Token::eCmdTokenType OperandToken::getCmdType() const {
	return eCmdTokenType::invalid;
}

enum eOperandType OperandToken::getOpType() const {
	return op;
}

Lexer::Lexer(std::vector<std::string> &program) {
	for (auto &line : program) {
		std::stringstream ss;
		++line_n;
		if (line.front() == ';' || std::all_of(line.begin(), line.end(), isspace))
			continue;
		if (line.find(';') != std::string::npos) {
			line = line.substr(0, line.find(';'));
			line.erase(line.find_last_not_of(" \n\r\t") + 1);
		}
		ss.str(line);
		parseLine(ss);
	}
};

Lexer::~Lexer() {};

void Lexer::parseLine(std::stringstream &line) {
	std::string word;
	line >> word;
	try
	{
		tokens.push_back(new CmdToken(word));
		if(!line.eof()) {
			line >> word;
			if (!tokens.back()->needsOp())
				throw SyntaxErrorException("no operand expected");
			tokens.push_back(new OperandToken(word));
			if (word.back() != ')')
				throw SyntaxErrorException("expected ')'");
		} else {
			if (tokens.back()->needsOp())
				throw SyntaxErrorException("operand needed");
		}
		if(!line.eof()) {
			line >> word;
			if(!std::all_of(word.begin(), word.end(), isspace))
				throw SyntaxErrorException("no operand expected");
		}
	}
	catch(const std::exception& e)
	{
		errors.push_back("line " + std::to_string(line_n) + ": " + e.what());
	}
}

void Lexer::printTokens(void) {
	std::cout << "----TOKENS----" << std::endl;
	for (auto &token : tokens) {
		token->printToken();
	}
	std::cout << "--END TOKENS--" << std::endl;
}

void Lexer::printErrors(void) {
	for (auto &error : errors) {
		std::cout << error << std::endl;
	}
}

const std::vector<Token*> &Lexer::getTokens (void) const {
	return tokens;
}

bool Lexer::hasErrors (void) const {
	return (errors.size() ? true : false);
}
