#include "MatFunction.h"



//void Mat_Ax_by (double *A, long Arow, long Acol, double *x, double *y, double beta, char TransA)
//{ // y = A*x + beta*y
//	double alpha = 1;
//	integer inc = 1; 
//	dgemv_( &TransA, &Arow, &Acol, &alpha, A, &Arow, x, &inc, &beta, y, &inc); // y = alpha*A*x + beta*y
//
//}
// A = zeros(m,n)
void MatInitialize( int m, int n, Mat &A, double *raw)
{
	A.row = m;
	A.col = n;
	A.len = m*n;

	if (NULL == A.data)
		A.data = (double *)malloc((size_t)A.len*sizeof(double));

	if (raw)
		memcpy( A.data, raw, (size_t)A.len*sizeof(double));
	else
		memset( A.data, 0.0, (size_t)A.len*sizeof(double));
}

// B = [A,;];
void MatRealloc( Mat &A, int m, int n)
{
	A.row = m;
	A.col = n;
	A.len = m*n;

	double *newA = (double *)realloc( A.data, (size_t)A.len*sizeof(double));
	A.data = newA;
}

// B = A
void MatCopy( Mat &A, Mat &B)
{
	B.row = A.row;
	B.col = A.col;
	B.len = A.len;

	if (NULL == B.data)
		B.data = (double *)malloc((size_t)B.len*sizeof(double));
	memcpy( B.data, A.data, (size_t)B.len*sizeof(double));
}


// C = A+B
void MatAdd( Mat &A, Mat &B, Mat &C) 
{
	if ( B.row != A.row || B.col != A.col)
	{
		cerr<<"[Error:CMatrix+] Matrix dimensions must agree."<<endl;
		system("pause");
		exit(0);
	}

	MatCopy( A, C);

	Add_AB( A.data, B.data, C.data, C.len);
}
void MatAdd( Mat &A, MN &Amn, Mat &B, MN &Bmn, Mat &C) 
{
	if (Amn.r1<0)
	{
		Amn.r1 = 0;
		Amn.r2 = A.row-1;
		Amn.c1 = 0;
		Amn.c2 = A.col-1;
	}

	if (Bmn.r1<0)
	{
		Bmn.r1 = 0;
		Bmn.r2 = B.row-1;
		Bmn.c1 = 0;
		Bmn.c2 = B.col-1;
	}

	int row = Amn.r2-Amn.r1+1;
	int col = Amn.c2-Amn.c1+1;

	if ( Bmn.r2-Bmn.r1+1 != row || Bmn.c2-Bmn.c1+1 != col)
	{
		cerr<<"[Error:CMatrix+] Matrix dimensions must agree."<<endl;
		system("pause");
		exit(0);
	}

	MatInitialize( row, col, C);

	Mat A1, B1;
	if ( !Amn.Full && Bmn.Full)
	{
		MatSubM( A, Amn, A1);
		Add_AB( A1.data, B.data, C.data, C.len);
	}
	else if ( Amn.Full && !Bmn.Full)
	{
		MatSubM( B, Bmn, B1);
		Add_AB( A.data, B1.data, C.data, C.len);
	}
	else if ( !Amn.Full && !Bmn.Full)
	{
		MatSubM( A, Amn, A1);
		MatSubM( B, Bmn, B1);
		Add_AB( A1.data, B1.data, C.data, C.len);
	}

}

// A += B
void MatAddEq( Mat &A, Mat &B) 
{
	if ( B.row != A.row || B.col != A.col)
	{
		cerr<<"[Error:CMatrix+] Matrix dimensions must agree."<<endl;
		system("pause");
		exit(0);
	}

	Add_AB( A.data, B.data, A.data, A.len);
}

