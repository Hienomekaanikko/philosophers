/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 09:47:03 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/28 14:52:12 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init_data(t_data *data)
{
	data->error = 0;
	data->status_msg = malloc(sizeof(pthread_mutex_t));
	if (!data->status_msg)
	{
		error_message("Memory allocation error", NULL);
		return ;
	}
	if (pthread_mutex_init(data->status_msg, NULL) != 0)
		error_message("Failure", "Mutex");
	printf("exiting init data");
}

int init_philo_data(t_data *data, int argc, char **argv)
{
	int		i;

	i = 0;
	data->nbr_of_philosophers = ft_atoi(argv[1]);
	data->philosophers = malloc(data->nbr_of_philosophers * sizeof(t_philosopher));
	if (!data->philosophers)
	{
		error_message("ERROR: Malloc fail!", NULL);
		return (1);
	}
	while (i < data->nbr_of_philosophers)
	{
		data->philosophers[i].max_meals = 0;
		if (argc == 6)
			data->philosophers[i].max_meals += ft_atoi(argv[5]);
		data->philosophers[i].last_meal = 0;
		data->philosophers[i].dead = 0;
		data->philosophers[i].time_to_die = ft_atoi(argv[2]);
		data->philosophers[i].time_to_eat = ft_atoi(argv[3]);
		data->philosophers[i].time_to_sleep = ft_atoi(argv[4]);
		i++;
	}
	return (0);
}
