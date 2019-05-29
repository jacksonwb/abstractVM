/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AVM.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 13:45:31 by jbeall            #+#    #+#             */
/*   Updated: 2019/05/27 14:40:42 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AVM.hpp"

AVM::AVM(std::istream &in) {
	readProg(in);
	L = new Lexer(program);
};

void AVM::readProg(std::istream &in) {
	std::string line;
	size_t pos = 0;
	bool isStdIn = typeid(in) == typeid(std::cin) ? true : false;
	while (getline(in, line)) {
		if (isStdIn && (pos = line.find(";;")) != std::string::npos) {
			program.push_back(line.substr(0, pos));
			break;
		}
		program.push_back(line);
	}
}

void AVM::writeProg(std:: ostream &out) {
	for (std::vector<std::string>::iterator it = program.begin(); it != program.end(); it++)
		out << *it << std::endl;
}

void AVM::push(IOperand const *op) {
	stack.push_back(op);
}

void AVM::pop(void) {
	if (!stack.size())
		throw RuntimeErrorException("pop - empty stack");
	else {
		stack.pop_back();
	}
}

void AVM::dump(void) {
	auto it = stack.rbegin();
	while(it != stack.rend()) {
		std::cout << (*it)->toString() << std::endl;
		it++;
	}
}

void AVM::assert(IOperand const *op) {
	if (!stack.size())
		throw RuntimeErrorException("assert - empty stack");
	IOperand const *stack_op = stack.back();
	if (*stack_op != *op)
		throw RuntimeErrorException("assert - fail");
}

void AVM::add(void) {
	if (stack.size() < 2)
		throw RuntimeErrorException("add - not enough opperands");
	const IOperand *second = stack.back();
	stack.pop_back();
	const IOperand *first = stack.back();
	stack.pop_back();
	stack.push_back(*first + *second);
}

void AVM::sub(void) {
	if (stack.size() < 2)
		throw RuntimeErrorException("sub - not enough opperands");
	const IOperand *second = stack.back();
	stack.pop_back();
	const IOperand *first = stack.back();
	stack.pop_back();
	stack.push_back(*first - *second);
}

void AVM::mul(void) {
	if (stack.size() < 2)
		throw RuntimeErrorException("mul - not enough opperands");
	const IOperand *second = stack.back();
	stack.pop_back();
	const IOperand *first = stack.back();
	stack.pop_back();
	stack.push_back(*first * *second);
}

void AVM::div(void) {
	if (stack.size() < 2)
		throw RuntimeErrorException("div - not enough opperands");
	const IOperand *second = stack.back();
	stack.pop_back();
	if (std::stod(second->toString()) == 0.0)
		throw RuntimeErrorException("divide by zero");
	const IOperand *first = stack.back();
	stack.pop_back();
	stack.push_back(*first / *second);
}

void AVM::mod(void) {
	if (stack.size() < 2)
		throw RuntimeErrorException("mod - not enough opperands");
	const IOperand *second = stack.back();
	stack.pop_back();
	if (std::stod(second->toString()) == 0.0)
		throw RuntimeErrorException("divide by zero");
	const IOperand *first = stack.back();
	stack.pop_back();
	stack.push_back(*first % *second);
}

void AVM::print(void) {
	if (stack.empty())
		throw RuntimeErrorException("print - not enough opperands");
	const IOperand *top = stack.back();
	if (top->getType() != eOperandType::Int8)
		throw RuntimeErrorException("print - must be int8 value");
	std::cout << static_cast<char>(std::stoi(top->toString()));
}

void AVM::exit(size_t i, const std::vector<Token*> &tokens) {
	if (i < tokens.size() - 1)
		throw RuntimeErrorException("exit - not all instructions processed");
}

void AVM::run(bool verbose) {
	if (verbose)
		L->printTokens();
	if (L->hasErrors()) {
		L->printErrors();
		return;
	}
	const std::vector<Token*> &tokens = L->getTokens();
	try
	{
		for (size_t i = 0; i < tokens.size(); i++) {
			if (tokens[i]->t == Token::cmd) {
				switch(tokens[i]->getCmdType())
				{
					case Token::eCmdTokenType::push:
						push(IOperand::createOperand(tokens[++i]));
						break;
					case Token::eCmdTokenType::pop:
						pop();
						break;
					case Token::eCmdTokenType::dump:
						dump();
						break;
					case Token::eCmdTokenType::assert:
						assert(IOperand::createOperand(tokens[++i]));
						break;
					case Token::eCmdTokenType::add:
						add();
						break;
					case Token::eCmdTokenType::sub:
						sub();
						break;
					case Token::eCmdTokenType::mul:
						mul();
						break;
					case Token::eCmdTokenType::div:
						div();
						break;
					case Token::eCmdTokenType::mod:
						mod();
						break;
					case Token::eCmdTokenType::print:
						print();
						break;
					case Token::eCmdTokenType::exit:
						exit(i, tokens);
						return;
					default:
						throw RuntimeErrorException("invalid command");
				}
			}
		}
		throw RuntimeErrorException("no exit instruction");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

AVM::~AVM() {};
