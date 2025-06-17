/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:17:01 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/17 16:09:54 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	someone_died(t_philo *philo)
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

static void	eater(t_philo *philo)
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

static void	sleeper(t_philo *philo)
{
	print_action(philo, "is sleeping");
	usleep(philo->time_to_sleep * 1000);
}

static void	thinker(t_philo *philo)
{
	print_action(philo, "is thinking");
	usleep(100);
}

void	*run(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->nbr_of_philos == 1)
	{
		one_philo_case(philo);
		return (NULL);
	}
	else
	{
		while (1)
		{
			if (!increment_meals(philo))
				break ;
			if (someone_died(philo))
				break ;
			eater(philo);
			sleeper(philo);
			thinker(philo);
		}
	}
	return (NULL);
}
