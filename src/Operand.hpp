/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operand.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 13:45:31 by jbeall            #+#    #+#             */
/*   Updated: 2019/05/28 20:28:44 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERAND_HPP
# define  OPERAND_HPP
#include <string>
#include <limits>
#include <stdexcept>
#include <cmath>
#include "Lexer.hpp"
#include "utility.hpp"

class OperandOverflowExceptions : public std::exception {
	std::string message;
	public:
		OperandOverflowExceptions(std::string type) {
			this->message = std::string(RED_ON) + "runtime error:" +
				std::string(RESET) + " overflow creating value of type: " + type;
		}
		virtual const char *what() const throw() {
			return message.c_str();
		}
};

class OperandUnderflowExceptions : public std::exception {
	std::string message;
	public:
		OperandUnderflowExceptions(std::string type) {
			this->message = std::string(RED_ON) + "runtime error:" +
				std::string(RESET) + " underflow creating value of type: " + type;
		}
		virtual const char *what() const throw() {
			return message.c_str();
		}
};

class IOperand {
	static const IOperand* createInt8(std::string const &value);
	static const IOperand* createInt16(std::string const &value);
	static const IOperand* createInt32(std::string const &value);
	static const IOperand* createFloat(std::string const &value);
	static const IOperand* createDouble(std::string const &value);
	public:
		static IOperand const * createOperand( eOperandType type, std::string const & value );
		static IOperand const * createOperand(Token*);
		virtual int getPrecision( void ) const = 0;
		virtual eOperandType getType( void ) const = 0;
		virtual IOperand const * operator+( IOperand const & rhs ) const = 0;
		virtual IOperand const * operator-( IOperand const & rhs ) const = 0;
		virtual IOperand const * operator*( IOperand const & rhs ) const = 0;
		virtual IOperand const * operator/( IOperand const & rhs ) const = 0;
		virtual IOperand const * operator%( IOperand const & rhs ) const = 0;
		virtual bool operator==(IOperand const &rhs) const = 0;
		virtual bool operator!=(IOperand const &rhs) const = 0;
		virtual std::string const & toString( void ) const = 0;
		virtual ~IOperand( void ) {}
};

template <class T>
class Operand : public IOperand {
	std::string m_str;
	public:
		const T m_value;
		Operand(void) : m_str("0"), m_value(0) {};
		Operand(T v) : m_str(std::to_string(v)), m_value(v) {};
		Operand(Operand const &in);
		int getPrecision( void ) const;
		eOperandType getType( void ) const;
		IOperand const *operator+(IOperand const &rhs) const;
		IOperand const * operator-( IOperand const & rhs ) const;
		IOperand const * operator*( IOperand const & rhs ) const;
		IOperand const * operator/( IOperand const & rhs ) const;
		IOperand const * operator%( IOperand const & rhs ) const;
		bool operator==(IOperand const &rhs) const;
		bool operator!=(IOperand const &rhs) const;
		std::string const & toString( void ) const;
		~Operand(void) {};
};

template <class T>
std::string const &Operand<T>::toString(void) const {
	return (m_str);
}
// template <class T>
// Operand<T>::Operand() {};

// template <class T>
// Operand<T>::~Operand() {};

template<class T>
IOperand const *Operand<T>::operator+(IOperand const &rhs) const {
	double rhs_val = std::stod(rhs.toString());
	if (rhs.getPrecision() > this->getPrecision())
		return IOperand::createOperand(rhs.getType(), std::to_string(rhs_val + this->m_value));
	return IOperand::createOperand(this->getType(), std::to_string(rhs_val + this->m_value));
}

template<class T>
IOperand const *Operand<T>::operator-(IOperand const &rhs) const {
	double rhs_val = std::stod(rhs.toString());
	if (rhs.getPrecision() > this->getPrecision())
		return IOperand::createOperand(rhs.getType(), std::to_string(rhs_val - this->m_value));
	return IOperand::createOperand(this->getType(), std::to_string(rhs_val - this->m_value));
}

template<class T>
IOperand const *Operand<T>::operator*(IOperand const &rhs) const {
	double rhs_val = std::stod(rhs.toString());
	if (rhs.getPrecision() > this->getPrecision())
		return IOperand::createOperand(rhs.getType(), std::to_string(rhs_val * this->m_value));
	return IOperand::createOperand(this->getType(), std::to_string(rhs_val * this->m_value));
}

template<class T>
IOperand const *Operand<T>::operator/(IOperand const &rhs) const {
	double rhs_val = std::stod(rhs.toString());
	if (rhs.getPrecision() > this->getPrecision())
		return IOperand::createOperand(rhs.getType(), std::to_string(this->m_value / rhs_val));
	return IOperand::createOperand(this->getType(), std::to_string(this->m_value / rhs_val));
}

template<class T>
IOperand const *Operand<T>::operator%(IOperand const &rhs) const {
	double rhs_val = std::stod(rhs.toString());
	if (rhs.getPrecision() > this->getPrecision())
		return IOperand::createOperand(rhs.getType(), std::to_string(fmod(this->m_value, rhs_val)));
	return IOperand::createOperand(this->getType(), std::to_string(fmod(this->m_value, rhs_val)));
}

template<class T>
bool Operand<T>::operator==(IOperand const &rhs) const {
	if (this->getType() != rhs.getType())
		return false;
	if (this->getType() == eOperandType::Float)
		return this->m_value == std::stof(rhs.toString());
	else
		return this->m_value == std::stod(rhs.toString());
	return true;
}

template<class T>
bool Operand<T>::operator!=(IOperand const &rhs) const {
	return (!(*this == rhs));
}

#endif
