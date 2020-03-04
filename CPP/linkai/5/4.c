#include<stdio.h>
int main()
{
	float height;
	while(printf("Enter a height in centimeters:"),scanf("%f",&height)!=EOF)
	{
		if(height<=0) break;
		printf("%.1f cm = %d feet, %.1f inches\n", height,(int)(height/30.38),(height- 30.38*(int)(height/30.38))/2.54);
	}
	return 0;
}

