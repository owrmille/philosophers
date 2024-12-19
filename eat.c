#include "philosophers.h"

void	go_eat(t_philo *philo)
{
	if (take_forks(philo) == 1)
		return ;
	if (!is_dead(philo))
	{
		pthread_mutex_lock(&(philo->lock));
		philo->last_meal_time = get_time();
		pthread_mutex_unlock(&(philo->lock));
		print_message(philo, "is eating");
		ft_usleep(philo->sim->input_data->eat_time, philo);
		pthread_mutex_lock(&(philo->lock));
		philo->num_finished_meals++;
		pthread_mutex_unlock(&(philo->lock));
	}
	return_forks(philo);
}

int	take_forks(t_philo *philo)
{
	if (!take_fork(philo, philo->first_fork_idx))
	{
		return (1);
	}
	if (!take_fork(philo, philo->second_fork_idx))
	{
		return_fork(philo, philo->first_fork_idx);
		return (1);
	}
	return (0);
}

bool	take_fork(t_philo *philo, int fork_idx)
{
	pthread_mutex_t	*fork;
	t_simulation	*sim;

	sim = philo->sim;
	fork = &(sim->forks[fork_idx]);
	if (is_dead(philo))
		return (false);
	pthread_mutex_lock(fork);
	print_message(philo, "has taken a fork");
	return (true);
}


void	return_forks(t_philo *philo)
{
	return_fork(philo, philo->second_fork_idx);
	return_fork(philo, philo->first_fork_idx);
}


void	return_fork(t_philo *philo, int fork_idx)
{
	t_simulation	*sim;

	sim = philo->sim;
	pthread_mutex_unlock(&(sim->forks[fork_idx]));
}
