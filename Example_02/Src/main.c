#include <stdint.h>
#include <stdio.h>

int add(int a, int b);
int mul(int a, int b);

int add(int a, int b)
{
	int sum = a + b;
	return sum;
}

int mul(int a, int b)
{
	int product = a * b;
	return product;
}
int main(void)
{
	int a = 10, b = 20, sum, product;
	printf("Add two number \n");
	sum = add(a, b);
	printf("Sum of two numbers %d \n", sum);

	printf("Multiply two number \n");
	product = mul(a, b);
	printf("Product of two numbers %d \n", product);

	fflush(stdout);
	return 0;
}
