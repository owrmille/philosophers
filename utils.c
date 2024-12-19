#include "philosophers.h"

void	print_message(t_philo *philo, char *msg)
{
	size_t	current_time;

	if (!is_dead(philo))
	{
		pthread_mutex_lock(&(philo->sim->write_lock));
		current_time = get_time() - philo->sim->start_time;
		printf("%ld %d %s\n", current_time, philo->id, msg);
		pthread_mutex_unlock(&(philo->sim->write_lock));
	}
}
