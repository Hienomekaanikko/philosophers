/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:18:47 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/24 15:30:39 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	eat_status(t_data *data, int *all_ate, int *i)
{
	if (data->max_meals > 0 && data->nbr_of_philos > 1)
	{
		pthread_mutex_lock(&data->philo[*i].meals_eaten_mutex);
		if (data->philo[*i].meals_eaten == data->max_meals)
			*all_ate = 1;
		else
			*all_ate = 0;
		pthread_mutex_unlock(&data->philo[*i].meals_eaten_mutex);
	}
}

static int	check_philo_alive(t_data *data, int i)
{
	long long	current_time;

	pthread_mutex_lock(&data->philo[i].last_meal_mutex);
	current_time = timestamp(data->starting_time);
	if (current_time - data->philo[i].last_meal_time > data->time_to_die)
	{
		pthread_mutex_lock(&data->stop_mutex);
		data->stop_simulation = 1;
		printf("%lld %d died\n", current_time, data->philo[i].id);
		pthread_mutex_unlock(&data->stop_mutex);
		pthread_mutex_unlock(&data->philo[i].last_meal_mutex);
		return (0);
	}
	pthread_mutex_unlock(&data->philo[i].last_meal_mutex);
	return (1);
}

static int	sim_stop(t_data *data)
{
	pthread_mutex_lock(&data->stop_mutex);
	if (data->stop_simulation == 1)
	{
		pthread_mutex_unlock(&data->stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->stop_mutex);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	int		all_ate;

	data = (t_data *)arg;
	all_ate = 0;
	while (1 && !all_ate)
	{
		i = 0;
		if (sim_stop(data))
			break ;
		while (i < data->nbr_of_philos)
		{
			if (data->max_meals > 0 && data->nbr_of_philos > 1)
				eat_status(data, &all_ate, &i);
			if (!check_philo_alive(data, i))
				return (NULL);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