// C = A-B
void MatMinus( Mat &A, Mat &B, Mat &C) 
{
	if ( B.row != A.row || B.col != A.col)
	{
		cerr<<"[Error:CMatrix-] Matrix dimensions must agree."<<endl;
		system("pause");
		exit(0);
	}

	MatCopy( A, C);

	Miu_AB( A.data, B.data, C.data, C.len);
}
void MatMinus( Mat &A, MN &Amn, Mat &B, MN &Bmn, Mat &C) 
{
	if (Amn.r1<0)
	{
		Amn.r1 = 0;
		Amn.r2 = A.row-1;
		Amn.c1 = 0;
		Amn.c2 = A.col-1;
	}

	if (Bmn.r1<0)
	{
		Bmn.r1 = 0;
		Bmn.r2 = B.row-1;
		Bmn.c1 = 0;
		Bmn.c2 = B.col-1;
	}

	int row = Amn.r2-Amn.r1+1;
	int col = Amn.c2-Amn.c1+1;

	if ( Bmn.r2-Bmn.r1+1 != row || Bmn.c2-Bmn.c1+1 != col)
	{
		cerr<<"[Error:CMatrix-] Matrix dimensions must agree."<<endl;
		system("pause");
		exit(0);
	}

	MatInitialize( row, col, C);

	Mat A1, B1;
	if ( !Amn.Full && Bmn.Full)
	{
		MatSubM( A, Amn, A1);
		Miu_AB( A1.data, B.data, C.data, C.len);
	}
	else if ( Amn.Full && !Bmn.Full)
	{
		MatSubM( B, Bmn, B1);
		Miu_AB( A.data, B1.data, C.data, C.len);
	}
	else if ( !Amn.Full && !Bmn.Full)
	{
		MatSubM( A, Amn, A1);
		MatSubM( B, Bmn, B1);
		Miu_AB( A1.data, B1.data, C.data, C.len);
	}

}

// A -= B
void MatMinusEq( Mat &A, Mat &B) 
{
	if ( B.row != A.row || B.col != A.col)
	{
		cerr<<"[Error:CMatrix+] Matrix dimensions must agree."<<endl;
		system("pause");
		exit(0);
	}

	Miu_AB( A.data, B.data, A.data, A.len);
}

