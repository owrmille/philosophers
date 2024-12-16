#include "philosophers.h"

size_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
	{
		write(2, "Error: gettimeofday()\n", 23);
		return (0);
	}
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_usleep(size_t ms, t_philo *philo)
{
	size_t	start;

	start = get_time();
	while ((get_time() - start) < ms)
	{
		usleep(500);
		if (has_died(philo) || philo->sim->is_someone_dead)
			return (1);
	}
	return (0);
}
