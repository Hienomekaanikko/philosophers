/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:45:00 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/24 13:44:37 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	wait_for_room(t_philo *philo)
{
	if (philo->data->nbr_of_philos == 1)
		return ;
	while (1)
	{
		pthread_mutex_lock(&philo->data->limit_mutex);
		if (philo->data->limit > 0)
		{
			philo->data->limit--;
			pthread_mutex_unlock(&philo->data->limit_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->limit_mutex);
		usleep(100);
	}
}

int	someone_died(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->stop_mutex);
	if (philo->data->stop_simulation)
	{
		pthread_mutex_unlock(&philo->data->stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->stop_mutex);
	return (0);
}