// C = A*B
void MatAB( Mat &A, Mat &B, Mat &C, int dot)
{
	if (dot)
	{
		if ( B.row != A.row || B.col != A.col)
		{
			cerr<<"[Error:CMatrix*] Matrix dimensions must agree."<<endl;
			system("pause");
			exit(0);
		}

		MatCopy( A, C);

		for ( int y = 0 ; y < C.row ; y++)
		{
			for ( int x = 0 ; x < C.col ; x++)
			{
				C.data[y*C.col+x] = A.data[y*C.col+x]*B.data[y*C.col+x];
			}
		}
	} 
	else
	{
		if ( B.row != A.col)
		{
			cerr<<"[Error:CMatrix*] Matrix dimensions must agree."<<endl;
			system("pause");
			exit(0);
		}

		MatInitialize( A.row, B.col, C);

		for ( int y = 0 ; y < C.row ; y++)
		{
			for ( int x = 0 ; x < C.col ; x++)
			{
				double tmp = 0.;
				for ( int k = 0 ; k < A.col ; k++)
				{
					tmp += A.data[y*A.col+k] * B.data[k*B.col+x];
				}
				C.data[y*C.col+x] = tmp;
			}
		}
	}
}
void MatAB( Mat &A, MN &Amn, Mat &B, MN &Bmn, Mat &C)
{
	if (Amn.r1<0)
	{
		Amn.r1 = 0;
		Amn.r2 = A.row-1;
		Amn.c1 = 0;
		Amn.c2 = A.col-1;
	}

	if (Bmn.r1<0)
	{
		Bmn.r1 = 0;
		Bmn.r2 = B.row-1;
		Bmn.c1 = 0;
		Bmn.c2 = B.col-1;
	}

	int row = Amn.r2-Amn.r1+1;
	int col = Bmn.c2-Bmn.c1+1;

	if ( Bmn.r2-Bmn.r1+1 != Amn.c2-Amn.c1+1)
	{
		cerr<<"[Error:CMatrix*] Matrix dimensions must agree."<<endl;
		system("pause");
		exit(0);
	}

	MatInitialize( row, col, C);

	for ( int y = 0 ; y < C.row ; y++)
	{
		for ( int x = 0 ; x < C.col ; x++)
		{
			double tmp = 0.;
			for ( int k = Amn.c1 ; k <= Amn.c2 ; k++)
			{
				tmp += A.data[(Amn.r1+y)*A.col+k] * B.data[k*B.col+(Bmn.c1+x)];
			}
			C.data[y*C.col+x] = tmp;
		}
	}
}
void MatABrow( double *A, int Arow, int Acol, double *B, int Brow, int Bcol, double *C)
{
	//MatInitialize( A.row, B.col, C);
	for ( int y = 0 ; y < Arow ; y++)
	{
		for ( int x = 0 ; x < Bcol ; x++)
		{
			double tmp = 0.;
			for ( int k = 0 ; k < Acol ; k++)
			{
				tmp += A[y*Acol+k] * B[k*Bcol+x];
			}
			C[y*Bcol+x] = tmp;
		}
	}
}
void MatABcol( double *A, int Arow, int Acol, double *B, int Brow, int Bcol, double *C)
{
	//MatInitialize( A.row, B.col, C);
	for ( int x = 0 ; x < Bcol ; x++)
	{
		for ( int y = 0 ; y < Arow ; y++)
		{
			double tmp = 0.;
			for ( int k = 0 ; k < Acol ; k++)
			{
				tmp += A[k*Arow+y] * B[x*Brow+k];
			}
			C[x*Arow+y] = tmp;
		}
	}
}
//void MatABcol( double* A, int Arow, int Acol, double* B, int Brow, int Bcol, double *C) // declar inline later is faster?
//{// C = A*B
//	// Acol = Brow
//	for ( int m = 0 ; m < Arow ; ++m)			
//		for ( int n = 0 ; n < Bcol ; ++n)									
//		{
//			double tmp = 0.;
//			for ( int k = 0 ; k < Acol ; ++k)
//				tmp += A[m + k*Arow] * B[k + n*Brow]; 
//			C[m + n*Arow] = tmp; 	
//		}
//}

// C = A*scalar
void MatAs( Mat &A, double B, Mat &C)
{
	MatCopy( A, C);

	for ( int k = 0 ; k < A.len ; k++)
	{
		C.data[k] = A.data[k] * B;
	}
}
void MatAs( Mat &A, MN &Amn, double B, Mat &C)
{
	int row = Amn.r2-Amn.r1+1;
	int col = Amn.c2-Amn.c1+1;

	MatInitialize( row, col, C);

	Mat A1;
	MatSubM( A, Amn, A1);

	for ( int k = 0 ; k < A1.len ; k++)
	{
		C.data[k] = A1.data[k] * B;
	}
}

// A *= scalar
void MatAsEq( Mat &A, double B)
{
	for ( int k = 0 ; k < A.len ; k++)
	{
		A.data[k] *= B;
	}
}
void MatAsEq( Mat &A, MN &Amn, double B)
{
	for ( int y = Amn.r1 ; y <= Amn.r2 ; y++)
	{
		for ( int x = Amn.c1 ; x <= Amn.c2 ; x++)
		{
			A.data[y*A.col+x] *= B;
		}
	}
}

