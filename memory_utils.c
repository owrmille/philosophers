#include "philosophers.h"

void	free_philos(t_philo *arr_philos)
{
	if (arr_philos)
		free(arr_philos);
}
