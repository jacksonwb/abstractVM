/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eOperandType.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 11:58:48 by jbeall            #+#    #+#             */
/*   Updated: 2019/05/27 15:05:05 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EOPERANDTYPE_HPP
# define EOPERANDTYPE_HPP

#define RED_ON "\033[1;31m"
#define RESET "\033[0m"

enum class eOperandType {
	Int8 = 1,
	Int16,
	Int32,
	Float,
	Double
};

#endif