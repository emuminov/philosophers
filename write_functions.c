/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:06:39 by emuminov          #+#    #+#             */
/*   Updated: 2024/04/29 16:59:30 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	write_status(t_philo *philo, enum e_status s, bool is_allowed)
{
	pthread_mutex_lock(&philo->params->write_lock);
	if ((!get_or_set_is_running(philo->params, -1, GET)) && !is_allowed)
	{
		pthread_mutex_unlock(&philo->params->write_lock);
		return ;
	}
	if (s == THINK)
		printf("%lu %u is thinking\n", (diff(philo->params->start_time)),
			philo->index);
	else if (s == FORK)
		printf("%lu %u has taken a fork\n", (diff(philo->params->start_time)),
			philo->index);
	else if (s == EAT)
		printf(GREEN "%lu %u is eating\n" RES,
			(diff(philo->params->start_time)), philo->index);
	else if (s == SLEEP)
		printf(GRAY "%lu %u is sleeping\n" RES,
			(diff(philo->params->start_time)), philo->index);
	else if (s == DIE)
		printf(RED "%lu %u died\n" RES, (diff(philo->params->start_time)),
			philo->index);
	pthread_mutex_unlock(&philo->params->write_lock);
}

void	write_error(enum e_error e)
{
	if (e == NON_NUMERIC)
		ft_putstr_err(RED "Non-numeric input\n" RES);
	else if (e == BAD_PHILO_NUM)
		ft_putstr_err(RED "Bad philo number\n" RES);
	else if (e == BAD_DIE_TIME)
		ft_putstr_err(RED "Bad die time\n" RES);
	else if (e == BAD_EAT_TIME)
		ft_putstr_err(RED "Bad eat time\n" RES);
	else if (e == BAD_SLEEP_TIME)
		ft_putstr_err(RED "Bad sleep time\n" RES);
	else if (e == BAD_MEALS_NUM)
		ft_putstr_err(RED "Bad meals number\n" RES);
	else if (e == BAD_ARGS_NUMBER)
		ft_putstr_err(RED "Usage: philos_nbr, time_to_die,"
			" time_to_eat, time_to_sleep, "
			"[number_pf_times_philosopers_must_eat]\n" RES);
}
