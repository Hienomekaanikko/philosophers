/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:08:44 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/17 16:10:44 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	timestamp(long long starting_time)
{
	struct timeval	curr_time;
	long long		curr_time_ms;
	long long		elapsed_ms;

	gettimeofday(&curr_time, NULL);
	curr_time_ms = (curr_time.tv_sec * 1000) + (curr_time.tv_usec / 1000);
	elapsed_ms = curr_time_ms - starting_time;
	return (elapsed_ms);
}

long long	init_time(void)
{
	struct timeval	start;

	gettimeofday(&start, NULL);
	return ((start.tv_sec * 1000) + (start.tv_usec / 1000));
}
