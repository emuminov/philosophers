/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:20:05 by emuminov          #+#    #+#             */
/*   Updated: 2024/04/29 15:57:18 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

unsigned long	diff(unsigned long timestamp)
{
	return (get_time() - timestamp);
}

void	ft_usleep(unsigned long ms)
{
	unsigned long	start;
	unsigned long	curr;

	start = get_time();
	curr = get_time();
	while ((curr - start) < ms)
	{
		curr = get_time();
		usleep(50);
	}
}

void	wait_for_all_threads(t_params *p)
{
	while (get_or_increment_threads_ready(p, GET) < p->philo_nbr)
		;
}

void	wait_for_time_sync(t_params *p)
{
	while (get_or_set_time_is_synced(p, -1, GET) == false)
		;
}
