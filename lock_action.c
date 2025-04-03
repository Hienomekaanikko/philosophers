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

void	lock_forks(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->left_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_fork);
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
	pthread_mutex_unlock(&philo->data->action_lock);
	usleep(philo->time_to_eat * 1000); // Simulate eating
	philo->last_meal = timestamp(philo->starting_time); // Update last meal time
	unlock_forks(philo);
	philo->meals++;
}

void	single_eat_lock(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->left_fork);
	pthread_mutex_lock(&philo->data->action_lock);
	print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->data->action_lock);
	usleep(philo->time_to_die * 1000);
	pthread_mutex_unlock(&philo->left_fork);
}
