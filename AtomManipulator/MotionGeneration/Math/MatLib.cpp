#include "MatLib.h"


//==== Print ====//
void Mat_error(char * error)
{
	cout<<error<<endl;
	system("pause");
}

void Mat_Show( const double *A, const Mat_int Arow, const Mat_int Acol, const  char* text)
{
	cout<<"----------------------------------------------------"<<endl;

	if (text)
		cout<<text<<" = "<<endl;

	for ( Mat_int i = 0; i < Arow ; ++i)
	{
		for ( Mat_int j = 0 ; j < Acol ; ++j)
		{			
			if (A[i + j*Arow]>=0)
				printf("    %4.2f", A[i + j*Arow]);
			else
				printf("   %4.2f", A[i + j*Arow]);
		}
		cout<<endl;
	}	
	cout<<endl;
	cout<<"----------------------------------------------------"<<endl;
}

//==== BLAS 2 ====//
void Mat_Ax_by_large (double *A, Mat_int Arow, Mat_int Acol, double *x, double *y, double beta, char TransA)
{ // y = A*x + beta*y
	double alpha = 1;
	Mat_int inc = 1; 
	dgemv_( &TransA, &Arow, &Acol, &alpha, A, &Arow, x, &inc, &beta, y, &inc); // y = alpha*A*x + beta*y
	
}

void Mat_Ax_by (double *A, Mat_int Arow, Mat_int Acol, double *x, double *y,  double beta,  char TransA)
{
	if (TransA == 'N')
	{
		for (Mat_int i = 0; i < Arow; ++i)
		{
			double tmp = 0; 
			for(Mat_int j = 0; j < Acol; ++j)
				tmp += A[ i + Arow*j] * x[j];
			y[i] = tmp + beta * y[i];		
		}	
	}
	else
	{// faster
		for(Mat_int j = 0; j < Acol; ++j)
		{
			double tmp = 0; 			
			for (Mat_int i = 0; i < Arow; ++i)
				tmp += A[ i + Arow*j] * x[i];
			y[j] = tmp + beta*y[j];		
		}
	}
}

void Mat_ADBx_by  (double *A, Mat_int Arow, Mat_int Acol, double *D, double *B, Mat_int Brow, Mat_int Bcol, double *x, double *y, double beta, char TransA, char TransB)
{	// y = A*D*B*x + beta*y
		
	static Mat_int _l = 0; // size of static memory
	static double*z = NULL;
	
	Mat_int l =  ((TransA =='N') ? Acol : Arow); // length of D

	if (_l < l)
	{
		if (z != NULL)
			delete [] z; 

		_l = 2*l; 		
		z = new double [_l];
	}	

	Mat_Ax_by(B, Brow, Bcol, x, z, 0, TransB);		
	Mat_xy_mul(z, D, z, l); // z = D*z	
	Mat_Ax_by(A, Arow, Acol, z, y, beta, TransA);	
}



//==== BLAS 3 ====//
void Mat_AB_bC_large (double *A, Mat_int Arow, Mat_int Acol, double *B, Mat_int Brow, Mat_int Bcol, double *C, double beta, char TransA, char TransB)
{
	double alpha = 1; 
	Mat_int M = TransA == 'N' ? Arow : Acol;
	Mat_int K = TransA == 'N' ? Acol : Arow;
	Mat_int N = TransB == 'N' ? Bcol : Brow;
	
	//alpha*op( A )*op( B ) + beta*C,
	dgemm_(&TransA, &TransB, &M, &N, &K, &alpha,  A, &Arow, B, &Brow, &beta, C, &M);	
}

void Mat_AB_bC(double* A, Mat_int Arow, Mat_int Acol, double* B, Mat_int Brow, Mat_int Bcol, double *C, double beta, char TransA)
{// C = A*B + beta * C	
								
	if (TransA == 'N')
	{// Acol = Brow
		for ( Mat_int m = 0 ; m < Arow ; ++m)			
			for ( Mat_int n = 0 ; n < Bcol ; ++n)
			{
				double tmp = 0.;
				for ( Mat_int k = 0 ; k < Acol ; ++k)
					tmp += A[m + k*Arow] * B[k + n*Brow];
				C[m + n*Arow] = tmp + beta * C[m + n*Arow]; 	
			}
	}
	else
	{// Arow = Brow
		for ( Mat_int n = 0 ; n < Bcol ; ++n)
			for ( Mat_int m = 0 ; m < Acol ; ++m)						
			{
				double tmp = 0.;
				for ( Mat_int k = 0 ; k < Arow ; ++k)
					tmp += A[k + m*Arow] * B[k + n*Brow];
				C[m + n*Acol] = tmp + beta * C[m + n*Acol]; 	
			}

	}
}


