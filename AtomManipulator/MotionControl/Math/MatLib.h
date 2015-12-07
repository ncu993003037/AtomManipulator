#pragma once

#ifndef _MATLIB_H_
#define _MATLIB_H_

// In this current version, this library should only be called by a single thread

#include <iostream> // should be placed before BLAS and LAPACK
#include <f2c.h>
extern "C"
{
#include <clapack.h> 
}

//#include "lapacke.h"
#define s_ele sizeof(double)
typedef long Mat_int;


using namespace std;

//==== Initialization ====//

void Mat_error(char * error);

#define Mat_zeros(A,l) (memset(A, 0.0, l*s_ele))

void Mat_vals (double *A, Mat_int l, double val);
void Mat_eye (double *A, Mat_int Arow, Mat_int Acol, double val = 1);

//==== Operation ====//

#define Mat_rep_col(A,Arow, Acol, k, x) (memcpy( A+Arow*k, x, Arow*s_ele))// replace the kth column with x
void Mat_rep_row(double *A, Mat_int Arow, Mat_int Acol, Mat_int k, double *x);
#define Mat_cpy(A_dst,B_src,l) (memcpy(A_dst, B_src, l *s_ele)) // copy l elements from B to A 
void Mat_trans(double* A, Mat_int Arow, Mat_int Acol, double *B); // B = A'


//==== Print ====//
void Mat_Show( const double *A, const Mat_int Arow, const Mat_int Acol, const  char* text);

//==== BLAS 1 ====//
void Mat_ax_plu (double *x, double  a, double *z, Mat_int l); // z = a + x
void Mat_ax_min (double *x, double  a, double *z, Mat_int l); // z = x - a
void Mat_ax_min (double  a, double *x, double *z, Mat_int l); // z = a - x
void Mat_ax_mul (double *x, double  a, double *z, Mat_int l); // z = a * x
void Mat_ax_div (double *x, double  a, double *z, Mat_int l); // z = x / a
void Mat_ax_div (double  a, double *x, double *z, Mat_int l); // z = a ./x


void Mat_xy_plu (double *x, double *y, double *z, Mat_int l); // z = x + y
void Mat_xy_min (double *x, double *y, double *z, Mat_int l); // z = x - y
void Mat_xy_mul (double *x, double *y, double *z, Mat_int l); // z = x .* y
void Mat_xy_div (double *x, double *y, double *z, Mat_int l); // z = x ./ y

double Mat_xy (double *x, double *y, Mat_int l);
double Mat_norm (double *x, Mat_int l);

//==== BLAS 2 ====//
void Mat_Ax_by_large    (double *A, Mat_int Acol, Mat_int Arow, double *x, double *y, double beta = 0.0, char TransA = 'N'); // y = A*x + beta*y (default beta = 0)

void Mat_Ax_by  (double *A, Mat_int Arow, Mat_int Acol, double *x, double *y, double beta = 0.0, char TransA = 'N');
void Mat_Ax_y   (double *A, Mat_int Arow, Mat_int Acol, double *x, double *y);  // y = A * x
void Mat_Atx_y  (double *A, Mat_int Arow, Mat_int Acol, double *x, double *y);  // y = At * x

void Mat_ADBx_by  (double *A, Mat_int Acol, Mat_int Arow, double *D, double *B, Mat_int Bcol, Mat_int Brow, 
				   double *x, double *y, double beta = 0, char TransA = 'N', char TransB = 'N'); // y = A*D*B*x + beta*y

//==== BLAS 3 ====//
void Mat_AB_bC_large (double *A, Mat_int Arow, Mat_int Acol, double *B, Mat_int Brow, Mat_int Bcol, double *C, 
				double beta = 0, char TransA = 'N', char TransB = 'N'); // C = A*B + beta*C
void Mat_AB_bC(double* A, Mat_int Arow, Mat_int Acol, double* B, Mat_int Brow, Mat_int Bcol, double *C, double beta = 0, char TransA = 'N');
void Mat_AB_C (double* A, Mat_int Arow, Mat_int Acol, double* B, Mat_int Brow, Mat_int Bcol, double *C);
void Mat_AtB_C( double* A, Mat_int Arow, Mat_int Acol, double* B, Mat_int Brow, Mat_int Bcol, double *C);

