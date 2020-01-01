#include<stdio.h>
#include<math.h>
#include<time.h>
#include<string.h>
#include<sys/time.h>
#include<omp.h>
#include<immintrin.h>
#define INT int
#define DOU double
#define AVX __m256d

void ReadFileConvertCsr(FILE * fp_mtx,FILE * fp_vec,INT *&row_ptr,INT *&col_idx,DOU *&mtx_val,DOU *&vec_val,INT *&par_set)
{
        //INT row_num,col_num,nzz_num;
        //fscanf(fp_mtx,"%d %d %d",&row_num,&col_num,&nzz_num);
        //-----------------------Read Vector-------------------------//
        INT vec_row;
        //vec_row = col_num;
        //srand(time(NULL));
        fscanf(fp_vec,"%d",&vec_row);
        vec_val = (DOU *)malloc(sizeof(DOU)*vec_row);
        memset(vec_val,0,sizeof(DOU)*vec_row);
        for(INT i=1;i<=vec_row;i++)
        {
                fscanf(fp_vec,"%lf",&vec_val[i]);
                //vec_val[i] = ((rand()%10)+1)*1.0;
        }
        //-----------------------Read Matrix-------------------------//
        par_set = (INT *)malloc(sizeof(INT)*10);
        INT row_num,col_num,nzz_num;
        fscanf(fp_mtx,"%d %d %d",&row_num,&col_num,&nzz_num);
        printf("(%d , %d) , nzz_num = %d\n",row_num,col_num,nzz_num);
        INT mtx_width;
        INT x,cnt_ite;
        x = INT(nzz_num*1.0/row_num+0.5);
        if(x<=16)
        {
                mtx_width = x;
        }
        else
        {
                if(x>=128)
                {
                        cnt_ite=0;
                        while(x>=128)
                        {
                                if(x&1)
                                {
                                        x++;
                                }
                                x/=2;
                                if(cnt_ite==4)
                                {
                                        break;
                                }
                                cnt_ite++;
                        }
                        mtx_width = x;
                }
                else
                {
                        if(x>=36)
                        {
                                cnt_ite=0;
                                while(x>=36)
                                {
                                        if(x&1)
                                        {
                                                x++;
                                        }
                                        x/=2;
                                        if(cnt_ite==2)
                                        {
                                                break;
                                        }
                                        cnt_ite++;
                                }
                                mtx_width = x;
                        }
                        else
                        {
                                //printf("%d\n",x);
                                if(x&1)
                                {
                                        if(x%10>=5)
                                                x=(x/10+1)*10;
                                        else
                                                x=x/10*10;
                                }
                                x/=2;
                                mtx_width = x;
                        }
                }
        }
        //mtx_width = 39;
        printf("mtx_width = %d \n",mtx_width);
        INT mtx_high=4;
        INT str_max;
        str_max = nzz_num + row_num * (mtx_width-1);
        row_ptr = (INT *)malloc(sizeof(INT)*(row_num+1));
        col_idx = (INT *)malloc(sizeof(INT)*str_max);
        mtx_val = (DOU *)malloc(sizeof(DOU)*str_max);
        memset(mtx_val,0,sizeof(DOU)*str_max);
        memset(col_idx,0,sizeof(INT)*str_max);
        INT v=-1,cnt=0,cnt1;
        INT row,col;
        DOU nzz;
        for(INT i=0;i<nzz_num;i++)
        {
                fscanf(fp_mtx,"%d %d %lf",&col,&row,&nzz);
                row--;
                while(v<row)
                {
                        v++;
                        if(cnt%mtx_width!=0)
                        {
                                cnt=(cnt+mtx_width-1)/mtx_width*mtx_width;
                        }
                        row_ptr[v] = cnt;
                }
                col_idx[cnt] = col;
                mtx_val[cnt] = nzz;
                cnt++;
        }
        while(v<row_num)
        {
                v++;
                if(cnt%mtx_width!=0)
                {
                        cnt=(cnt+mtx_width-1)/mtx_width*mtx_width;
                }
                row_ptr[v] = cnt;
        }
        if(cnt%mtx_width!=0)
        {
                cnt=(cnt+mtx_width-1)/mtx_width*mtx_width;
        }
        v++;
        row_ptr[v]=cnt;
        if(cnt%(mtx_width*mtx_high)!=0)
        {
                cnt=(cnt+mtx_width*mtx_high-1)/(mtx_width*mtx_high)*(mtx_width*mtx_high);
        }
        par_set[0] = cnt;
        par_set[1] = mtx_width;
        par_set[2] = mtx_high;
        par_set[3] = row_num+1;
        par_set[5] = par_set[0]/par_set[2];
        par_set[6] = par_set[5]/par_set[1];
        par_set[7] = nzz_num;
}

