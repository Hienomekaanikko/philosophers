/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 10:20:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/28 14:42:04 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include "libft/libft.h"
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

# define ARG1	"number_of_philosophers, "
# define ARG2	"time_to_die, "
# define ARG3	"time_to_eat, "
# define ARG4	"time_to_sleep, "
# define ARG5	"optional: [number_of_times_each_philosopher_must_eat]"

typedef struct s_philosopher
{
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_t		thread;
	time_t			left_fork_pickup_time;
	time_t			right_fork_pickup_time;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	time_t			curr_time;
	time_t			starting_time;
	time_t			last_meal;
	int				max_meals;
	int				id;
	int				dead;
} t_philosopher;

typedef struct s_data
{
	int				nbr_of_philosophers;
	int				error;
	t_philosopher	*philosophers;
	pthread_mutex_t	*status_msg;
} t_data;

typedef struct s_thread_args
{
	t_philosopher	*philo;
	pthread_mutex_t	*mutex_msg;
} t_thread_args;


//closing program / memory handling

void		free_all(t_data *data);
int			validate_input(t_data *data, char **argv);
void		error_message(char *msg, char *arg);
void		init_data(t_data *data);
int			init_philo_data(t_data *data, int argc, char **argv);
void		destroy_left_forks(t_data *data);
void		print_status(pthread_mutex_t *msg_mutex, char *msg, long long time, int id);
long long	timestamp(time_t starting_time);

#endif
