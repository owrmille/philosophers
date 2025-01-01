#include "philosophers.h"

void	go_eat(t_philo *philo)
{
	size_t	meal_time;

	if (take_forks(philo) == 1)
		return ;
	pthread_mutex_lock(&(philo->lock));
	if (!philo->dead)
	{
		meal_time = get_time();
		philo->last_meal_time = meal_time;
		pthread_mutex_unlock(&(philo->lock));
		print_message(philo, "is eating", meal_time);
		ft_usleep(philo->sim->input_data->eat_time, philo);
		pthread_mutex_lock(&(philo->lock));
		philo->num_finished_meals++;
		pthread_mutex_unlock(&(philo->lock));
	}
	else {
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
	if (is_dead(philo))
	{
		pthread_mutex_unlock(fork);
		return (false);
	}
	print_message(philo, "has taken a fork", get_time());
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
