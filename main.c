/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:14:04 by emuminov          #+#    #+#             */
/*   Updated: 2024/03/13 10:28:15 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

typedef	pthread_mutex_t t_fork;

typedef struct	s_philo
{
	unsigned int	index;
	unsigned int	last_eat_time;
	unsigned int	meals_counter;
	int				state;
	t_fork			left_fork;
	t_fork			*right_fork;
}				t_philo;

typedef struct	s_params
{
	unsigned int	philo_nbr;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	max_nbr_of_meals;
	t_philo			*philos;
}				t_params;

void	terminate(char *msg)
{
	if (msg)
		printf("%s\n", msg);
	exit(EXIT_FAILURE);
}

static inline bool	ft_isdigit(int d)
{
	return (d >= '0' && d <= '9');
}

bool	is_numeric(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
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

void	params_validate(char **argv, t_params *params)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (is_numeric(argv[i]) == false)
			terminate("Non-numeric input\n");
		i++;
	}
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
	params_validate(argv, params);
	
}

int	main(int argc, char **argv)
{
	t_params	params;

	if (argc < 4 || argc > 6)
		exit(EXIT_FAILURE);
	parse_input(argc, argv, &params);
	return (EXIT_SUCCESS);
}
