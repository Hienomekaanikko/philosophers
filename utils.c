/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:10:21 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/03 13:19:35 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_all(t_data *data)
{
	free(data->philosophers);
}

void destroy_left_forks(t_data *data)
{
	int i = 0;

	while (i < data->nbr_of_philosophers)
	{
		pthread_mutex_destroy(&data->philosophers[i].left_fork);
		i++;
	}
}

void	error_message(char *msg, char *arg)
{
	ft_putstr_fd("ERROR: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(msg, 2);
}

