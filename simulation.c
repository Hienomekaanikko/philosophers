/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:56:33 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/16 16:20:05 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void eater(t_philosopher *philo)
{
	long long	start;
	long long	current_time;

	start = philo->data->starting_time;
	if (philo->id % 2 == 0)
		usleep (100);
	pthread_mutex_lock(&philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	current_time = timestamp(philo->data->starting_time);
	printf("%lld %d is eating\n", current_time, philo->id);
	usleep(philo->time_to_eat * 1000);
	philo->last_meal_time = current_time;
	pthread_mutex_unlock(&philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void sleeper(t_philosopher *philo)
{
	long long	current_time;

	current_time = timestamp(philo->data->starting_time);
	printf("%lld %d is sleeping\n", current_time, philo->id);
	usleep(philo->time_to_sleep * 1000);
}

static void thinker(t_philosopher *philo)
{
	long long	current_time;

	current_time = timestamp(philo->data->starting_time);
	printf("%lld %d is thinking\n", current_time, philo->id);
	usleep(100);
}

static void	*do_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (1)
	{
		if (philo->data->stop_simulation)
			break ;
		eater(philo);
		sleeper(philo);
		thinker(philo);
	}
	return (0);
}

static int	init_threads(t_data *data)
{
	int			i;

	i = 0;
	data->threads = malloc(data->nbr_of_philos * sizeof(pthread_t));
	if (!data->threads)
		return (0);
	data->starting_time = init_time();
	while (i < data->nbr_of_philos)
	{
		if (pthread_create(&data->threads[i], NULL, do_routine, &data->philo[i]) != 0)
			return (0);
	}
	return (1);
}

int	run_simulation(t_data *data)
{
	int	i;

	i = 0;
	if (!init_threads(data))
		return (0);

	while (i < data->nbr_of_philos)
	{
		if (pthread_join(data->threads[i], NULL) != 0)
			return (0);
		i++;
	}
	free(data->threads);
	return (1);
}
