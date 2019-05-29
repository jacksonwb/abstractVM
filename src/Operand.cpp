/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operand.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 13:45:31 by jbeall            #+#    #+#             */
/*   Updated: 2019/05/28 20:26:51 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Operand.hpp"

const IOperand* IOperand::createInt8(std::string const &value) {
	int val;
	try
	{
		val = std::stoi(value);
	}
	catch (std::exception &e)
	{
		if (value[0] == '-')
			throw OperandUnderflowExceptions("int8");
		throw OperandOverflowExceptions("int8");
	}
	if (val > std::numeric_limits<int8_t>::max())
		throw OperandOverflowExceptions("int8");
	if (val < std::numeric_limits<int8_t>::min())
		throw OperandUnderflowExceptions("int8");
	return new Operand<int8_t>(val);
}

const IOperand* IOperand::createInt16(std::string const &value) {
	long val;
	try
	{
		val = std::stol(value);
	}
	catch (std::exception &e)
	{
		if (value[0] == '-')
			throw OperandUnderflowExceptions("int16");
		throw OperandOverflowExceptions("int16");
	}
	if (val > std::numeric_limits<int16_t>::max())
		throw OperandOverflowExceptions("int16");
	if (val < std::numeric_limits<int16_t>::min())
		throw OperandUnderflowExceptions("int16");
	return new Operand<int16_t>(val);
}

const IOperand* IOperand::createInt32(std::string const &value) {
	long long val;
	try
	{
		val = std::stoll(value);
	}
	catch (std::exception &e)
	{
		if (value[0] == '-')
			throw OperandUnderflowExceptions("int32");
		throw OperandOverflowExceptions("int32");
	}
	if (val > std::numeric_limits<int32_t>::max())
		throw OperandOverflowExceptions("int32");
	if (val < std::numeric_limits<int32_t>::min())
		throw OperandUnderflowExceptions("int32");
	return new Operand<int32_t>(val);
}

const IOperand* IOperand::createFloat(std::string const &value) {
	float val;
	try
	{
		val = std::stof(value);
	}
	catch (std::exception &e)
	{
		if (value[0] == '-')
			throw OperandUnderflowExceptions("float");
		throw OperandOverflowExceptions("float");
	}
	return new Operand<float>(val);
}

const IOperand* IOperand::createDouble(std::string const &value) {
	double val;
	try
	{
		val = std::stod(value);
	}
	catch (std::exception &e)
	{
		if (value[0] == '-')
			throw OperandUnderflowExceptions("double");
		throw OperandOverflowExceptions("double");
	}
	return new Operand<double>(val);
}

IOperand const * IOperand::createOperand( eOperandType type, std::string const & value ) {
	const IOperand* (*dispatch[])(std::string const &value) = {
		&IOperand::createInt8,
		&IOperand::createInt16,
		&IOperand::createInt32,
		&IOperand::createFloat,
		&IOperand::createDouble
	};
	return dispatch[(int)type - 1](value);
}

IOperand const * IOperand::createOperand(Token *t) {
	eOperandType type = dynamic_cast<const OperandToken*>(t)->getOpType();
	const std::string &value = dynamic_cast<const OperandToken*>(t)->value;
	const IOperand* (*dispatch[])(std::string const &value) = {
		&IOperand::createInt8,
		&IOperand::createInt16,
		&IOperand::createInt32,
		&IOperand::createFloat,
		&IOperand::createDouble
	};
	return dispatch[(int)type - 1](value);
}

template <>
int Operand<int8_t>::getPrecision(void) const {return 1;}

template <>
int Operand<int16_t>::getPrecision(void) const {return 2;}

template <>
int Operand<int32_t>::getPrecision(void) const {return 3;}

template <>
int Operand<float>::getPrecision(void) const {return 4;}

template <>
int Operand<double>::getPrecision(void) const {return 5;}

template <>
eOperandType Operand<int8_t>::getType(void) const {return eOperandType::Int8;}

template <>
eOperandType Operand<int16_t>::getType(void) const {return eOperandType::Int16;}

template <>
eOperandType Operand<int32_t>::getType(void) const {return eOperandType::Int32;}

template <>
eOperandType Operand<float>::getType(void) const {return eOperandType::Float;}

template <>
eOperandType Operand<double>::getType(void) const {return eOperandType::Double;}