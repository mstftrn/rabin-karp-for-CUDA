#include <stdio.h>
#include <omp.h>
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
	p0 = (d * p0 + (str[i] - '0')) % q;
    }

    return p0;

}

int rk_matcher(char *str, char *pattern, int d, int q)
{
    int i = 0;
    int str_length = strlen(str);
    int pattern_length = strlen(pattern);
    int p0 = 0;
    int ts[str_length];
    int num_cores=4;
    int chunk_len=(int)ceil((float)str_length/num_cores);
    int padding_len=chunk_len*num_cores-str_length;
    int el_chunk_len=chunk_len+pattern_length-1;
    int tss[num_cores][el_chunk_len];
    for (int i=0; i<pattern_length-1; i++)
    	tss[0][i]=0;

    p0 = compute_value(pattern, pattern_length, d, q);
    
    ts[0] = compute_value(str, pattern_length, d, q);

    for (i = 1; i < str_length-pattern_length+1; ++i) {
	    ts[i] =
		(ts[i - 1] * d -
		 ((str[i - 1] - '0') * (int) pow(d,
						 pattern_length))) % q +
		(str[i + pattern_length - 1]
		 - '0') % q;
    }
    for (i=0;i<str_length;i++)
    {
    	printf("%d ", ts[i]);
    }

    int j = 0;
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
    int pos = rk_matcher("1234567", "78", 10, 500000);
    //printf("%d", pos);
    return 0;
}
