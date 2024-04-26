/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:37:12 by emuminov          #+#    #+#             */
/*   Updated: 2024/04/26 15:06:27 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_numeric(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

static enum e_error	validate_input_bounds(char **argv)
{
	long	philo_nbr;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	max_nbr_of_meals;

	philo_nbr = ft_atol(argv[1]);
	time_to_die = ft_atol(argv[2]);
	time_to_eat = ft_atol(argv[3]);
	time_to_sleep = ft_atol(argv[4]);
	if (argv[5])
		max_nbr_of_meals = ft_atol(argv[5]);
	if (philo_nbr <= 0 || philo_nbr > MAX_PHILO_NUMBER)
		return (BAD_PHILO_NUM);
	else if (time_to_die <= 0 || time_to_die > INT_MAX)
		return (BAD_DIE_TIME);
	else if (time_to_eat <= 0 || time_to_eat > INT_MAX)
		return (BAD_EAT_TIME);
	else if (time_to_sleep <= 0 || time_to_sleep > INT_MAX)
		return (BAD_SLEEP_TIME);
	else if (argv[5] && (max_nbr_of_meals <= 0 || max_nbr_of_meals > INT_MAX))
		return (BAD_MEALS_NUM);
	return (NO_ERRORS);
}

enum e_error	validate_input(char **argv)
{
	int	i;

	i = 0;
	while (argv[++i])
		if (!is_numeric(argv[i]))
			return (NON_NUMERIC);
	return (validate_input_bounds(argv));
}
