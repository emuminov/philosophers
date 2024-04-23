/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:14:04 by emuminov          #+#    #+#             */
/*   Updated: 2024/04/23 17:48:33 by emuminov         ###   ########.fr       */
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
	pthread_t			monitor_th;
	unsigned int		philo_nbr;
	unsigned long		time_to_die;
	unsigned long		time_to_eat;
	unsigned long		time_to_sleep;
	unsigned long		start_time;
	unsigned int		max_nbr_of_meals;
	unsigned int		threads_ready;
	unsigned int		is_running;
	t_philo				philos[MAX_PHILO_NUMBER];
	t_mtx				sync_lock;
	t_mtx				write_lock;
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
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
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

void	init_params(int argc, char **argv, t_params *p)
{
	p->philo_nbr = ft_atol(argv[1]);
	p->time_to_die = ft_atol(argv[2]);
	p->time_to_eat = ft_atol(argv[3]);
	p->time_to_sleep = ft_atol(argv[4]);
	p->is_running = 1;
	if (argc == 5)
		p->max_nbr_of_meals = -1;
	if (argc == 6)
		p->max_nbr_of_meals = ft_atol(argv[5]);
	p->start_time = get_time();
	pthread_mutex_init(&p->write_lock, NULL);
	// TODO: protect mutex
	pthread_mutex_init(&p->sync_lock, NULL);
	// TODO: protect mutex
}

void	parse_input(int argc, char **argv, t_params *p)
{
	validate_input(argv);
	init_params(argc, argv, p);
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

void	think(t_philo *philo)
{
	pthread_mutex_lock(&philo->params->write_lock);
	printf("%lu %u is thinking\n", (get_time() - philo->params->start_time), philo->index);
	pthread_mutex_unlock(&philo->params->write_lock);
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->right_fork);
	pthread_mutex_lock(&philo->params->write_lock);
	printf("%lu %u has taken a fork\n", get_time() - philo->params->start_time, philo->index);
	pthread_mutex_unlock(&philo->params->write_lock);
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(&philo->params->write_lock);
	printf("%lu %u has taken a fork\n", get_time() - philo->params->start_time, philo->index);
	pthread_mutex_unlock(&philo->params->write_lock);
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->params->write_lock);
	printf("%lu %u is eating\n", get_time() - philo->params->start_time, philo->index);
	pthread_mutex_unlock(&philo->params->write_lock);
	ft_usleep(philo->params->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(&philo->right_fork);
}

void	philo_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->params->write_lock);
	printf("%lu %u is sleeping\n", get_time() - philo->params->start_time, philo->index);
	pthread_mutex_unlock(&philo->params->write_lock);
	ft_usleep(philo->params->time_to_sleep);
}

void	sync_philos(t_philo *philo)
{
	pthread_mutex_lock(&philo->params->sync_lock);
	philo->params->threads_ready += 1;
	pthread_mutex_unlock(&philo->params->sync_lock);
	while (philo->params->threads_ready < philo->params->philo_nbr)
		;
}

int	simulation_is_running(t_philo *philo)
{
	int	is_running;

	pthread_mutex_lock(&philo->params->sync_lock);
	is_running = philo->params->is_running;
	pthread_mutex_unlock(&philo->params->sync_lock);
	return (is_running);
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	think(philo);
	if (philo->index % 2 == 0)
		ft_usleep(10);
	while (simulation_is_running(philo))
	{
		take_forks(philo);
		eat(philo);
		philo_sleep(philo);
		think(philo);
	}
	return (NULL);
}

void	init_philos(t_params *p)
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

void	*monitor_routine(void *data)
{
	t_params		*p;
	unsigned int	i;

	p = (t_params *)data;
	while (1)
	{
		i = 0;
		while (i < p->philo_nbr)
		{
			i++;
		}
	}
	return (NULL);
}

void	init_monitor(t_params *p)
{
	pthread_create(&p->monitor_th, NULL, philo_routine, p);
	// TODO: protect thread creation
}

int	main(int argc, char **argv)
{
	static t_params	p;

	if (argc != 5 && argc != 6)
		exit(EXIT_FAILURE);
	parse_input(argc, argv, &p);
	init_philos(&p);
	for (unsigned int i = 0; i < p.philo_nbr; i++)
		pthread_join(p.philos[i].th, NULL);
	return (EXIT_SUCCESS);
}
