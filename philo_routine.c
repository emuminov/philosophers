/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:10:59 by emuminov          #+#    #+#             */
/*   Updated: 2024/04/29 18:51:17 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks_from_left(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	write_status(philo, FORK, 0);
	pthread_mutex_lock(&philo->right_fork);
	write_status(philo, FORK, 0);
}

void	take_forks_from_right(t_philo *philo)
{
	pthread_mutex_lock(&philo->right_fork);
	write_status(philo, FORK, 0);
	pthread_mutex_lock(philo->left_fork);
	write_status(philo, FORK, 0);
}

static void	alternate(t_philo *philo)
{
	philo->take_forks(philo);
	if (get_or_set_is_running(philo->params, -1, GET))
	{
		write_status(philo, EAT, 0);
		pthread_mutex_lock(&philo->meal_lock);
		philo->last_meal_time = get_time();
		philo->meals_counter++;
		pthread_mutex_unlock(&philo->meal_lock);
		ft_usleep(philo->params->time_to_eat);
	}
	pthread_mutex_unlock(&philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	if (get_or_set_is_running(philo->params, -1, GET))
	{
		write_status(philo, SLEEP, 0);
		ft_usleep(philo->params->time_to_sleep);
	}
	if (get_or_set_is_running(philo->params, -1, GET))
		write_status(philo, THINK, 0);
}

void	*solo_routine(void *data)
{
	t_philo			*philo;
	unsigned long	start_time;

	philo = (t_philo *)data;
	start_time = get_time();
	philo->last_meal_time = start_time;
	philo->params->start_time = start_time;
	philo->params->time_is_synced = true;
	write_status(philo, THINK, false);
	write_status(philo, FORK, false);
	ft_usleep(philo->params->time_to_die);
	write_status(philo, DIE, true);
	return (NULL);
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	get_or_increment_threads_ready(philo->params, SET);
	wait_for_all_threads(philo->params);
	wait_for_time_sync(philo->params);
	write_status(philo, THINK, false);
	if (philo->index % 2 == 0)
		ft_usleep(10);
	while (get_or_set_is_running(philo->params, -1, GET) == 1)
		alternate(philo);
	return (NULL);
}