// A /= scalar
void MatOsEq( Mat &A, double B)
{
	if ( B == 0)
	{
		cerr<<"[Error:CMatrix/] NaN"<<endl;
		system("pause");
		exit(0);
	}

	for ( int k = 0 ; k < A.len ; k++)
	{
		A.data[k] /= B;
	}
}
void MatOsEq( Mat &A, MN &Amn, double B)
{
	if ( B == 0)
	{
		cerr<<"[Error:CMatrix/] NaN"<<endl;
		system("pause");
		exit(0);
	}

	for ( int y = Amn.r1 ; y <= Amn.r2 ; y++)
	{
		for ( int x = Amn.c1 ; x <= Amn.c2 ; x++)
		{
			A.data[y*A.col+x] /= B;
		}
	}
}

void MatSum( Mat &A, Mat &B, DIR V)
{
	if ( V == MAT_VERT)
	{
		MatInitialize( 1, A.col, B);
		for ( int i = 0 ; i < A.col ; i++)
		{
			for ( int j = 0 ; j < A.row ; j++)
			{
				B.data[i] += A.data[j*A.col+i];
			}
		}
	} 
	else
	{
		MatInitialize( A.row, 1, B);
		for ( int i = 0 ; i < A.row ; i++)
		{
			for ( int j = 0 ; j < A.col ; j++)
			{
				B.data[i] += A.data[i*A.col+j];
			}
		}
	}
}

// A = A'
//void MatTself( Mat &A)
//{
//	Mat tmp( A.col, A.row);
//
//	for ( int i = 0 ; i < tmp.row ; i++)
//	{
//		for ( int j = 0 ; j < tmp.col ; j++)
//		{
//			tmp.data[i*tmp.col+j] = A.data[j*A.col+i];
//		}
//	}
//
//	for ( int i = 0 ; i < A.len ; i++)
//		A.data[i] = tmp.data[i];
//
//	A.row = tmp.row;
//	A.col = tmp.col;
//}

// B = A'
void MatT( Mat &A, Mat &B)
{
	MatInitialize( A.col, A.row, B);

	for ( int i = 0 ; i < B.row ; i++)
	{
		for ( int j = 0 ; j < B.col ; j++)
		{
			B.data[i*B.col+j] = A.data[j*A.col+i];
		}
	}
}
void MatTcol( double *A, int Arow, int Acol, double *B)
{
	//MatInitialize( A.col, A.row, B);

	for ( int i = 0 ; i < Arow ; ++i)
	{
		for ( int j = 0 ; j < Acol ; ++j)
		{
			B[i*Acol+j] = A[j*Arow+i];
		}
	}
}

// B = A(r1:r2,c1:c2)
void MatSubM( Mat &A, MN &mn, Mat &B)
{
	if ( mn.r1>mn.r2 || mn.c1>mn.c2)
	{
		cerr<<"[Error:CMatrix SubM] Wrong index."<<endl;
		system("pause");
		exit(0);
	}
	MatInitialize( mn.r2-mn.r1+1, mn.c2-mn.c1+1, B);

	for ( int i = 0 ; i < B.row ; i++)
	{
		for ( int j = 0 ; j < B.col ; j++)
		{
			B.data[i*B.col+j] = A.data[(mn.r1+i)*A.col+(mn.c1+j)];
		}
	}
}

// A(r1:r2,c1:c2) = B
void MatReplaceRow( double *A, double *B, int len)
{
	memcpy( A, B, len * sizeof(double));
}
void MatReplace( Mat &A, MN &mn, Mat &B)
{
	//if ( r2-r1+1 > B.row || c2-c1+1 > B.col)
	//{
	//	cerr<<"[Error:CMatrix Replace] Wrong dims."<<endl;
	//	system("pause");
	//	exit(0);
	//}

	for ( int i = mn.r1 ; i <= mn.r2 ; i++)
	{
		for ( int j = mn.c1 ; j <= mn.c2 ; j++)
		{
			A.data[i*A.col+j] = B.data[(i-mn.r1)*B.col+(j-mn.c1)];
		}
	}
}
void MatReplace( Mat &A, MN &mn, double &B)
{
	//if ( r2-r1+1 > B.row || c2-c1+1 > B.col)
	//{
	//	cerr<<"[Error:CMatrix Replace] Wrong dims."<<endl;
	//	system("pause");
	//	exit(0);
	//}

	//for ( int i = r1 ; i <= r2 ; i++)
	//{
	//	for ( int j = c1 ; j <= c2 ; j++)
	//	{
			A.data[mn.r1*A.col+mn.c1] = B;
	//	}
	//}
}

