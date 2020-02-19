/*************************************************************************
    > File Name: 8.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月19日 星期三 10时59分37秒
 ************************************************************************/

#include<stdio.h>
#include<ctype.h>
char get_first(void);
char get_choice(void);
float get_float(void);

int main()
{
		char choice;
		float num1, num2;

		while ((choice = get_choice()) != 'q')
		{
			puts("Enter first number:");
			num1 = get_float();
			puts("Enter second number:");
			num2 = get_float();
			switch (choice)
			{
				case 'a': printf("%f + %f = %f\n", num1, num2, num1+num2);
						  break;
				case 's': printf("%f - %f = %f\n", num1, num2, num1-num2);
						  break;
				case 'm': printf("%f * %f = %f\n", num1, num2, num1*num2);
						  break;
				case 'd': while (num2 == 0)
						  {
							  printf("The dividend can not be 0.\nEnter second number again:\n");
							  num2 = get_float();
						  }

						  printf("%f / %f = %f\n", num1, num2, num1/num2);
						  break;
			}
		}
		puts("Bye.");
		return 0;
}

float get_float(void)
{
	float input;
	char ch;

	while (scanf("%f", &input) != 1)
	{
		while ((ch =getchar()) != '\n')
			putchar(ch);
		printf(" is not an integer.\nPlease enter an integer value:\n");
	}

	return input;
}

char get_choice(void)
{
	char ch;

	printf("Enter the operation of your choice:\n");
    printf("%-20s%-20s\n%-20s%-20s\n%-20s\n", "a. add", "s. subtract", "m. multuply", "d. divide", "q. quit");
	ch =get_first();
	while(ch != 'a' && ch != 's' && ch != 'm' && ch != 'd' && ch != 'q')
	{
		puts("Please respond with a, s, m, d or q");
		ch =get_first();
	}

	return tolower(ch);
}

char get_first(void)
{
    char ch = 0;

    while (!isalpha(ch = getchar()))
        continue;
	while (getchar() != '\n')
		continue;

    return ch;
}