// Diagonal Matrix Multiplication
void Mat_ADB_C   (double *A, Mat_int Arow, Mat_int Acol, double *D, double *B, Mat_int Brow, Mat_int Bcol, double *C, char TransA = 'N', char TransB = 'N'); // C = A*D*B
void Mat_D1AD2_C (double *A, Mat_int Arow, Mat_int Acol, double *D1, double *D2, double *C, char TransA = 'N'); // C = D1*A*D2 
void Mat_DA_C    (double *A, Mat_int Arow, Mat_int Acol, double *D,  double *C, char TransA = 'N'); // C = D*A
void Mat_AD_C   (double *A, Mat_int Arow, Mat_int Acol, double *D,  double *C, char TransA = 'N'); // C = A*D

// Diagonal Matrix Summation
void Mat_A_D_plu   (double *A, Mat_int Arow, Mat_int Acol, double *D);
void Mat_A_D_min   (double *A, Mat_int Arow, Mat_int Acol, double *D);
void Mat_D_A_min   (double *A, Mat_int Arow, Mat_int Acol, double *D);
void Mat_I_A_plu  (double *A, Mat_int Arow, Mat_int Acol, double alpha = 1);
void Mat_A_I_min  (double *A, Mat_int Arow, Mat_int Acol, double alpha = 1);
void Mat_I_A_min  (double *A, Mat_int Arow, Mat_int Acol, double alpha = 1); // A = alpha * I - A;
 

//==== LAPACK ====//
void Mat_svd ( double* J, Mat_int Jrow, Mat_int Jcol,  double *U, double *S, double *Vt);
void Mat_inv22 (double *A, double *invA);



///////////////////////////////////////////////////////////////////////////////////////
//-------------------------inline  IMPLEMENTATION    --------------------------------//
///////////////////////////////////////////////////////////////////////////////////////


// Initialization
inline void Mat_vals (double *A, Mat_int l, double val)
{for (Mat_int i = 0; i<l; ++i) A[i] = val;}
inline void Mat_eye (double *A, Mat_int Arow, Mat_int Acol, double val )
{for (Mat_int i=0; i<Arow; ++i)		A[i+i*Arow] = val;}
inline void Mat_rep_row(double *A, Mat_int Arow, Mat_int Acol, Mat_int k, double *x) // replace the kth row with x
{for (Mat_int i = 0; i<Acol; ++i) A[k+i*Arow] = x[i];}
inline void Mat_trans(double* A, Mat_int Arow, Mat_int Acol, double *B) 
{// B = A'
	for (Mat_int j =0; j< Acol; ++j)
		for (Mat_int i = 0; i < Arow; ++i)		
			B[j + i*Acol] = A[i+j*Arow];
}

// BLAS 1
inline void Mat_ax_plu (double *x, double  a, double *z, Mat_int l) // z = a + x
{for (Mat_int i = 0; i<l; ++i) z[i] = x[i] + a;}
inline void Mat_ax_min (double *x, double  a, double *z, Mat_int l) // z = x - a
{for (Mat_int i = 0; i<l; ++i) z[i] = x[i] - a;}
inline void Mat_ax_min (double  a, double *x, double *z, Mat_int l) // z = a - x
{for (Mat_int i = 0; i<l; ++i) z[i] = a - x[i];}
inline void Mat_ax_mul (double *x, double  a, double *z, Mat_int l) // z = a * x
{for (Mat_int i = 0; i<l; ++i) z[i] = x[i] * a;}
inline void Mat_ax_div (double *x, double  a, double *z, Mat_int l) // z = x / a
{for (Mat_int i = 0; i<l; ++i) z[i] = x[i] / a;}
inline void Mat_ax_div (double  a, double *x, double *z, Mat_int l) // z = a ./x
{for (Mat_int i = 0; i<l; ++i) z[i] = a  / x[i] ;}

inline void Mat_xy_plu (double *x, double *y, double *z, Mat_int l)
{for (Mat_int i = 0; i<l; ++i) z[i] = x[i] + y[i];}
inline void Mat_xy_min (double *x, double *y, double *z, Mat_int l)
{for (Mat_int i = 0; i<l; ++i) z[i] = x[i] - y[i];}
inline void Mat_xy_mul (double *x, double *y, double *z, Mat_int l)
{for (Mat_int i = 0; i<l; ++i) z[i] = x[i] * y[i];}
inline void Mat_xy_div (double *x, double *y, double *z, Mat_int l)
{for (Mat_int i = 0; i<l; ++i) z[i] = x[i] / y[i];}
inline double Mat_xy (double *x, double *y, Mat_int l) 
{ // inner product
	double z = 0;
	for (Mat_int i = 0; i<l; ++i) z+= x[i] * y[i];
	return z;
} 
inline double Mat_norm (double *x, Mat_int l)
{	
	double z = 0;
	for (Mat_int i = 0; i<l; ++i) z+= x[i] * x[i];
	return sqrt(z);

}

