#include "philosophers.h"

void	*routine(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *)arg;

	if (philo->first_fork_idx == philo->second_fork_idx)
	{
		handle_one_fork(philo);
		return (NULL);
	}

	if (philo->id % 2 == 0)
		ft_usleep(philo->sim->input_data->eat_time / 2, philo);
	while (1)
	{
		go_eat(philo);
		if (is_dead(philo))
			break ;
		go_sleep(philo);
		if (is_dead(philo))
			break ;
		go_think(philo);
		if (is_dead(philo))
			break ;
	}
	return (NULL);
}

void	handle_one_fork(t_philo *philo)
{
	take_fork(philo, philo->first_fork_idx);
	ft_usleep(philo->sim->input_data->die_time, philo);
	return_fork(philo, philo->first_fork_idx);
}


void	go_sleep(t_philo *philo)
{
	print_message(philo, "is sleeping");
	ft_usleep(philo->sim->input_data->sleep_time, philo);
}

void	go_think(t_philo *philo)
{
	print_message(philo, "is thinking");
	if (philo->sim->input_data->num_philos % 2 != 0)
		ft_usleep(1, philo);
}
