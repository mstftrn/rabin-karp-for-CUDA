#include <stdio.h>
//#include <omp.h>
#include <string.h>
#include <math.h>
/*
 * compute string value, length should be small than strlen
 */
int compute_value(char *str, int length, int d, int q)
{
    int i = 0;
    int p0 = 0;

    for (i = 0; i < length; ++i) {
	p0 = (d * p0 + (str[i] /*- '0'*/)) % q;
    }

    return p0;

}

int rk_matcher(char *str, char *pattern, int d, int q)
{
    int i = 0,j=0;
    int str_length = strlen(str);
    int pattern_length = strlen(pattern);
    int p0 = 0;
    int ts[str_length];

    int num_cores=4;
    int chunk_len=(int)ceil((float)str_length/num_cores);
    int padding_len=chunk_len*num_cores-str_length;
    int el_chunk_len=chunk_len+pattern_length-1;

    //matrix which holds the characters, each row will go to a core
    int tss[num_cores][el_chunk_len];
    
    //initial zeroes
    for (i=0; i<pattern_length-1; i++)
    	tss[0][i]=0;

    //first n-1 cores' characters
    for (i=0; i<num_cores-1; i++)
        for (j=0;j<chunk_len;j++)
            tss[i][j+pattern_length-1]=str[i*chunk_len+j];
    
    //last core's characters
    for (i=(num_cores-1)*chunk_len, j=0; i<str_length;i++,j++)
        tss[num_cores-1][j+pattern_length-1]=str[i];
    
    //last n-1 cores' padding characters
    for (i=1;i<num_cores;i++)
        for (j=0;j<pattern_length-1;j++)
            tss[i][j]=tss[i-1][j+chunk_len];
    
    //last core's last paddings
    for (i=0; i<padding_len;i++)
        tss[num_cores-1][el_chunk_len-i-1]=0;

    /* This code block prints what is inside the matrix
    for (i=0;i<num_cores;i++)
    {
        for (j=0;j<el_chunk_len;j++)
            if (tss[i][j]==0)
                printf("%c", '0');
            else
                printf("%c", tss[i][j]);
        printf("\n");
    }
    */



    //hash value of the pattern
    p0 = compute_value(pattern, pattern_length, d, q);
    
    //hash value of the first char
    ts[0] = compute_value(str, pattern_length, d, q);

    //p does not change, calculate once
    int p=pow(d, pattern_length-1);
    for (i = 1; i < str_length-pattern_length+1; i++) 
    {
	ts[i] = ((str[i + pattern_length - 1])*p
                    +(ts[i-1]-(str[i-1]))/d)%q;
	/*	(ts[i - 1] * d -
		 ((str[i - 1] - '0') * (int) pow(d,
						 pattern_length))) % q +
		(str[i + pattern_length - 1]
		 - '0') % q;*/
    }

/*    for (i=0;i<str_length-pattern_length+1;i++)
    {
    	printf("%d ", ts[i]);
    }*/

    for (i = 0; i <= str_length - pattern_length+1; ++i) {
	if (ts[i] == p0) {
	    for (j = 0; j < pattern_length; ++j) {
		if (pattern[j] != str[i + j]) {
		    break;
		} else if (j == pattern_length - 1) {
		    printf("%d\n", i);
		}
	    }
	}
    }

    return 0;

}

int main(int argc, char *argv[])
{
    int pos = rk_matcher("bababanaparaver", "aba", 3, 50);
    //printf("%d", pos);
    return 0;
}