// BLAS 2
inline void Mat_Ax_y   (double *A, Mat_int Arow, Mat_int Acol, double *x, double *y)  
{// y = A * x
	for (Mat_int i = 0; i < Arow; ++i)
	{
		double tmp = 0; 
		for(Mat_int j = 0; j < Acol; ++j)
			tmp += A[ i + Arow*j] * x[j];
		y[i] = tmp;
	}	
}
inline void Mat_Atx_y  (double *A, Mat_int Arow, Mat_int Acol, double *x, double *y)
{// y = At * x
	for(Mat_int j = 0; j < Acol; ++j)
	{
		double tmp = 0; 			
		for (Mat_int i = 0; i < Arow; ++i)
			tmp += A[ i + Arow*j] * x[i];
		y[j] = tmp;
	}
}

// BLAS 3
inline void Mat_AB_C( double* A, Mat_int Arow, Mat_int Acol, double* B, Mat_int Brow, Mat_int Bcol, double *C) // declar inline later is faster?
{// C = A*B
 // Acol = Brow
	for ( Mat_int m = 0 ; m < Arow ; ++m)			
		for ( Mat_int n = 0 ; n < Bcol ; ++n)									
		{
			double tmp = 0.;
			for ( Mat_int k = 0 ; k < Acol ; ++k)
				tmp += A[m + k*Arow] * B[k + n*Brow]; 
			C[m + n*Arow] = tmp; 	
		}
}
inline void Mat_AtB_C( double* A, Mat_int Arow, Mat_int Acol, double* B, Mat_int Brow, Mat_int Bcol, double *C)
{// C = At * B
 // Arow = Brow
	for ( Mat_int n = 0 ; n < Bcol ; ++n)
		for ( Mat_int m = 0 ; m < Acol ; ++m)					
		{
			double tmp = 0.;
			for ( Mat_int k = 0 ; k < Arow ; ++k)
				tmp += A[k + m*Arow] * B[k + n*Brow];
			C[m + n*Acol] = tmp; 	
		}
}

// Diagonal Matrix Summation
inline void Mat_A_D_plu   (double *A, Mat_int Arow, Mat_int Acol, double *D) // A = A + D
{for (Mat_int i = 0; i<Arow; ++i) A[i+i*Arow] += D[i];};
inline void Mat_A_D_min   (double *A, Mat_int Arow, Mat_int Acol, double *D) // A = A - D
{for (Mat_int i = 0; i<Arow; ++i) A[i+i*Arow] -= D[i];};
inline void Mat_D_A_min   (double *A, Mat_int Arow, Mat_int Acol, double *D) // A = D - A
{for (Mat_int i = 0; i<Arow; ++i) A[i+i*Arow] = D[i] - A[i+i*Arow] ;};
inline void Mat_I_A_plu  (double *A, Mat_int Arow, Mat_int Acol, double alpha ) // A = A + alpha*I;
{for (Mat_int i = 0; i<Arow; ++i) A[i+i*Arow] += alpha;};
inline void Mat_A_I_min  (double *A, Mat_int Arow, Mat_int Acol, double alpha ) // A = A - alpha * I
{for (Mat_int i = 0; i<Arow; ++i) A[i+i*Arow] -= alpha;};
inline void Mat_I_A_min  (double *A, Mat_int Arow, Mat_int Acol, double alpha ) // A = alpha * I - A
{
	for (Mat_int i = 0; i<Arow*Acol; ++i)	A[i] = -A[i];
	for (Mat_int i = 0; i<Arow; ++i) A[i+i*Arow] += alpha;	
};



inline void Mat_inv22 (double *A, double *invA)
{
	double det = A[0]*A[3] - A[1]*A[2];
	invA[0] =  A[3] / det;
	invA[1] =- A[2] / det;
	invA[2] =- A[1] / det;
	invA[3] =  A[0] / det;
}

#endif