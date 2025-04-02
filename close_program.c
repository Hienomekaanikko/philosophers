/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_program.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:00:24 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/02 17:10:13 by msuokas          ###   ########.fr       */
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
		pthread_mutex_destroy(&data->philosophers[i].mutex);
		i++;
	}
}
