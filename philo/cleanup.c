/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 17:02:36 by emuminov          #+#    #+#             */
/*   Updated: 2024/05/04 18:11:44 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup(t_params *p)
{
	unsigned int	i;

	i = 0;
	while (i < p->philo_nbr)
	{
		pthread_join(p->philos[i].th, NULL);
		i++;
	}
	pthread_join(p->monitor_th, NULL);
	pthread_mutex_destroy(&p->sync_lock);
	pthread_mutex_destroy(&p->write_lock);
	i = 0;
	while (i < p->philo_nbr)
	{
		pthread_mutex_destroy(&p->philos[i].meal_lock);
		pthread_mutex_destroy(&p->philos[i].right_fork.fork_mtx);
		i++;
	}
}
