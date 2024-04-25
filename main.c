/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:14:04 by emuminov          #+#    #+#             */
/*   Updated: 2024/04/25 20:46:54 by emuminov         ###   ########.fr       */
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
	unsigned long		last_meal_time;
	int					state;
	t_mtx				meal_lock;
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

	pthread_mutex_init(&philo.right_fork, NULL);
	pthread_mutex_init(&philo.meal_lock, NULL);
	philo.index = index + 1;
	philo.last_meal_time = p->start_time;
	philo.meals_counter = 0;
	philo.state = THINKING;
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

enum e_flag
{
	GET,
	SET,
};

int	get_or_set_is_running(t_params *p, int val, enum e_flag f)
{
	int	is_running;

	pthread_mutex_lock(&p->sync_lock);
	if (f == SET)
		p->is_running = val;
	is_running = p->is_running;
	pthread_mutex_unlock(&p->sync_lock);
	// pthread_mutex_lock(&p->write_lock);
	// printf("Program is running %d\n", is_running);
	// pthread_mutex_unlock(&p->write_lock);
	return (is_running);
}

enum e_status
{
	THONK,
	FORK,
	EAT,
	SLEEP,
	DIE,
};

void	write_status(t_philo *philo, enum e_status s, int is_allowed)
{
	pthread_mutex_lock(&philo->params->write_lock);
	if ((!get_or_set_is_running(philo->params, -1, GET)) && !is_allowed)
	{
		pthread_mutex_unlock(&philo->params->write_lock);
		return ;
	}
	if (s == THONK)
		printf("%lu %u is thinking\n", (get_time() - philo->params->start_time),
				philo->index);
	else if (s == FORK)
		printf("%lu %u has taken a fork\n", (get_time() - philo->params->start_time),
				philo->index);
	else if (s == EAT)
		printf("%lu %u is eating\n", (get_time() - philo->params->start_time),
				philo->index);
	else if (s == SLEEP)
		printf("%lu %u is sleeping\n", (get_time() - philo->params->start_time),
				philo->index);
	else if (s == DIE)
		printf("%lu %u died\n", (get_time() - philo->params->start_time),
				philo->index);
	pthread_mutex_unlock(&philo->params->write_lock);
}

void	think(t_philo *philo)
{
	if (get_or_set_is_running(philo->params, -1, GET))
		write_status(philo, THONK, 0);
}

void	take_forks(t_philo *philo)
{
	if (philo->index == philo->params->philo_nbr)
	{
		pthread_mutex_lock(philo->left_fork);
		write_status(philo, FORK, 0);
		pthread_mutex_lock(&philo->right_fork);
		write_status(philo, FORK, 0);
	}
	else
	{
		pthread_mutex_lock(&philo->right_fork);
		write_status(philo, FORK, 0);
		pthread_mutex_lock(philo->left_fork);
		write_status(philo, FORK, 0);
	}
}

void	eat(t_philo *philo)
{
	if (get_or_set_is_running(philo->params, -1, GET))
	{
		write_status(philo, EAT, 0);
		pthread_mutex_lock(&philo->meal_lock);
		philo->last_meal_time = get_time();
		philo->meals_counter++;
		pthread_mutex_unlock(&philo->meal_lock);
		ft_usleep(philo->params->time_to_eat);
	}
	pthread_mutex_unlock(&philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	die(t_philo *philo)
{
	write_status(philo, DIE, 1);
}

void	philo_sleep(t_philo *philo)
{
	if (get_or_set_is_running(philo->params, -1, GET))
	{
		write_status(philo, SLEEP, 0);
		ft_usleep(philo->params->time_to_sleep);
	}
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	think(philo);
	if (philo->index % 2 == 0)
		ft_usleep(10);
	while (get_or_set_is_running(philo->params, -1, GET) == 1)
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
		i++;
	}
}

int	check_if_philo_should_be_dead(t_philo *philo, t_params *p)
{
	unsigned long	last_meal_time_diff;

	pthread_mutex_lock(&philo->meal_lock);
	last_meal_time_diff = get_time() - philo->last_meal_time;
	pthread_mutex_unlock(&philo->meal_lock);
	if (last_meal_time_diff >= p->time_to_die)
		return (1);
	return (0);
}

void	stop_program(t_params *p)
{
	unsigned int	i;

	pthread_mutex_destroy(&p->sync_lock);
	pthread_mutex_destroy(&p->write_lock);
	i = 0;
	while (i < p->philo_nbr)
	{
		pthread_mutex_destroy(&p->philos[i].meal_lock);
		pthread_mutex_destroy(p->philos[i].left_fork);
		i++;
	}
	exit(EXIT_SUCCESS);
}

unsigned int	get_or_set_meals_counter(t_philo *philo, int val, enum e_flag f)
{
	unsigned int	meals_counter;

	pthread_mutex_lock(&philo->meal_lock);
	if (f == SET)
		philo->meals_counter = val;
	meals_counter = philo->meals_counter;
	pthread_mutex_unlock(&philo->meal_lock);
	return (meals_counter);
}

int	check_if_philos_ate_all_their_meals(t_params *p)
{
	unsigned int	i;

	i = 0;
	while (i < p->philo_nbr)
	{
		if (get_or_set_meals_counter(&p->philos[i], -1, GET) < p->max_nbr_of_meals)
			return (0);
		i++;
	}
	return (1);
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
			if (check_if_philo_should_be_dead(&p->philos[i], p))
			{
				pthread_mutex_lock(&p->sync_lock);
				p->is_running = 0;
				pthread_mutex_unlock(&p->sync_lock);
				die(&p->philos[i]);
				return (NULL);
			}
			i++;
			ft_usleep(1);
		}
		if (check_if_philos_ate_all_their_meals(p))
		{
			pthread_mutex_lock(&p->sync_lock);
			p->is_running = 0;
			pthread_mutex_unlock(&p->sync_lock);
			return (NULL);
		}
	}
	return (NULL);
}

void	init_monitor(t_params *p)
{
	pthread_create(&p->monitor_th, NULL, monitor_routine, p);
	// TODO: protect thread creation
}

int	main(int argc, char **argv)
{
	static t_params	p;
	unsigned int	i;

	if (argc != 5 && argc != 6)
		exit(EXIT_FAILURE);
	parse_input(argc, argv, &p);
	init_philos(&p);
	init_monitor(&p);
	for (unsigned int i = 0; i < p.philo_nbr; i++)
		pthread_join(p.philos[i].th, NULL);
	pthread_join(p.monitor_th, NULL);
	pthread_mutex_destroy(&p.sync_lock);
	pthread_mutex_destroy(&p.write_lock);
	i = 0;
	while (i < p.philo_nbr)
	{
		pthread_mutex_destroy(&p.philos[i].meal_lock);
		pthread_mutex_destroy(p.philos[i].left_fork);
		i++;
	}
	return (EXIT_SUCCESS);
}
