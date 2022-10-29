#include<stdio.h>
int main(){
    int p, N;
    printf(" give in input a positive integer value N:");
    scanf("%d",&N);
    p=1;
    while (p + 2 < N) p = p + 2;
	if(p + 2 >= N){
	printf("P=%d is the biggest odd less than N=%d\n ", p ,N);
	}
       return 0;
}