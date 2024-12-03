#include "philosophers.h"

int	ft_atoi(const char	*str)
{
	int	sign_val;
	int	number;

	sign_val = 1;
	number = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
	{
		str++;
	}
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
		{
			sign_val = -1;
		}
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		number *= 10;
		number += *str - 48;
		str++;
	}
	return (sign_val * number);
}
