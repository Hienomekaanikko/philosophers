#include "philosophers.h"

int	check_limits(t_data *data, char *arg)
{
	long long	nbr;

	nbr = ft_atol(arg);
	if (nbr > INT_MAX)
	{
		error_message(" <- No larger values than INT MAX are allowed!", arg);
		data->error = 1;
	}
	else if (nbr < INT_MIN)
	{
		error_message(" <- No negative numbers allowed!", arg);
		data->error = 1;
	}
	return (1);
}

int	validate_input(t_data *data, char **argv)
{
	int			i;
	int			j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (is_space(&argv[i][j]))
			j++;
		if (argv[i][j] == '+')
			j++;
		else if (argv[i][j] == '-')
			return (0);
		while (argv[i][j])
		{
			if (!ft_isnum(&argv[i][j]))
			{
				error_message(" <- Only digits are allowed!", argv[i]);
				data->error = 1;
				break ;
			}
			j++;
		}
		if (!check_limits(data, argv[i]))
			data->error = 1;
		i++;
	}
	if (!data->error)
		return (1);
	else
		return (0);
}
