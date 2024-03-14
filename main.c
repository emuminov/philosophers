/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:14:04 by emuminov          #+#    #+#             */
/*   Updated: 2024/03/14 16:34:21 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define MAX_PHILO_NUMBER 200

enum					states
{
	THINKING,
	EATING,
	SLEEPING,
};

typedef pthread_mutex_t	t_mtx;
typedef struct s_params	t_params;

typedef struct s_philo
{
	pthread_t			th;
	unsigned int		index;
	unsigned int		meals_counter;
	unsigned long		last_eat_time;
	int					state;
	t_mtx				right_fork;
	t_mtx				*left_fork;
	t_params			*params;
}						t_philo;

typedef struct s_params
{
	unsigned int		philo_nbr;
	unsigned long		time_to_die;
	unsigned long		time_to_eat;
	unsigned long		time_to_sleep;
	unsigned int		max_nbr_of_meals;
	unsigned int		threads_ready;
	t_philo				philos[MAX_PHILO_NUMBER];
	t_mtx				mtx_sync;
	t_mtx				mtx_print;
}						t_params;

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

static int	ft_isspace(int c)
{
	return (c == '\t' || c == ' ' || c == '\r'
		|| c == '\v' || c == '\n' || c == '\f');
}

long	ft_atol(const char *str)
{
	long long	result;
	short		sign;
	size_t		i;

	result = 0;
	i = 0;
	while (ft_isspace(str[i]))
		i++;
	sign = ((str[i] == '-') * -1) + ((str[i] != '-') * 1);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && ft_isdigit(str[i]))
		result = (result * 10) + (str[i++] - '0');
	return (result * sign);
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
	int	i;

	i = 0;
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

	philo_nbr = ft_atol(argv[1]);
	time_to_die = ft_atol(argv[2]);
	time_to_eat = ft_atol(argv[3]);
	time_to_sleep = ft_atol(argv[4]);
	if (argv[5])
		max_nbr_of_meals = ft_atol(argv[5]);
	if (philo_nbr <= 0 || philo_nbr > MAX_PHILO_NUMBER)
		terminate("Wrong number of philosophers\n");
	else if (time_to_die <= 0 || time_to_die > INT_MAX)
		terminate("Bad time to die\n");
	else if (time_to_eat <= 0 || time_to_eat > INT_MAX)
		terminate("Bad time to eat\n");
	else if (time_to_sleep <= 0 || time_to_sleep > INT_MAX)
		terminate("Bad time to sleep\n");
	else if (argv[5] && (max_nbr_of_meals <= 0 || max_nbr_of_meals > INT_MAX))
		terminate("Bad time to sleep\n");
}

void	validate_input(char **argv)
{
	validate_input_being_numeric(argv);
	validate_input_bounds(argv);
}

void	params_init(int argc, char **argv, t_params *p)
{
	p->philo_nbr = ft_atol(argv[1]);
	p->time_to_die = ft_atol(argv[2]);
	p->time_to_eat = ft_atol(argv[2]);
	p->time_to_sleep = ft_atol(argv[3]);
	if (argc == 5)
		p->max_nbr_of_meals = -1;
	if (argc == 6)
		p->max_nbr_of_meals = ft_atol(argv[5]);
	pthread_mutex_init(&p->mtx_print, NULL);
	// TODO: protect mutex
	pthread_mutex_init(&p->mtx_sync, NULL);
	// TODO: protect mutex
}

void	parse_input(int argc, char **argv, t_params *p)
{
	validate_input(argv);
	params_init(argc, argv, p);
}

t_philo	init_philo(unsigned int index, t_params *p)
{
	t_philo	philo;
	t_mtx	right_fork;

	pthread_mutex_init(&right_fork, NULL);
	philo.index = index + 1;
	philo.last_eat_time = 0;
	philo.meals_counter = 0;
	philo.state = THINKING;
	philo.right_fork = right_fork;
	philo.left_fork = NULL;
	philo.params = p;
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

void	arbiter(void)
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
	pthread_mutex_lock(&philo->params->mtx_print);
	printf("Philo id: %d\n", philo->index);
	pthread_mutex_unlock(&philo->params->mtx_print);
	// pthread_mutex_lock(&philo->params->mtx_sync);
	// philo->params->threads_ready++;
	// pthread_mutex_unlock(&philo->params->mtx_sync);
	// while (philo->params->threads_ready < philo->params->philo_nbr)
	// 	;
	// pthread_mutex_lock(&philo->params->mtx_print);
	// printf("Philo id: %d\tThreads ready: %d\n", philo->index,
	// 	philo->params->threads_ready);
	// pthread_mutex_unlock(&philo->params->mtx_print);
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
		p->philos[i] = init_philo(i, p);
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
	for (unsigned int i = 0; i < p.philo_nbr; i++)
	{
		pthread_join(p.philos[i].th, NULL);
	}
	return (EXIT_SUCCESS);
}
