/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:10:21 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/12 11:24:54 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_all(t_data *data)
{
	free(data->philosophers);
}

void	destroy_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philosophers)
	{
		pthread_mutex_destroy(&data->philosophers[i].left_fork);
		i++;
	}
	pthread_mutex_destroy(&data->action_lock);
}

void	error_message(char *msg, char *arg)
{
	ft_putstr_fd("ERROR: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(msg, 2);
}

