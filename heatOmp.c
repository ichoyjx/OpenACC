#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifdef _OPENMP
#include <omp.h>
#endif

// I2D to index into a linear memory space from a 2D array index pair
#define I2D(ni, i, j) ((i) + (ni)*(j))


// kernel to update temperatures - CPU version
void step_kernel_cpu(int ni,
                     int nj,
                     float tfac,
                     float *temp_in,
                     float *temp_out) {
    int i, j, i00, im10, ip10, i0m1, i0p1;
    float d2tdx2, d2tdy2;

#pragma omp parallel for shared(tfac, temp_in, temp_out) private(i, j, i00, im10, ip10, i0m1, i0p1, d2tdx2, d2tdy2)
    // loop over all points in domain (not boundary points)
    for (j=1; j < nj-1; j++) {
        for (i=1; i < ni-1; i++) {
	    // find indices into linear memory for central point and neighbours
            i00 = I2D(ni, i, j);
            im10 = I2D(ni, i-1, j);
            ip10 = I2D(ni, i+1, j);
            i0m1 = I2D(ni, i, j-1);
            i0p1 = I2D(ni, i, j+1);

	    // evaluate derivatives
            d2tdx2 = temp_in[im10] - 2*temp_in[i00] + temp_in[ip10];
            d2tdy2 = temp_in[i0m1] - 2*temp_in[i00] + temp_in[i0p1];

	    // update temperatures
            temp_out[i00] = temp_in[i00] + tfac*(d2tdx2 + d2tdy2);
        }
    }
}


int main(int argc, char *argv[])
{
    int ni, nj, nstep;
    float tfac, *temp1_h, *temp2_h, *temp_tmp;
    int i, j, i2d, istep;
    float temp_bl, temp_br, temp_tl, temp_tr;
    clock_t startclock, stopclock;
    double time;
    double start, end;
    FILE *fp;

    // domain size and number of timesteps (iterations)
    ni = 200;
    nj = 200;
    nstep = 10000;

    // allocate temperature array on host
    temp1_h = (float *)malloc(sizeof(float)*ni*nj);
    temp2_h = (float *)malloc(sizeof(float)*ni*nj);

    // initial temperature in interior
    for (j=1; j < nj-1; j++) {
        for (i=1; i < ni-1; i++) {
            i2d = i + ni*j;
            temp1_h[i2d] = 0.0f;
        }
    }

    // initial temperature on boundaries - set corners
    temp_bl = 200.0f;
    temp_br = 300.0f;
    temp_tl = 200.0f;
    temp_tr = 300.0f;

    // set edges by linear interpolation from corners
    for (i=0; i < ni; i++) {
        // bottom
        j = 0;
        i2d = i + ni*j;
        temp1_h[i2d] = temp_bl + (temp_br-temp_bl)*(float)i/(float)(ni-1);

        // top
        j = nj-1;
        i2d = i + ni*j;
        temp1_h[i2d] = temp_tl + (temp_tr-temp_tl)*(float)i/(float)(ni-1);
    }

    for (j=0; j < nj; j++) {
        // left
        i = 0;
        i2d = i + ni*j;
        temp1_h[i2d] = temp_bl + (temp_tl-temp_bl)*(float)j/(float)(nj-1);

        // right
        i = ni-1;
        i2d = i + ni*j;
        temp1_h[i2d] = temp_br + (temp_tr-temp_br)*(float)j/(float)(nj-1);
    }

    // duplicate temeperature array on host
    memcpy(temp2_h, temp1_h, sizeof(float)*ni*nj);

    tfac = 0.2f;

    #ifdef _OPENMP
        start = omp_get_wtime();
    #else
	startclock = clock();
    #endif

    // main iteration loop
    for (istep=0; istep < nstep; istep++) {
            // CPU kernel
            step_kernel_cpu(ni, nj, tfac, temp1_h, temp2_h);
	    // swap the temp pointers
            temp_tmp = temp1_h;
            temp1_h = temp2_h;
            temp2_h = temp_tmp;

    }

    #ifdef _OPENMP
        end = omp_get_wtime();
    	printf("Time for OpenMP version: %f s\n", end-start);
    #else
	stopclock = clock();
    	time =((double)(stopclock-startclock))/CLOCKS_PER_SEC;
    	printf("Time for baseline version: %f s\n",time);
    #endif


    // output temp1 to a file
    fp = fopen("out1.dat", "w");
    fprintf(fp, "%i %i\n", ni, nj);
    for (j=0; j < nj; j++) {
        for (i=0; i < ni; i++) {
            fprintf(fp, "%f\n", i, j, temp1_h[i + ni*j]);
        }
    }
    fclose(fp);
}
