/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:14:04 by emuminov          #+#    #+#             */
/*   Updated: 2024/03/13 13:22:12 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX_PHILO_NUMBER 200

enum states
{
	THINKING,
	EATING,
	SLEEPING,
};

typedef	pthread_mutex_t t_fork;

typedef struct	s_philo
{
	pthread_t		th;
	unsigned int	index;
	unsigned int	meals_counter;
	unsigned long	last_eat_time;
	int				state;
	t_fork			right_fork;
	t_fork			*left_fork;
}				t_philo;

typedef struct	s_params
{
	unsigned int	philo_nbr;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	unsigned int	max_nbr_of_meals;
	t_philo			philos[MAX_PHILO_NUMBER];
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

unsigned long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	// TODO: protect error
	return (tv.tv_sec / 1000 + tv.tv_usec * 1000);
}

void	ft_usleep(unsigned long ms)
{
	unsigned long	start;
	unsigned long	curr;

	start = get_time();
	curr = get_time();
	while ((curr - start) > ms)
	{
		curr = get_time();
		usleep(50);
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
	if (philo_nbr <= 0 || philo_nbr > MAX_PHILO_NUMBER)
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

void	params_init(int argc, char **argv, t_params *p)
{
	p->philo_nbr = atoi(argv[1]);
	p->time_to_die = atoi(argv[2]);
	p->time_to_eat = atoi(argv[2]);
	p->time_to_sleep = atoi(argv[3]);
	if (argc == 5)
		p->max_nbr_of_meals = -1;
	if (argc == 6)
		p->max_nbr_of_meals = atoi(argv[5]);
}

void	parse_input(int argc, char **argv, t_params *p)
{
	validate_input(argv);
	params_init(argc, argv, p);
}

t_philo	init_philo(unsigned int index)
{
	t_philo	philo;
	t_fork	right_fork;

	pthread_mutex_init(&right_fork, NULL);
	philo.index = index + 1;
	philo.last_eat_time = 0;
	philo.meals_counter = 0;
	philo.state = THINKING;
	philo.right_fork = right_fork;
	philo.left_fork = NULL;
	return (philo);
}

void	connect_philos_forks(unsigned int philos_nbr, t_philo *philos)
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

void	arbiter()
{
	// while 1
	//   time = get_time()
	//   while philos
	//     if time - current_philo.last_meal <= 0
	//       end dinner
	//     
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	// wait for the creation of all threads
	// if game is not started
	//   philos starting from 0 start to eat
	//   the rest thinks
	// while game is not ended
	//   current philo thinks
	//   waits for the arbiter descision
	//   if (the current_philo == philo allowed to eat by the arbiter
	//      OR allowed philo is not adjacent to current philo
	//      AND current philo is odd if the allowed philo is odd
	//      OR current philo is even if the allowed philo is even)
	//      current_philo takes left fork
	//      current_philo takes right fork
	//      current_philo sleeps for time_to_eat time
	//      current_philo sleeps for time_to_sleep time
	return (NULL);
}

void	init_philos_arr(t_params *p)
{
	unsigned int	i;

	i = 0;
	while (i < p->philo_nbr)
	{
		p->philos[i] = init_philo(i);
		i++;
	}
	connect_philos_forks(p->philo_nbr, p->philos);
	i = 0;
	while (i < p->philo_nbr)
	{
		pthread_create(&p->philos[i].th, NULL, philo_routine, &p->philos[i]);
		// TODO: protect thread creation
		i++;
	}
}



int	main(int argc, char **argv)
{
	static t_params	p;

	if (argc != 5 && argc != 6)
		exit(EXIT_FAILURE);
	parse_input(argc, argv, &p);
	init_philos_arr(&p);
	return (EXIT_SUCCESS);
}
