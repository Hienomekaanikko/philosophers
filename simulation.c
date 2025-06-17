/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:56:33 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/17 14:00:22 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	eat_status(t_data *data, int *all_ate, int *i)
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

void	print_action(t_philosopher *philo, const char *msg)
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

void	*monitor_routine(void *arg)
{
	t_data		*data = (t_data *)arg;
	int			i;
	int			all_ate;
	long long	current_time;

	all_ate = 0;
	while (1)
	{
		i = 0;
		if (all_ate == 1)
			break ;
		pthread_mutex_lock(&data->stop_mutex);
		if (data->stop_simulation == 1)
			break ;
		pthread_mutex_unlock(&data->stop_mutex);
		while (i < data->nbr_of_philos)
		{
			if (data->max_meals > 0 && data->nbr_of_philos > 1)
				eat_status(data, &all_ate, &i);
			pthread_mutex_lock(&data->philo[i].last_meal_mutex);
			current_time = timestamp(data->starting_time);
			if (current_time - data->philo[i].last_meal_time > data->time_to_die)
			{
				pthread_mutex_lock(&data->stop_mutex);
				data->stop_simulation = 1;
				pthread_mutex_unlock(&data->stop_mutex);
				printf("%lld %d died\n", current_time, data->philo[i].id);
				pthread_mutex_unlock(&data->philo[i].last_meal_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&data->philo[i].last_meal_mutex);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}


static void eater(t_philosopher *philo)
{
	long long	current_time;

	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->left_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork);
		print_action(philo, "has taken a fork");
	}
	current_time = timestamp(philo->data->starting_time);
	print_action(philo, "is eating");
	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal_time = current_time;
	pthread_mutex_unlock(&philo->last_meal_mutex);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void sleeper(t_philosopher *philo)
{
	print_action(philo, "is sleeping");
	usleep(philo->time_to_sleep * 1000);
}

static void thinker(t_philosopher *philo)
{
	print_action(philo, "is thinking");
	usleep(100);
}

void	*do_routine(void *arg)
{
	t_philosopher	*philo;
	long long		current_time;

	philo = (t_philosopher *)arg;
	if (philo->data->nbr_of_philos == 1)
	{
		pthread_mutex_lock(&philo->left_fork);
		print_action(philo, "has taken a fork");
		usleep(philo->time_to_die * 1000);
		pthread_mutex_lock(&philo->data->stop_mutex);
		philo->data->stop_simulation = 1;
		current_time = timestamp(philo->data->starting_time);
		printf("%lld %d died\n", current_time, philo->id);
		pthread_mutex_unlock(&philo->data->stop_mutex);
		pthread_mutex_unlock(&philo->left_fork);
		return (NULL);
	}
	else
	{
		while (1)
		{
			if (philo->max_meals > 0)
			{
				if (philo->meals_eaten == philo->max_meals)
					break ;
				pthread_mutex_lock(&philo->meals_eaten_mutex);
				philo->meals_eaten++;
				pthread_mutex_unlock(&philo->meals_eaten_mutex);
			}
			pthread_mutex_lock(&philo->data->stop_mutex);
			if (philo->data->stop_simulation)
			{
				pthread_mutex_unlock(&philo->data->stop_mutex);
				break;
			}
			pthread_mutex_unlock(&philo->data->stop_mutex);
			eater(philo);
			sleeper(philo);
			thinker(philo);
		}
	}
	return (NULL);
}

static int	init_threads(t_data *data)
{
	int	i;

	i = 0;
	data->threads = malloc(data->nbr_of_philos * sizeof(pthread_t));
	if (!data->threads)
		return (0);
	while (i < data->nbr_of_philos)
	{
		if (pthread_create(&data->threads[i], NULL, do_routine, &data->philo[i]) != 0)
			return (0);
		i++;
	}
	return (1);
}

void	run_simulation(t_data *data)
{
	pthread_t	monitor;
	int			i;

	i = 0;
	if (!init_threads(data))
		return ;
	if (pthread_create(&monitor, NULL, monitor_routine, data) != 0)
		return ;
	while (i < data->nbr_of_philos)
	{
		if (pthread_join(data->threads[i], NULL) != 0)
			return ;
		i++;
	}
	pthread_join(monitor, NULL);
	return ;
}
