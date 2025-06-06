/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock_action.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 14:22:15 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/03 15:53:22 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_status(t_philosopher *philo, char *msg)
{
	if (philo->data->simulation_running)
		printf("%lld %d %s\n", timestamp(philo->starting_time), philo->id, msg);
}

void lock_forks(t_philosopher *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (&philo->left_fork < philo->right_fork)
	{
		first = &philo->left_fork;
		second = philo->right_fork;
	} else
	{
		first = philo->right_fork;
		second = &philo->left_fork;
	}
	pthread_mutex_lock(first);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(second);
	print_status(philo, "has taken a fork");
}


void	unlock_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(&philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	think_lock(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->action_lock);
	print_status(philo, "is thinking");
	pthread_mutex_unlock(&philo->data->action_lock);
}

void	sleep_lock(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->action_lock);
	print_status(philo, "is sleeping");
	pthread_mutex_unlock(&philo->data->action_lock);
	usleep(philo->time_to_sleep * 1000); // Simulate sleeping
}

void	eat_lock(t_philosopher *philo)
{
	lock_forks(philo);
	pthread_mutex_lock(&philo->data->action_lock);
	print_status(philo, "is eating");
	philo->last_meal = timestamp(philo->starting_time);
	pthread_mutex_unlock(&philo->data->action_lock);
	precise_usleep(philo->time_to_eat * 1000);
	unlock_forks(philo);
	philo->meals++;
	if (philo->data->meals_required > 0)
	{
		pthread_mutex_lock(&philo->data->meal_lock);
		philo->data->meals_completed++;
		if (philo->data->meals_completed >=
			philo->data->meals_required * philo->data->nbr_of_philosophers)
		{
			pthread_mutex_unlock(&philo->data->meal_lock);
			philo->data->simulation_running = 0;
		}
		else
			pthread_mutex_unlock(&philo->data->meal_lock);
	}
}


void	single_eat_lock(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->left_fork);
	pthread_mutex_lock(&philo->data->action_lock);
	print_status(philo, "has taken a fork");
	philo->last_meal = timestamp(philo->starting_time); // 🕒 Critical update
	pthread_mutex_unlock(&philo->data->action_lock);
	usleep(philo->time_to_die * 1000); // Wait for death time
	pthread_mutex_unlock(&philo->left_fork);
}

