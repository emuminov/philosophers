/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:15:56 by emuminov          #+#    #+#             */
/*   Updated: 2024/05/04 18:07:56 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	philo_should_be_dead(t_philo *philo, t_params *p)
{
	unsigned long	last_meal_time_diff;

	pthread_mutex_lock(&philo->meal_lock);
	last_meal_time_diff = diff(philo->last_meal_time);
	pthread_mutex_unlock(&philo->meal_lock);
	return (last_meal_time_diff >= p->time_to_die);
}

static bool	one_of_philos_is_dead(t_params *p)
{
	unsigned int	i;

	i = 0;
	while (i < p->philo_nbr)
	{
		if (philo_should_be_dead(&p->philos[i], p))
		{
			get_or_set_is_running(p, false, SET);
			write_status(&p->philos[i], DIE, 1);
			return (true);
		}
		i++;
	}
	return (false);
}

static int	philos_ate_all_their_meals(t_params *p)
{
	unsigned int	i;

	i = 0;
	while (i < p->philo_nbr)
	{
		if (get_or_set_meals_counter(&p->philos[i], -1,
				GET) < p->max_nbr_of_meals)
			return (false);
		i++;
	}
	get_or_set_is_running(p, false, SET);
	return (true);
}

static void	sync_time(t_params *p)
{
	unsigned long	start_time;
	unsigned int	i;

	start_time = get_time();
	i = 0;
	while (i < p->philo_nbr)
	{
		pthread_mutex_lock(&p->philos[i].meal_lock);
		p->philos[i].last_meal_time = start_time;
		pthread_mutex_unlock(&p->philos[i].meal_lock);
		i++;
	}
	pthread_mutex_lock(&p->sync_lock);
	p->start_time = start_time;
	p->time_is_synced = true;
	pthread_mutex_unlock(&p->sync_lock);
}

void	*monitor_routine(void *data)
{
	t_params	*p;

	p = (t_params *)data;
	if (p->philo_nbr == 1)
		return (NULL);
	wait_for_all_threads(p);
	sync_time(p);
	while (1)
	{
		if (one_of_philos_is_dead(p) || philos_ate_all_their_meals(p))
			break ;
		ft_usleep(1);
	}
	return (NULL);
}