// C = [ A, B] or [ A; B]
void MatNMat( Mat &A, Mat &B, DIR V, Mat &C)
{
	//  V = 1, C = [ A, B];
	// V = -1, C = [ A; B];

	if (V>0)
	{
		if ( A.row != B.row)
		{
			cerr<<"[Error:CMatrix NMat] Wrong row dim."<<endl;
			system("pause");
			exit(0);
		}
		
		if (C.data != A.data)
		{
			MatInitialize( A.row, A.col+B.col, C);

			for ( int y = 0 ; y < A.row ; y++)
			{
				for ( int x1 = 0; x1 < A.col ; x1++)
				{
					C.data[y*C.col+x1] = A.data[y*A.col+x1];
				}

				for ( int x2 = 0; x2 < B.col ; x2++)
				{
					C.data[y*C.col+A.col+x2] = B.data[y*B.col+x2];
				}
			}
		} 
		else
		{
			Mat AA;
			MatCopy( A, AA);

			MatRealloc( A, A.row, A.col+B.col);

			for ( int y = 0 ; y < AA.row ; y++)
			{
				for ( int x1 = 0; x1 < AA.col ; x1++)
				{
					A.data[y*A.col+x1] = AA.data[y*AA.col+x1];
				}

				for ( int x2 = 0; x2 < B.col ; x2++)
				{
					A.data[y*A.col+AA.col+x2] = B.data[y*B.col+x2];
				}
			}
		}
	} 
	else
	{
		if ( A.col != B.col)
		{
			cerr<<"[Error:CMatrix NMat] Wrong col dim."<<endl;
			system("pause");
			exit(0);
		}

		if (C.data != A.data)
		{
			MatInitialize( A.row+B.row, A.col, C);

			for ( int x = 0 ; x < A.col ; x++)
			{
				for ( int y1 = 0; y1 < A.row ; y1++)
				{
					C.data[y1*C.col+x] = A.data[y1*A.col+x];
				}

				for ( int y2 = 0; y2 < B.row ; y2++)
				{
					C.data[(A.row+y2)*C.col+x] = B.data[y2*B.col+x];
				}
			}
		} 
		else
		{
			Mat AA;
			MatCopy( A, AA);

			MatRealloc( A, A.row+B.row, A.col);

			for ( int x = 0 ; x < AA.col ; x++)
			{
				for ( int y1 = 0; y1 < AA.row ; y1++)
				{
					A.data[y1*A.col+x] = AA.data[y1*AA.col+x];
				}

				for ( int y2 = 0; y2 < B.row ; y2++)
				{
					A.data[(AA.row+y2)*A.col+x] = B.data[y2*B.col+x];
				}
			}
		}
	}
}

// [ V, IX] = min/max(A)
void MatMinMax( Mat &A, CompareState state, VIX &V)
{
	if (state>0)
	{
		V.Val = -1.0*DBL_MAX;
		for ( int i = 0 ; i < A.len ; i++)
		{
			if ( A.data[i]>V.Val)
			{
				V.Val = A.data[i];
				V.IX = i;
			}
		}
	} 
	else
	{
		V.Val = DBL_MAX;
		for ( int i = 0 ; i <A.len ; i++)
		{
			if ( A.data[i]<V.Val)
			{
				V.Val = A.data[i];
				V.IX = i;
			}
		}
	}
}