void Avx2_CsrConvertCsr2(INT *&row_ptr,DOU *&mtx_val,INT *&Csr2_row_ptr,AVX *&Csr2_mtx_val,INT * par_set)
{
        INT i_end,mtx_width,j_end;
        i_end = par_set[3];
        mtx_width = par_set[1];
        j_end = par_set[0]/par_set[2];
        Csr2_row_ptr = (INT *)malloc(sizeof(INT)*i_end);
        Csr2_mtx_val = (AVX *)_mm_malloc(sizeof(AVX)*j_end,sizeof(DOU)*8);
        #pragma omp parallel for schedule(static)
        for(INT i=0;i<i_end;i++)
        {
                Csr2_row_ptr[i]=row_ptr[i]/mtx_width;
        }
        i_end = par_set[0];
        INT tile_size = par_set[1]*par_set[2];
        #pragma omp parallel for schedule(static)
        for(INT i=0;i<i_end;i++)
        {
                INT x,y,z;
                z = i/tile_size;
                x = z*mtx_width+i%mtx_width;
                y = (i%tile_size)/mtx_width;
                Csr2_mtx_val[x][y] = mtx_val[i];
        }
}

void Avx2_Madd(AVX *&Csr2_mid_val,AVX *&Csr2_mtx_val,INT *col_idx,DOU *&vec_val,INT * par_set)
{
        INT i_end     = par_set[6];
        INT mtx_width = par_set[1];
        INT mtx_high  = par_set[2];
        #pragma omp parallel for schedule(static)
        for(INT i=0;i<i_end;i++)
        {
                Csr2_mid_val[i]=_mm256_setzero_pd();
                INT xx=i*mtx_width;
                INT yy=xx*mtx_high;
                for(INT j=0;j<mtx_width;j++)
                {
                        AVX Csr2_col_val;
                        INT JJ=yy+j;
                        Csr2_col_val    = _mm256_set_pd(vec_val[col_idx[JJ+3*mtx_width]],vec_val[col_idx[JJ+2*mtx_width]],vec_val[col_idx[JJ+mtx_width]],vec_val[col_idx[JJ]]);
                        Csr2_mid_val[i] = _mm256_fmadd_pd(Csr2_mtx_val[xx+j],Csr2_col_val,Csr2_mid_val[i]);
                }
        }
}

void Avx2_SegSum(INT *&Csr2_row_ptr,AVX *&Csr2_mid_val,DOU *&mtx_ans,INT *&par_set)
{
        #pragma omp parallel for schedule(static)
        for(INT i=0;i<par_set[3]-1;i++)
        {
                mtx_ans[i]=0;
                for(INT j=Csr2_row_ptr[i];j<Csr2_row_ptr[i+1];j++)
                {
                        mtx_ans[i]+=Csr2_mid_val[j>>2][j%4];
                }
        }
}

