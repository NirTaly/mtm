#include <stdio.h>
#include <stdlib.h>

#define ERROR		(0)
#define SUCCESS		(1)

/***************************** FUNCTIONS DECLARATION *************************/
int inputHandler(int *dest);
int powerOf2(int num);
unsigned int logOf2(unsigned int num);

int main()
{
	int size;
	char valid;

	printf("Enter size of input:\n");
	if (2 != scanf("%d%c", &size, &valid) || (valid != ' ' && valid != '\n') || size <= 0)
	{
		printf("Invalid size\n");
		return (0);
	}

	// if (ERROR == inputHandler(&size) || size <= 0)
	// {
	// 	printf("Invalid size\n");
	// 	return (0);		
	// }

	printf("Enter numbers:\n");

	int *nums = malloc(sizeof(int) * size);
	if (!nums)
	{
		fprintf(stderr, "Malloc Failed\n");
		return (0);
	}
	
	for (int i = 0; i < size; i++)
	{
		if (2 != scanf("%d%c", nums+i, &valid) || (valid != ' ' && valid != '\n' && i != size-1))
		{
			printf("Invalid number\n");
			
			free(nums);
			return (0);
		}
		// if (ERROR == inputHandler(nums + i))
		// {
		// 	printf("Invalid number\n");
			
		// 	free(nums);
		// 	return (0);
		// }
	}
	
	int pow = 0;
	unsigned int pow_sum = 0;
	for (int i = 0; i < size; i++)
	{
		pow = powerOf2(nums[i]);
		if (pow >= 0)
		{
			pow_sum += pow;

			printf("The number %d is a power of 2: %d = 2^%d\n", nums[i],nums[i], pow);
		}
	}
	
	printf("Total exponent sum is %u\n",pow_sum);
	
	free(nums);
	return (0);
}


int powerOf2(int num)
{
	if (num > 0)
	{
		unsigned int u_num = num;
		// is power of 2
		if ((u_num & (u_num - 1) ) == 0)
		{
			return logOf2(u_num);
		}
	}

	return (-1);
}

unsigned int logOf2(unsigned int num)
{
	unsigned int pow = 0;

	while (num >>=1) 
	{
		pow++;
	}

	return pow;
}

int inputHandler(int *dest)
{
	char valid;

	if (2 != scanf("%d%c", dest, &valid) || (valid != ' ' && valid != '\n'))
	{
		return (ERROR);		
	}
		
	return (SUCCESS);
}