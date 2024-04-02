#include <stdio.h>
int max_from_list(int l[], int size);
int modulo2(int);
int main()
{
    int size_list;
    int list[3] = {1,2,3};
    size_list=sizeof(list)/sizeof(list[0]);
    int number;
    printf("Hello World! ");
    scanf("%d",&number);
    printf("%d\n",~(number-1));
    modulo2(number);
    printf("Numarul maxim din lista este: %d",max_from_list(list,size_list));
    return 0;
};

int modulo2(int number)
{
  switch(number%2)
  {
    case 0:
        printf("Par\n");
        break;
    case 1:
        printf("Impar\n");
        break;
    default:
        printf("Error\n");
        break;
  };
  return 0;
};

int max_from_list(int l[], int size)
{
    int max = l[0];
    for(int i=1; i<size; i++)
    {
        if(l[i]>max)
        {
            max=l[i];
        };
    };
    return max;
};
