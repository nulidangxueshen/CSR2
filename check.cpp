#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
int main(int argc , char ** argv)
{
        FILE * answer_serial;
        FILE * answer_CSR2;
        char * x;
        char * y;
        x = argv[1];
        y = argv[2];
        int num,num1,cnt=0;
        double CSR2,serial;
        answer_serial = fopen(x,"rb+");
        answer_CSR2   = fopen(y,"rb+");
        fscanf(answer_serial,"%d\n",&num);
        fscanf(answer_CSR2,"%d\n",&num1);
        for(int i=0;i<num1;i++)
        {
                fscanf(answer_serial,"%lf\n",&serial);
                fscanf(answer_CSR2,"%lf\n",&CSR2);
                double ans1,ans2;
                ans1 = serial-CSR2>0?serial-CSR2:CSR2-serial;
                ans2 = CSR2>0?CSR2:-CSR2;
                if(ans1>0.01*ans2)
                {
                        printf("serial:%lf CSR2:%lf\n",serial,CSR2);
                        cnt++;
                }
        }
        if(cnt==0)
        {
                printf("Answer is Right! PASS\n");
        }
        else
        {
                printf("%d\n",cnt);
                printf("Answer is Error! NO PASS\n");
        }
        return 0;
}

