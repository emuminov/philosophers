/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_program.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:12:42 by emuminov          #+#    #+#             */
/*   Updated: 2024/04/29 17:20:57 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	connect_philos_forks(unsigned int philos_nbr, t_philo *philos)
{
	unsigned int	i;

	i = 0;
	while (i < philos_nbr)
	{
		if (i == 0)
			philos[i].left_fork = &philos[philos_nbr - 1].right_fork;
		else
			philos[i].left_fork = &philos[i - 1].right_fork;
		i++;
	}
}

static t_philo	init_philo(unsigned int i, t_params *p)
{
	t_philo	philo;

	pthread_mutex_init(&philo.right_fork, NULL);
	pthread_mutex_init(&philo.meal_lock, NULL);
	philo.index = i + 1;
	if (philo.index == p->philo_nbr)
		philo.take_forks = take_forks_from_left;
	else
		philo.take_forks = take_forks_from_right;
	philo.last_meal_time = 0;
	philo.meals_counter = 0;
	philo.left_fork = NULL;
	philo.params = p;
	return (philo);
}

static void	init_philos(t_params *p)
{
	unsigned int	i;

	i = 0;
	while (i < p->philo_nbr)
	{
		p->philos[i] = init_philo(i, p);
		i++;
	}
	connect_philos_forks(p->philo_nbr, p->philos);
	if (p->philo_nbr == 1)
	{
		pthread_create(&p->philos[0].th, NULL, solo_routine, &p->philos[0]);
		return ;
	}
	i = 0;
	while (i < p->philo_nbr)
	{
		pthread_create(&p->philos[i].th, NULL, philo_routine, &p->philos[i]);
		i++;
	}
}

void	init(int argc, char **argv, t_params *p)
{
	p->philo_nbr = ft_atol(argv[1]);
	p->time_to_die = ft_atol(argv[2]);
	p->time_to_eat = ft_atol(argv[3]);
	p->time_to_sleep = ft_atol(argv[4]);
	p->is_running = 1;
	if (argc == 5)
		p->max_nbr_of_meals = UINT_MAX;
	if (argc == 6)
		p->max_nbr_of_meals = ft_atol(argv[5]);
	pthread_mutex_init(&p->write_lock, NULL);
	pthread_mutex_init(&p->sync_lock, NULL);
	init_philos(p);
	pthread_create(&p->monitor_th, NULL, monitor_routine, p);
}
