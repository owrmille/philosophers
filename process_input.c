#include "philosophers.h"

void	save_input_data(t_input *data, int argc, char **argv)
{
	data->num_philos = ft_atoi(argv[1]);
	data->die_time = ft_atoi(argv[2]);
	data->eat_time = ft_atoi(argv[3]);
	data->sleep_time = ft_atoi(argv[4]);
	if (argc == 6)
		data->num_meals = ft_atoi(argv[5]);
	else
		data->num_meals = -1;
}

void	init_input_data(t_input *data)
{
	data->num_meals = 0;
	data->num_philos = 0;
	data->die_time = 0;
	data->eat_time = 0;
	data->sleep_time = 0;
}

int	process_input(t_input *data, int argc, char **argv)
{
	init_input_data(data);
	if (argc != 5 && argc != 6)
	{
		write(2, "Error: wrong number of arguments.\n", 35);
		return (1);
	}
	save_input_data(data, argc, argv);
	if (data->num_philos <= 0 || data->die_time <= 0
		|| data->eat_time <= 0 || data->sleep_time <= 0
		|| (data->num_meals < 0 && data->num_meals != -1))
	{
		write(2,
			"Error: data should include only numbers bigger than 0.\n", 56);
		return (1);
	}
	if (data->num_philos > 200)
	{
		write(2, "Error: too many philosophers! Be careful.\n", 43);
		return (1);
	}
	return (0);
}
