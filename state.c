#include "philosophers.h"


bool	has_died(t_philo *philo)
{
	size_t	curtime;
	size_t	last_meal_time;
	size_t	die_time;

	if (is_dead(philo))
	{
		return (true);
	}

	curtime = get_time();
	pthread_mutex_lock(&(philo->meal_lock));
	last_meal_time = philo->last_meal_time;
	pthread_mutex_unlock(&(philo->meal_lock));

	die_time = philo->sim->input_data->die_time;

	if (curtime - last_meal_time > die_time)
	{
		print_message(philo, "died");
		set_dead(philo, 1);
		return (true);
	}
	return (false);
}

void	set_dead(t_philo *philos, int num_philos)
{
	int i;

	i = -1;
	while (++i < num_philos)
	{
		pthread_mutex_lock(&philos[i].dead_lock);
		philos[i].dead = true;
		pthread_mutex_unlock(&philos[i].dead_lock);
	}
}

bool	is_dead(t_philo *philo)
{
	bool	dead;

	pthread_mutex_lock(&(philo->dead_lock));
	dead = philo->dead;
	pthread_mutex_unlock(&(philo->dead_lock));
	return (dead);
}
