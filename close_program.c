/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_program.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:00:24 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/26 14:01:20 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//clearing memory and handling shut down

void	free_all(t_data *data)
{
	free(data->philosophers);
}
