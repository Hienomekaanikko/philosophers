/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:56:33 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/23 13:38:32 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_action(t_philo *philo, const char *msg)
{
	long long	time;

	pthread_mutex_lock(&philo->data->stop_mutex);
	if (!philo->data->stop_simulation)
	{
		time = timestamp(philo->data->starting_time);
		printf("%lld %d %s\n", time, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->data->stop_mutex);
}

void	one_philo_case(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork);
	print_action(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->left_fork);
}

int	increment_meals(t_philo *philo)
{
	if (philo->max_meals > 0)
	{
		if (philo->meals_eaten == philo->max_meals)
			return (0);
		pthread_mutex_lock(&philo->meals_eaten_mutex);
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->meals_eaten_mutex);
	}
	return (1);
}

static int	init_threads(t_data *data)
{
	int	i;

	i = 0;
	data->threads = malloc(data->nbr_of_philos * sizeof(pthread_t));
	if (!data->threads)
	{
		data->msg = "Malloc failed";
		return (0);
	}
	while (i < data->nbr_of_philos)
	{
		if (pthread_create(&data->threads[i], NULL, run, &data->philo[i]) != 0)
		{
			pthread_mutex_lock(&data->stop_mutex);
			data->stop_simulation = 1;
			pthread_mutex_unlock(&data->stop_mutex);
			thread_failure_join(data, i);
			data->msg = "Thread creation failed";
			return (0);
		}
		i++;
	}
	return (1);
}

void	run_simulation(t_data *data)
{
	pthread_t	monitor;
	int			i;

	i = 0;
	data->starting_time = init_time();
	if (!init_threads(data))
		return ;
	if (pthread_create(&monitor, NULL, monitor_routine, data) != 0)
	{
		data->msg = "Thread creation failed";
		return ;
	}
	while (i < data->nbr_of_philos)
	{
		if (pthread_join(data->threads[i], NULL) != 0)
		{
			data->msg = "Thread joining failed";
			return ;
		}
		i++;
	}
	if (pthread_join(monitor, NULL) != 0)
		data->msg = "Thread joining failed";
	return ;
}
