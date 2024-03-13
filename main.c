/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:14:04 by emuminov          #+#    #+#             */
/*   Updated: 2024/03/12 20:37:29 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>

typedef	pthread_mutex_t t_fork;

typedef struct	s_philo
{
	long	index;
	long	last_eat_time;
	long	meals_counter;
	int		state;
	t_fork	right_fork;
	t_fork	*left_fork;
}				t_philo;

typedef struct	s_params
{
	int		philo_nbr;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		max_nbr_of_meals;
	t_philo	*philos;
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

void	ft_usleep(unsigned int useconds)
{
	struct timeval start;
	struct timeval curr;

	gettimeofday(&start, NULL);
	gettimeofday(&curr, NULL);
	while ((((curr.tv_sec - start.tv_sec) * 1e6) +
			((curr.tv_usec - start.tv_usec))) > useconds)
	{
		gettimeofday(&curr, NULL);
		usleep(10);
	}
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

static inline void	validate_input_being_numeric(char **argv)
{
	int		i;
	i = -1;
	while (argv[++i])
		if (is_numeric(argv[i]) == false)
			terminate("Non-numeric input\n");
}

static inline void	validate_input_bounds(char **argv)
{
	long	philo_nbr;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	max_nbr_of_meals;

	philo_nbr = atol(argv[1]);
	time_to_die = atol(argv[2]);
	time_to_eat = atol(argv[3]);
	time_to_sleep = atol(argv[4]);
	max_nbr_of_meals = atol(argv[5]);
	if (philo_nbr <= 0 || philo_nbr > INT_MAX)
		terminate("Wrong number of philosophers\n");
	else if (time_to_die <= 0 || time_to_die > INT_MAX)
		terminate("Bad time to die\n");
	else if (time_to_eat <= 0 || time_to_eat > INT_MAX)
		terminate("Bad time to eat\n");
	else if (time_to_sleep <= 0 || time_to_sleep > INT_MAX)
		terminate("Bad time to sleep\n");
	else if (max_nbr_of_meals <= 0 || max_nbr_of_meals > INT_MAX)
		terminate("Bad time to sleep\n");
}

void	validate_input(char **argv)
{
	validate_input_being_numeric(argv);
	validate_input_bounds(argv);
}

void	params_init(int argc, char **argv, t_params *params)
{
	params->philo_nbr = atoi(argv[1]);
	params->time_to_die = atoi(argv[2]);
	params->time_to_eat = atoi(argv[2]);
	params->time_to_sleep = atoi(argv[3]);
	if (argc == 5)
		params->max_nbr_of_meals = -1;
	if (argc == 6)
		params->max_nbr_of_meals = atoi(argv[5]);
}

void	parse_input(int argc, char **argv, t_params *params)
{
	validate_input(argv);
	params_init(argc, argv, params);
}

int	main(int argc, char **argv)
{
	t_params	params;

	if (argc != 5 && argc != 6)
		exit(EXIT_FAILURE);
	parse_input(argc, argv, &params);
	return (EXIT_SUCCESS);
}