// Compute the minimum-norm solution to a real linear least squares problem
// minimize 2-norm (|b-A*x|)
void MatB_Ax( Mat &A, Mat &B, Mat &X)
{
	integer m = A.row;
	integer n = A.col;
	integer nrhs = B.col;
	integer lda = A.row;
	integer ldb = B.row;
	doublereal rcond = 1e-5;
	doublereal wkopt;
	doublereal *work;
	integer *iwork = new integer[3*m*0+11*m];
	integer info, rank;
	
	doublereal S[3];
	MatCopy( B, X);

	integer lwork = -1;
	dgelsd_( &m, &n, &nrhs, A.data, &lda, X.data, &ldb, S, &rcond, &rank, &wkopt, &lwork, iwork, &info);

	lwork = (integer)wkopt;
	work = new doublereal[lwork];
	dgelsd_( &m, &n, &nrhs, A.data, &lda, X.data, &ldb, S, &rcond, &rank, work, &lwork, iwork, &info);

	/* Check for convergence */
	if( info != 0 ) {
		printf( "The algorithm computing SVD failed to converge;\n" );
		printf( "the least squares solution could not be computed.\n" );
		system("pause");
	}

	delete [] work;
	delete [] iwork;
}
// B = inv(A)
void MatInv( Mat &A, Mat &B)
{
	if ( A.row != A.col)
	{
		cerr<<"[Error:CMatrix Inv] Not square mat."<<endl;
		system("pause");
		exit(0);
	}

	if (A.row == 2)
	{
		double det = Cross2x2( A.data[0], A.data[1], A.data[2], A.data[3]);
		if ( det == 0.)
		{
			cout<<"Det: "<<det<<endl;
			cerr<<"[Error:CMatrix Inv] Matrix is close to singular or badly scaled."<<endl;
			//system("pause");
			//exit(0);
		}
	} 
	else if ( A.row == 3)
	{
		double det = (A.data[0]*A.data[4]*A.data[8] + 
			A.data[3]*A.data[7]*A.data[2] + 
			A.data[6]*A.data[1]*A.data[5]) - 
			(A.data[2]*A.data[4]*A.data[6] +
			A.data[1]*A.data[3]*A.data[8] +
			A.data[0]*A.data[5]*A.data[7]);

		if ( det == 0.)
		{
			cout<<"Det: "<<det<<endl;
			cerr<<"[Error:CMatrix Inv] Matrix is close to singular or badly scaled."<<endl;
			//system("pause");
			//exit(0);
		}
	}

	MatCopy( A, B);

	integer Dim = A.col;
	doublereal *work = new doublereal[Dim*Dim];
	integer INFO; 

	integer * ipiv = new integer[Dim*Dim];

	dgetrf_(&Dim,&Dim, B.data,&Dim,ipiv,&INFO);

	if (INFO != 0)
		cout << "Fail to Calculate LU decomposition" << endl;

	dgetri_(&Dim, B.data,&Dim,ipiv,work,&Dim,&INFO);

	if (INFO != 0)
		cout << "Fail to Calculate Matrix Inversion" << endl;

	delete[] work;
	delete[] ipiv;

	//} 
	//else
	//{
	//	if ( A.row == 2)
	//	{
	//		double det = Cross2x2( A.data[0], A.data[1], A.data[2], A.data[3]);

	//		if ( det == 0.)
	//		{
	//			cerr<<"[Error:CMatrix Inv] Matrix is close to singular or badly scaled."<<endl;
	//			system("pause");
	//			exit(0);
	//		}

	//		MatCopy( A, B);

	//		B.data[0] = A.data[3];
	//		B.data[3] = A.data[0];
	//		B.data[1] = -A.data[1];
	//		B.data[2] = -A.data[2];

	//		MatOsEq( B, det);
	//	} 
	//	else if ( A.row == 3)
	//	{
			//double det = (A.data[0]*A.data[4]*A.data[8] + 
			//	A.data[3]*A.data[7]*A.data[2] + 
			//	A.data[6]*A.data[1]*A.data[5]) - 
			//	(A.data[2]*A.data[4]*A.data[6] +
			//	A.data[1]*A.data[3]*A.data[8] +
			//	A.data[0]*A.data[5]*A.data[7]);

			//if ( det == 0.)
			//{
			//	cerr<<"[Error:CMatrix Inv] Matrix is close to singular or badly scaled."<<endl;
			//	//system("pause");
			//	//exit(0);
			//}

	//		MatCopy( A, B);

	//		int ind[4] = {-1};
	//		int cnt = 0;

	//		for ( int y = 0 ; y < 3 ; y++ ) {
	//			for ( int x = 0 ; x < 3 ; x++ ) {
	//				cnt = 0;
	//				for ( int yy = 0 ; yy < 3 ; yy++ ) {
	//					for ( int xx = 0 ; xx < 3 ; xx++ ) {
	//						if (yy != y && xx != x) {
	//							ind[cnt] = yy*A.col+xx;
	//							//cout<<cnt<<"     "<<y<<" "<<yy<<"    "<<x<<" "<<xx<<"    "<<yy*col+xx<<endl;
	//							cnt++;
	//						}
	//					}
	//				}
	//				if ( (y*A.col+x)%2 > 0 )
	//					B.data[y*A.col+x] = -1.0 * Cross2x2( A.data[ind[0]], A.data[ind[1]], A.data[ind[2]], A.data[ind[3]]);
	//				else
	//					B.data[y*A.col+x] = Cross2x2( A.data[ind[0]], A.data[ind[1]], A.data[ind[2]], A.data[ind[3]]);
	//			}
	//		}

	//		Mat Bt;
	//		MatT( B, Bt);

	//		MatOsEq( B, det);
	//	}
	//}
}

