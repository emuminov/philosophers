/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 13:04:11 by emuminov          #+#    #+#             */
/*   Updated: 2024/05/06 15:04:56 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks_from_right(t_philo *philo)
{
	pthread_mutex_lock(&philo->right_fork.fork_mtx);
	if (!philo->right_fork.owner)
	{
		philo->right_fork.owner = philo->index;
		write_status(philo, FORK, 0);
	}
	else if (philo->right_fork.owner != philo->index)
	{
		pthread_mutex_unlock(&philo->right_fork.fork_mtx);
		ft_usleep(1);
		return ;
	}
	pthread_mutex_unlock(&philo->right_fork.fork_mtx);
	pthread_mutex_lock(&philo->left_fork->fork_mtx);
	if (!philo->left_fork->owner)
	{
		philo->left_fork->owner = philo->index;
		write_status(philo, FORK, 0);
	}
	pthread_mutex_unlock(&philo->left_fork->fork_mtx);
	ft_usleep(1);
}

void	release_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->right_fork.fork_mtx);
	philo->right_fork.owner = 0;
	pthread_mutex_unlock(&philo->right_fork.fork_mtx);
	pthread_mutex_lock(&philo->left_fork->fork_mtx);
	philo->left_fork->owner = 0;
	pthread_mutex_unlock(&philo->left_fork->fork_mtx);
}

bool	has_both_forks(t_philo *philo)
{
	bool	has_right_fork;
	bool	has_left_fork;

	pthread_mutex_lock(&philo->right_fork.fork_mtx);
	has_right_fork = philo->right_fork.owner == philo->index;
	pthread_mutex_unlock(&philo->right_fork.fork_mtx);
	pthread_mutex_lock(&philo->left_fork->fork_mtx);
	has_left_fork = philo->left_fork->owner == philo->index;
	pthread_mutex_unlock(&philo->left_fork->fork_mtx);
	return (has_right_fork && has_left_fork);
}
