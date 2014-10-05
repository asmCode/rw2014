#include "Matrix.h"

namespace sm
{
	Matrix Matrix::Identity = CreateIdentityMatrix();

	Matrix Matrix::CreateIdentityMatrix()
	{
		Matrix mat;
		mat.a[0] = 1.0f;
		mat.a[1] = 0.0f;
		mat.a[2] = 0.0f;
		mat.a[3] = 0.0f;
		mat.a[4] = 0.0f;
		mat.a[5] = 1.0f;
		mat.a[6] = 0.0f;
		mat.a[7] = 0.0f;
		mat.a[8] = 0.0f;
		mat.a[9] = 0.0f;
		mat.a[10] = 1.0f;
		mat.a[11] = 0.0f;
		mat.a[12] = 0.0f;
		mat.a[13] = 0.0f;
		mat.a[14] = 0.0f;
		mat.a[15] = 1.0f;

		return mat;
	}
}
