/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:45:04 by emuminov          #+#    #+#             */
/*   Updated: 2024/04/26 15:12:20 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	ft_isdigit(int d)
{
	return (d >= '0' && d <= '9');
}

bool	ft_isspace(int c)
{
	return (c == '\t' || c == ' ' || c == '\r' || c == '\v' || c == '\n'
		|| c == '\f');
}

long	ft_atol(const char *str)
{
	long long	result;
	short		sign;
	size_t		i;

	result = 0;
	i = 0;
	while (ft_isspace(str[i]))
		i++;
	sign = ((str[i] == '-') * -1) + ((str[i] != '-') * 1);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && ft_isdigit(str[i]))
		result = (result * 10) + (str[i++] - '0');
	return (result * sign);
}

void	ft_putstr_err(char *msg)
{
	size_t	len;

	len = 0;
	while (msg[len])
		len++;
	write(STDERR_FILENO, msg, len);
}
