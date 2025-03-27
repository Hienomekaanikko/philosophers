#include "philosophers.h"

void	error_message(char *msg, char *arg)
{
	ft_putstr_fd("ERROR: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(msg, 2);
}
