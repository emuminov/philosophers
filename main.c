/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:14:04 by emuminov          #+#    #+#             */
/*   Updated: 2024/03/08 16:59:57 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t	mutex;
int mails = 0;

void	*roll_dice()
{
	for (int i = 0; i < 10000; i++)
	{
		pthread_mutex_lock(&mutex);
		mails++;
		pthread_mutex_unlock(&mutex);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	pthread_t		th[4];
	int				i;

	pthread_mutex_init(&mutex, NULL);
	for (i = 0; i < 4; i++)
	{
		if (pthread_create(&th[i], NULL, routine1, NULL) != 0)
			return (EXIT_FAILURE);
		printf("Thread %d has been started\n", i);
	}
	for (i = 0; i < 4; i++)
	{
		if (pthread_join(th[i], NULL))
			return (EXIT_FAILURE);
		printf("Thread %d has finished execution\n", i);
	}
	pthread_mutex_destroy(&mutex);
	printf("Number of mails: %d\n", mails);
	return (EXIT_SUCCESS);
}
