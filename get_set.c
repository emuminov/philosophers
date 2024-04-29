/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:29:45 by emuminov          #+#    #+#             */
/*   Updated: 2024/04/29 15:48:27 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned int	get_or_set_is_running(t_params *p, int val, enum e_flag f)
{
	int	is_running;

	pthread_mutex_lock(&p->sync_lock);
	if (f == SET)
		p->is_running = val;
	is_running = p->is_running;
	pthread_mutex_unlock(&p->sync_lock);
	return (is_running);
}

unsigned int	get_or_set_meals_counter(t_philo *philo, int val, enum e_flag f)
{
	unsigned int	meals_counter;

	pthread_mutex_lock(&philo->meal_lock);
	if (f == SET)
		philo->meals_counter = val;
	meals_counter = philo->meals_counter;
	pthread_mutex_unlock(&philo->meal_lock);
	return (meals_counter);
}

unsigned int	get_or_increment_threads_ready(t_params *p, enum e_flag f)
{
	unsigned int	threads_ready;

	pthread_mutex_lock(&p->sync_lock);
	if (f == SET)
		p->threads_ready++;
	threads_ready = p->threads_ready;
	pthread_mutex_unlock(&p->sync_lock);
	return (threads_ready);
}

bool	get_or_set_time_is_synced(t_params *p, int val, enum e_flag f)
{
	bool	time_is_synced;

	pthread_mutex_lock(&p->sync_lock);
	if (f == SET)
		p->time_is_synced = val;
	time_is_synced = p->time_is_synced;
	pthread_mutex_unlock(&p->sync_lock);
	return (time_is_synced);
}
