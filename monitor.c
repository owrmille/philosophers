#include "philosophers.h"

void	monitor_philos(t_simulation *sim)
{
	int	i;
	int	num_philos;
	t_philo	*philo;

	num_philos = sim->input_data->num_philos;
	while (1)
	{
		i = -1;
		while (++i < num_philos)
		{
			philo = &(sim->philos[i]);
			if (should_stop(sim, philo))
				return ;
		}
		usleep(5000);
	}
	return ;
}


bool	should_stop(t_simulation *sim, t_philo *philo)
{
	int num_philos;

	num_philos = sim->input_data->num_philos;
	if (has_died(philo))
	{
		set_dead(sim->philos, num_philos);
		return (true);
	}
	if (has_just_eaten(philo))
	{
		pthread_mutex_lock(&sim->lock);
		if (++sim->processed_philos == num_philos)
		{
			pthread_mutex_unlock(&sim->lock);
			set_dead(sim->philos, num_philos);
			return (true);
		}
		pthread_mutex_unlock(&sim->lock);
	}
	return (false);
}


bool	has_just_eaten(t_philo *philo)
{
	bool	has_eaten;

	if (philo->has_eaten || philo->sim->input_data->num_meals == -1)
		return (false);

	pthread_mutex_lock(&(philo->meal_lock));
	has_eaten = philo->num_finished_meals >= philo->sim->input_data->num_meals;
	pthread_mutex_unlock(&(philo->meal_lock));
	return (has_eaten);
}