// C = A( r, c)
double MatGet( Mat &A, int r, int c)
{
	if ( r >= A.row || c >= A.col)
	{
		cerr<<"[Error:CMatrix Get] Wrong index."<<endl;
		system("pause");
		exit(0);
	}

	return A.data[r*A.col+c];
}

// Show matrix
void MatShow( Mat &A, char text[])
{
	//cout<<"***********************************************"<<endl;

	if (text)
		cout<<text<<" = "<<endl;

	for ( int i = 0; i < A.row ; i++)
	{
		for ( int j = 0 ; j < A.col ; j++)
		{
			cout<<A.data[i*A.col+j]<<"  ";
		}
		cout<<endl;
	}
	//cout<<"***********************************************"<<endl;
	cout<<endl;
}

void MatShowCol( double *A, int Arow, int Acol, char text[])
{
	//cout<<"***********************************************"<<endl;

	if (text)
		cout<<text<<" = "<<endl;

	for ( int i = 0; i < Arow ; i++)
	{
		for ( int j = 0 ; j < Acol ; j++)
		{
			cout<<A[j*Arow+i]<<"  ";
		}
		cout<<endl;
	}
	//cout<<"***********************************************"<<endl;
	cout<<endl;
}

// Show matrix
void MatShow( Mat &A, Mat &B, Mat &C, char text[])
{
	for ( int i = 0; i < C.row ; i++)
	{
		for ( int j = 0 ; j < C.col ; j++)
		{
			cout<<C.data[i*C.col+j]<<"  ";
		}
		cout<<endl;
	}
	cout<<endl;

	cout<<"   || "<<endl;
	cout<<endl;

	for ( int i = 0; i < A.row ; i++)
	{
		for ( int j = 0 ; j < A.col ; j++)
		{
			cout<<A.data[i*A.col+j]<<"  ";
		}
		cout<<endl;
	}
	cout<<endl;

	if (text)
		cout<<"   "<<text<<endl;
	cout<<endl;

	for ( int i = 0; i < B.row ; i++)
	{
		for ( int j = 0 ; j < B.col ; j++)
		{
			cout<<B.data[i*B.col+j]<<"  ";
		}
		cout<<endl;
	}
	cout<<endl;
}