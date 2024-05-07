/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:28:53 by emuminov          #+#    #+#             */
/*   Updated: 2024/05/06 13:56:55 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define MAX_PHILO_NUMBER 200
# ifdef COLORED
#  define RES "\x1B[0m"
#  define RED "\033[0;31m"
#  define GREEN "\033[0;32m"
#  define GRAY "\033[0;36m"
# else
#  define RES ""
#  define RED ""
#  define GREEN ""
#  define GRAY ""
# endif

enum					e_error
{
	NO_ERRORS,
	NON_NUMERIC,
	BAD_PHILO_NUM,
	BAD_DIE_TIME,
	BAD_EAT_TIME,
	BAD_SLEEP_TIME,
	BAD_MEALS_NUM,
	BAD_ARGS_NUMBER,
};

enum					e_status
{
	THINK,
	FORK,
	EAT,
	SLEEP,
	DIE,
};

enum					e_flag
{
	GET,
	SET,
};

typedef pthread_mutex_t	t_mtx;
typedef struct s_params	t_params;

typedef struct s_fork
{
	unsigned int		owner;
	unsigned int		index;
	t_mtx				fork_mtx;
}						t_fork;

typedef struct s_philo
{
	pthread_t			th;
	unsigned int		index;
	unsigned int		meals_counter;
	unsigned long		last_meal_time;
	t_mtx				meal_lock;
	t_fork				right_fork;
	t_fork				*left_fork;
	void				(*take_forks)(struct s_philo *philo);
	t_params			*params;
}						t_philo;

typedef struct s_params
{
	pthread_t			monitor_th;
	unsigned int		philo_nbr;
	unsigned int		threads_ready;
	unsigned long		time_to_die;
	unsigned long		time_to_eat;
	unsigned long		time_to_sleep;
	unsigned int		max_nbr_of_meals;
	unsigned long		start_time;
	bool				is_running;
	bool				time_is_synced;
	t_philo				philos[MAX_PHILO_NUMBER];
	t_mtx				sync_lock;
	t_mtx				write_lock;
}						t_params;

// ft_utils.c
bool					ft_isdigit(int d);
bool					ft_isspace(int c);
long					ft_atol(const char *str);
void					ft_putstr_err(char *msg);

// time_utils.c
unsigned long			get_time(void);
unsigned long			diff(unsigned long timestamp);
void					ft_usleep(unsigned long ms);
void					wait_for_all_threads(t_params *p);
void					wait_for_time_sync(t_params *p);

// validation.c
enum e_error			validate_input(int argc, char **argv);

// write_functions.c
void					write_status(t_philo *philo, enum e_status s,
							bool is_allowed);
void					write_error(enum e_error e);

// get_set.c
unsigned int			get_or_set_meals_counter(t_philo *philo, int val,
							enum e_flag f);
unsigned int			get_or_increment_threads_ready(t_params *p,
							enum e_flag f);
bool					get_or_set_is_running(t_params *p, int val,
							enum e_flag f);
bool					get_or_set_time_is_synced(t_params *p, int val,
							enum e_flag f);

// forks.c
void					take_forks_from_right(t_philo *philo);
void					take_forks_from_left(t_philo *philo);
bool					has_both_forks(t_philo *philo);
void					release_forks(t_philo *philo);

// philo_routine.c
void					*solo_routine(void *data);
void					*philo_routine(void *data);

// monitor_routine.c
void					*monitor_routine(void *data);

// init_program.c
void					init(int argc, char **argv, t_params *p);

// cleanup
void					cleanup(t_params *p);
#endif
