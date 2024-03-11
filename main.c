/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:14:04 by emuminov          #+#    #+#             */
/*   Updated: 2024/03/11 09:55:12 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

typedef	pthread_mutex_t t_fork;

typedef struct	s_philo
{
	long	index;
	long	last_eat_time;
	long	meals_counter;
	int		state;
	t_fork	left_fork;
	t_fork	*right_fork;
}				t_philo;

typedef struct	s_params
{
	long	philo_nbr;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	max_nbr_of_meals;
	t_philo	*philos;
}				t_params;

void	terminate(char *msg)
{
	if (msg)
		printf("%s\n", msg);
	exit(EXIT_FAILURE);
}

void	params_init(int argc, char **argv, t_params *params)
{
	params->philo_nbr = atol(argv[1]);
	params->time_to_die = atol(argv[2]);
	params->time_to_eat = atol(argv[2]);
	params->time_to_sleep = atol(argv[3]);
	if (argc == 5)
		params->max_nbr_of_meals = -1;
	if (argc == 6)
		params->max_nbr_of_meals = atol(argv[5]);
}

void	params_validate(t_params *params)
{
	if (params->philo_nbr <= 0 || params->philo_nbr > INT_MAX)
		terminate("Wrong number of philosophers\n");
	else if (params->time_to_die <= 0 || params->time_to_die > INT_MAX)
		terminate("Bad time to die\n");
	else if (params->time_to_eat <= 0 || params->time_to_eat > INT_MAX)
		terminate("Bad time to eat\n");
	else if (params->time_to_sleep <= 0 || params->time_to_sleep > INT_MAX)
		terminate("Bad time to sleep\n");
}

void	parse_input(int argc, char **argv, t_params *params)
{
	params_init(argc, argv, params);
	params_validate(params);
	
}

int	main(int argc, char **argv)
{
	t_params	params;

	if (argc < 4 || argc > 6)
		exit(EXIT_FAILURE);
	parse_input(argc, argv, &params);
	return (EXIT_SUCCESS);
}
