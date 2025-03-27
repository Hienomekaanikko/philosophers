/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_program.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:00:24 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/27 15:06:18 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//clearing memory and handling shut down

void	free_all(t_data *data)
{
	int	i;

	i = 0;
	if (data->philosophers)
	{
		while (i < data->nbr_of_philosophers)
		{
			free(data->philosophers[i].left_fork);
			i++;
		}
		free(data->philosophers);
	}
}
