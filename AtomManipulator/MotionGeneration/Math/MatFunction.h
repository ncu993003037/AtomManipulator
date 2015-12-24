#include <windows.h>
#include <float.h>

#include <vector>

#include <iostream>

#include <f2c.h>
#include "blaswrap.h"
extern "C"
{
#include <clapack.h> 
}

using namespace std;

typedef enum {
	MAT_MIN        = 0, 
	MAT_MAX        = 1, 
} CompareState;

typedef enum {
	MAT_HORI        = 1,
	MAT_VERT        = -1,
} DIR;

typedef struct _VIX
{
	double Val;
	int IX;
} VIX;

struct MN
{
	int r1;
	int r2;
	int c1;
	int c2;
	bool Full;

	MN( int row1, int row2, int col1, int col2) : r1(row1), r2(row2), c1(col1), c2(col2), Full(false) { }
	MN( void) : r1(-1), r2(-1), c1(-1), c2(-1), Full(true) { }
} ;

//typedef struct _Mat
//{
//	double *data;
//	//int stPos;
//	int row;
//	int col;
//	int len;
//} Mat;

class Mat
{
public:
	Mat( int m, int n, double *d = NULL)
	{
		row = m;
		col = n;
		len = m*n;

		data = (double *)malloc((size_t)len*sizeof(double));

		if (d)
			memcpy( data, d, (size_t)len*sizeof(double));
		else
			memset( data, 0.0, (size_t)len*sizeof(double));
	};
	Mat( int m, int n, double identity)
	{
		row = m;
		col = n;
		len = m*n;

		data = (double *)malloc((size_t)len*sizeof(double));

		for ( int i = 0 ; i < m ; i++)
		{
			data[i*col+i] = identity;
		}
	};
	Mat(void)
	{
		row = -1;
		col = -1;
		len = 0;

		data = NULL;
	};
	~Mat(void)
	{
		free(data);
	};

	int row, col, len;
	double *data;
};

//void Mat_Ax_by (double *A, long Arow, long Acol, double *x, double *y, double beta, char TransA);

void		MatInitialize( int, int, Mat &A, double *raw = NULL);// Allocate matrix
void     MatRealloc( Mat &A, int m, int n);
void		MatCopy( Mat &A, Mat &B);// Copy matrix B = A

void		MatAdd( Mat &A, Mat &B, Mat &C);// C = A+B
void     MatAdd( Mat &A, MN &Amn, Mat &B, MN &Bmn, Mat &C);

void		MatAddEq( Mat &A, Mat &B);// A += B

void		MatMinus( Mat &A, Mat &B, Mat &C);// C = A-B
void     MatMinus( Mat &A, MN &Amn, Mat &B, MN &Bmn, Mat &C);

void		MatMinusEq( Mat &A, Mat &B);// A -= B

void		MatAB( Mat &A, Mat &B, Mat &C, int dot = 0);// C = A*B
void     MatAB( Mat &A, MN &Amn, Mat &B, MN &Bmn, Mat &C);

void		MatABrow( double *A, int Arow, int Acol, double *B, int Brow, int Bcol, double *C);
void		MatABcol( double *A, int Arow, int Acol, double *B, int Brow, int Bcol, double *C);

void		MatAs( Mat &A, double B, Mat &C);// C = A*scalar
void     MatAs( Mat &A, MN &Amn, double B, Mat &C);

void		MatAsEq( Mat &A, double B);// A *= scalar
void     MatAsEq( Mat &A, MN &Amn, double B);

void		MatOsEq( Mat &A, double B);// A /= scalar
void     MatOsEq( Mat &A, MN &Amn, double B);

void     MatSum( Mat &A, Mat &B, DIR V);

void		MatT( Mat &A, Mat &B);// B = A'
void		MatTcol( double *A, int Arow, int Acol, double *B);
void		MatSubM( Mat &A, MN &mn, Mat &B);// B = A( r1:r2, c1:c2)
void		MatReplaceRow( double *A, double *B, int len);
void		MatReplace( Mat &A, MN &mn, Mat &B);// A( r1:r2, c1:c2) = B
void     MatReplace( Mat &A, MN &mn, double &B);
void		MatNMat( Mat &A, Mat &B, DIR V, Mat &C);// C = [A;B] or C = [A,B]
void		MatMinMax( Mat &A, CompareState state, VIX &V);// [ Val, IX] = min/max(A)
void		MatInv( Mat &A, Mat &B);// B = inv(A)
void		MatB_Ax( Mat &A, Mat &B, Mat &X);// minimize 2-norm (|b-A*x|)
double MatGet( Mat &A, int r, int c);// C = A( r, c)
void		MatShow( Mat &A, char text[] = NULL);// Show matrix
void		MatShowCol( double *A, int Arow, int Acol, char text[] = NULL);
void     MatShow( Mat &A, Mat &B, Mat &C, char text[] = NULL);

inline double Cross2x2( double a, double b, double c, double d)
{
	return a * d - b * c;
}

inline void Add_AB(double* A, double* B, double* result, int Len) // 矩陣相加
{
	//// 原始碼
	//for (int i = 0; i < Len ;  i++)
	//result[i] = A[i]+B[i];


	__asm{

		mov eax, A
			mov ebx, B
			mov edx, result

			mov ecx, Len // 是多少 就會進去幾次回圈

	// for loop
	mat_add: // ecx 會自動減一直到0 跳出回圈

		fld QWORD PTR [eax] // load A[i] to FPU stack 0 ( st(0) )
		fadd QWORD PTR [ebx] // add B[i] to st(0)

		fstp QWORD PTR [edx] // pop the value 

		add eax,8
			add ebx,8
			add edx,8

			//loop mat_add

			dec ecx
			cmp ecx, 0
			jnz mat_add

			// for loop

	}
}

inline void Miu_AB(double* A, double* B, double* result, int Len) // 矩陣相減
{
	// 原始碼
	/*for (int i = 0; i < Len ;  i++)
		result[i] = A[i]-B[i];*/


	__asm{

		mov eax, A
		mov ebx, B
		mov edx, result

		mov ecx, Len // 是多少 就會進去幾次回圈

	// for loop
	mat_miu: // ecx 會自動減一直到0 跳出回圈

		fld QWORD PTR [eax] // load A[i] to FPU stack 0 ( st(0) )
		fsub QWORD PTR [ebx] // add B[i] to st(0)
		
		fstp QWORD PTR [edx] // pop the value 

		add eax,8
		add ebx,8
		add edx,8

		dec ecx
		cmp ecx, 0
		jnz mat_miu
		//loop mat_add
	 //for loop

	}
}