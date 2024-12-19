#include "philosophers.h"

void	print_input_data(t_input data, int argc)
{
	printf("num philo: %d\n", data.num_philos);
	printf("die time: %d\n", data.die_time);
	printf("eat time: %d\n", data.eat_time);
	printf("sleep time: %d\n", data.sleep_time);
	if (argc == 6)
		printf("num meals: %d\n", data.num_meals);
}

int	main(int argc, char **argv)
{
	t_input			input_data;
	t_simulation	sim;

	if (process_input(&input_data, argc, argv) == 1)
		return (1);
	if (run_simulation(&sim, &input_data) == 1)
		return (1);
	return (0);
}
