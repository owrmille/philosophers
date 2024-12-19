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
	pthread_mutex_lock(&(philo->lock));
	last_meal_time = philo->last_meal_time;
	die_time = philo->sim->input_data->die_time;
	pthread_mutex_unlock(&(philo->lock));
	if (last_meal_time < curtime && curtime - last_meal_time > die_time)
	{
		print_message(philo, "died", curtime);
		pthread_mutex_lock(&(philo->lock));
		philo->dead = true;
		pthread_mutex_unlock(&(philo->lock));
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
		pthread_mutex_lock(&philos[i].lock);
		philos[i].dead = true;
		pthread_mutex_unlock(&philos[i].lock);
	}
}

bool	is_dead(t_philo *philo)
{
	bool	dead;

	pthread_mutex_lock(&(philo->lock));
	dead = philo->dead;
	pthread_mutex_unlock(&(philo->lock));
	return (dead);
}