// Diagonal Matrix Multiplication
void Mat_ADB_C   (double *A, Mat_int Arow, Mat_int Acol, double *D, double *B, Mat_int Brow, Mat_int Bcol, double *C, char TransA, char TransB)
{// C = A*D*B

	Mat_int k1 =  ((TransA =='N') ? Acol : Arow); 
	Mat_int k2 =  ((TransB =='N') ? Brow : Bcol); 

	double *tmp = new double [k1*k2];
	Mat_DA_C ( B, Brow, Bcol, D, tmp, TransB); // tmp = D*B;
	Mat_AB_bC (A, Arow, Acol, tmp, k1, k2, C, 0, TransA); // C = A * B_tmp
	delete [] tmp;
}

void Mat_D1AD2_C (double *A, Mat_int Arow, Mat_int Acol, double *D1, double *D2, double *C, char TransA)
{ // C = D1*A*D2 

	if (TransA == 'N')
	{
		for (Mat_int j = 0; j<Acol; ++j)
			for (Mat_int i = 0; i<Arow; ++i)
				C[i + j*Arow] = D1[i] * A[i + j*Arow] * D2[j];
	}
	else
	{// C = D1 * At * D2
		for (Mat_int j = 0; j<Acol; ++j)
			for (Mat_int i = 0; i<Arow; ++i)			
				C[j + i*Acol] = D2[i] * A[i + j*Arow] * D1[j]; 
	}

}

void Mat_DA_C    (double *A, Mat_int Arow, Mat_int Acol, double *D,  double *C, char TransA)
{// C = D*A
	if (TransA == 'N')
	{
		for (Mat_int j = 0; j<Acol; ++j)
			for (Mat_int i = 0; i<Arow; ++i)
				C[i + j*Arow] = D[i] * A[i + j*Arow] ;
	}
	else
	{// C = D * At
		for (Mat_int j = 0; j<Acol; ++j)
			for (Mat_int i = 0; i<Arow; ++i)			
				C[j + i*Acol] = A[i + j*Arow] * D[j]; 
	}
}

void Mat_AD_C    (double *A, Mat_int Arow, Mat_int Acol, double *D,  double *C, char TransA)
{ // C = A*D
	if (TransA == 'N')
	{
		for (Mat_int j = 0; j<Acol; ++j)
			for (Mat_int i = 0; i<Arow; ++i)
				C[i + j*Arow] = A[i + j*Arow] * D[j];
	}
	else
	{// C = At * D2
		for (Mat_int j = 0; j<Acol; ++j)
			for (Mat_int i = 0; i<Arow; ++i)			
				C[j + i*Acol] = D[i] * A[i + j*Arow]; 
	}
}


//
void Mat_svd ( double* J, Mat_int Jrow, Mat_int Jcol,  double *U, double *S, double *Vt)
{

	// !!!!!!!!!!!!! NOTE: J will corrupt on exit
	//
	// Compute the SVD of J = U * S * Vt
	// U -             Jrow * min(Jrow, Jcol)
	// Vt - min(Jrow, Jcol) * Jcol ( NOT V !!!)
	// S -  min(Jrow, Jcol) * 1	


	static Mat_int _lwork = 0;
	static double* work = NULL;

	char Job = 'S';	
	Mat_int info; 
	Mat_int r = min(Jrow, Jcol);	
	Mat_int lwork = 10*r;

	if (_lwork < lwork)
	{
		if (work != NULL)
			delete [] work; 

		_lwork = 2*lwork; 		
		work = new double [_lwork];
	}


	dgesvd_(&Job, &Job, &Jrow, &Jcol, J, &Jrow, S, U, &Jrow, Vt, &r, work, &lwork, &info);


	if (info!=0)
		Mat_error("SVD fails to converge");
	
}