int main(int argc,char ** argv)
{
        FILE * fp_mtx;
        FILE * fp_vec;
        FILE * fp_ans;
        DOU  * vec_val;
        INT  * row_ptr;
        INT  * col_idx;
        DOU  * mtx_val;
        INT  * par_set;
        DOU  * mtx_ans;
        INT  * Csr2_row_ptr;
        AVX  * Csr2_mtx_val;
        AVX  * Csr2_mid_val;
        fp_ans = fopen("answer_CSR2.mtx","wb+");
        //---------------------------------------------------------------//
        char * Matrixname;
        char * Vectorname;
        char * Iterations;
        if(argc>3)
        {
                Matrixname = argv[1];
                Vectorname = argv[2];
                Iterations = argv[3];
        }
        printf("----------------%s----------------\n",Matrixname);
        fp_mtx = fopen(Matrixname,"rb+");                       // Read matrix file
        fp_vec = fopen(Vectorname,"rb+");                       // Read vector file
        //----------------------Counting iterations----------------------//
        INT ite=0;
        for(INT i=0;i<strlen(Iterations);i++)
        {
                ite = ite * 10 + Iterations[i] - '0';
        }
        printf("Number of iterations are %d times\n",ite);
        //---------------------------------------------------------------//
        ReadFileConvertCsr(fp_mtx,fp_vec,row_ptr,col_idx,mtx_val,vec_val,par_set);
        //------------------Avx2_CsrConvertCsr2 Warm Up------------------//
        for(INT i=0;i<5;i++)
        {
            Avx2_CsrConvertCsr2(row_ptr,mtx_val,Csr2_row_ptr,Csr2_mtx_val,par_set);
            free(Csr2_row_ptr);
            _mm_free(Csr2_mtx_val);
        }
        //---------------------------------------------------------------//
        struct timeval start,end;
        double timeuse;
        gettimeofday(&start,NULL);
        //---------------------------------------------------------------//
        Avx2_CsrConvertCsr2(row_ptr,mtx_val,Csr2_row_ptr,Csr2_mtx_val,par_set);
        Csr2_mid_val  = (AVX *)_mm_malloc(sizeof(AVX)*par_set[6],sizeof(DOU)*8);
        mtx_ans = (DOU *)malloc(sizeof(DOU)*(par_set[3]-1));
        //---------------------------------------------------------------//
        gettimeofday(&end,NULL);
        timeuse = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
        printf("CsrConvertCsr2 Time : %.6lf ms\n",timeuse*1000);
        free(row_ptr);
        free(mtx_val);
        //----------------------------Warm Up----------------------------//
        for(INT i=0;i<500;i++)
        {
        Avx2_Madd(Csr2_mid_val,Csr2_mtx_val,col_idx,vec_val,par_set);
        Avx2_SegSum(Csr2_row_ptr,Csr2_mid_val,mtx_ans,par_set);
        }
        //---------------------------CSR2 SpMV---------------------------//
        gettimeofday(&start,NULL);
        for(INT i=0;i<ite;i++)
        {
        Avx2_Madd(Csr2_mid_val,Csr2_mtx_val,col_idx,vec_val,par_set);
        Avx2_SegSum(Csr2_row_ptr,Csr2_mid_val,mtx_ans,par_set);
        }
        gettimeofday(&end,NULL);
        timeuse = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
        //------------------------Important Information Print--------------------------------------//
        printf("CSR2_Spmv Calculate Time : %.6lf ms\n",(timeuse/ite)*1000.0);
        printf("CSR2_Spmv Gflops : %.6lf Gflops \n",((2*par_set[7])*1.0)/1000000000/(timeuse*1.0/ite));
        double bandwidth;
        bandwidth = ((par_set[3]+par_set[7])*sizeof(int)+(2*par_set[7]+par_set[3]-1)*sizeof(double))/(timeuse*1.0/ite)/(1024*1024*1024);
        printf("bandwidth = %lf GB/s\n",bandwidth);
        printf("------------------------END----------------------\n");
        fprintf(fp_ans,"%d\n",par_set[3]-1);
        for(INT i=0;i<par_set[3]-1;i++)
        {
        fprintf(fp_ans,"%.6lf\n",mtx_ans[i]);
        }
        //--------------------------------------------------------------//
        free(vec_val);
        free(col_idx);
        free(par_set);
        free(mtx_ans);
        free(Csr2_row_ptr);
        _mm_free(Csr2_mtx_val);
        _mm_free(Csr2_mid_val);
        return 0;
}
