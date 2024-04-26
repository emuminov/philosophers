/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:20:05 by emuminov          #+#    #+#             */
/*   Updated: 2024/04/26 15:21:39 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	// TODO: protect error
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
