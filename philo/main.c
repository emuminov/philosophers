/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuminov <emuminov@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:14:04 by emuminov          #+#    #+#             */
/*   Updated: 2024/04/26 17:13:25 by emuminov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	static t_params	p;
	enum e_error	e;

	e = validate_input(argc, argv);
	if (e != NO_ERRORS)
	{
		write_error(e);
		return (EXIT_FAILURE);
	}
	init(argc, argv, &p);
	cleanup(&p);
	return (EXIT_SUCCESS);
}
