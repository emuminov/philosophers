/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:28:53 by emuminov          #+#    #+#             */
/*   Updated: 2024/04/26 14:30:06 by emuminov         ###   ########.fr       */
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

enum					states
{
	THINKING,
	EATING,
	SLEEPING,
};

enum					e_status
{
	THONK,
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
	void				(*take_forks)(struct s_philo *philo);
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
#endif
