#ifndef _MATRIX_
#define _MATRIX_
#pragma once

#include "Vec3.h"
#include "Vec4.h"

//[0] [4] [8]  [12]     [0]
//[1] [5] [9]  [13]     [1]
//[2] [6] [10] [14]     [2]
//[3] [7] [11] [15]     [3]

namespace sm
{
	class Matrix
	{
	public:
		static Matrix Identity;
		static Matrix CreateIdentityMatrix();

		float a[16];
		
		Matrix GetTransposed() const
		{
			Matrix matrix;
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					matrix.a[y * 4 + x] = a[x * 4 + y];
				}
			}
			
			return matrix;
		}
				
		Matrix GetInversed() const
		{
			Matrix src = this ->GetTransposed();
			Matrix dst;
			float tmp[12];
			float det;
			
			tmp[0] = src.a[10] * src.a[15];
			tmp[1] = src.a[11] * src.a[14];
			tmp[2] = src.a[9] * src.a[15];
			tmp[3] = src.a[11] * src.a[13];
			tmp[4] = src.a[9] * src.a[14];
			tmp[5] = src.a[10] * src.a[13];
			tmp[6] = src.a[8] * src.a[15];
			tmp[7] = src.a[11] * src.a[12];
			tmp[8] = src.a[8] * src.a[14];
			tmp[9] = src.a[10] * src.a[12];
			tmp[10] = src.a[8] * src.a[13];
			tmp[11] = src.a[9] * src.a[12];
			
			dst.a[0] = tmp[0]*src.a[5] + tmp[3]*src.a[6] + tmp[4]*src.a[7];
			dst.a[0] -= tmp[1]*src.a[5] + tmp[2]*src.a[6] + tmp[5]*src.a[7];
			dst.a[1] = tmp[1]*src.a[4] + tmp[6]*src.a[6] + tmp[9]*src.a[7];
			dst.a[1] -= tmp[0]*src.a[4] + tmp[7]*src.a[6] + tmp[8]*src.a[7];
			dst.a[2] = tmp[2]*src.a[4] + tmp[7]*src.a[5] + tmp[10]*src.a[7];
			dst.a[2] -= tmp[3]*src.a[4] + tmp[6]*src.a[5] + tmp[11]*src.a[7];
			dst.a[3] = tmp[5]*src.a[4] + tmp[8]*src.a[5] + tmp[11]*src.a[6];
			dst.a[3] -= tmp[4]*src.a[4] + tmp[9]*src.a[5] + tmp[10]*src.a[6];
			dst.a[4] = tmp[1]*src.a[1] + tmp[2]*src.a[2] + tmp[5]*src.a[3];
			dst.a[4] -= tmp[0]*src.a[1] + tmp[3]*src.a[2] + tmp[4]*src.a[3];
			dst.a[5] = tmp[0]*src.a[0] + tmp[7]*src.a[2] + tmp[8]*src.a[3];
			dst.a[5] -= tmp[1]*src.a[0] + tmp[6]*src.a[2] + tmp[9]*src.a[3];
			dst.a[6] = tmp[3]*src.a[0] + tmp[6]*src.a[1] + tmp[11]*src.a[3];
			dst.a[6] -= tmp[2]*src.a[0] + tmp[7]*src.a[1] + tmp[10]*src.a[3];
			dst.a[7] = tmp[4]*src.a[0] + tmp[9]*src.a[1] + tmp[10]*src.a[2];
			dst.a[7] -= tmp[5]*src.a[0] + tmp[8]*src.a[1] + tmp[11]*src.a[2];
			
			tmp[0] = src.a[2]*src.a[7];
			tmp[1] = src.a[3]*src.a[6];
			tmp[2] = src.a[1]*src.a[7];
			tmp[3] = src.a[3]*src.a[5];
			tmp[4] = src.a[1]*src.a[6];
			tmp[5] = src.a[2]*src.a[5];
			tmp[6] = src.a[0]*src.a[7];
			tmp[7] = src.a[3]*src.a[4];
			tmp[8] = src.a[0]*src.a[6];
			tmp[9] = src.a[2]*src.a[4];
			tmp[10] = src.a[0]*src.a[5];
			tmp[11] = src.a[1]*src.a[4];
			
			dst.a[8] = tmp[0]*src.a[13] + tmp[3]*src.a[14] + tmp[4]*src.a[15];
			dst.a[8] -= tmp[1]*src.a[13] + tmp[2]*src.a[14] + tmp[5]*src.a[15];
			dst.a[9] = tmp[1]*src.a[12] + tmp[6]*src.a[14] + tmp[9]*src.a[15];
			dst.a[9] -= tmp[0]*src.a[12] + tmp[7]*src.a[14] + tmp[8]*src.a[15];
			dst.a[10] = tmp[2]*src.a[12] + tmp[7]*src.a[13] + tmp[10]*src.a[15];
			dst.a[10]-= tmp[3]*src.a[12] + tmp[6]*src.a[13] + tmp[11]*src.a[15];
			dst.a[11] = tmp[5]*src.a[12] + tmp[8]*src.a[13] + tmp[11]*src.a[14];
			dst.a[11]-= tmp[4]*src.a[12] + tmp[9]*src.a[13] + tmp[10]*src.a[14];
			dst.a[12] = tmp[2]*src.a[10] + tmp[5]*src.a[11] + tmp[1]*src.a[9];
			dst.a[12]-= tmp[4]*src.a[11] + tmp[0]*src.a[9] + tmp[3]*src.a[10];
			dst.a[13] = tmp[8]*src.a[11] + tmp[0]*src.a[8] + tmp[7]*src.a[10];
			dst.a[13]-= tmp[6]*src.a[10] + tmp[9]*src.a[11] + tmp[1]*src.a[8];
			dst.a[14] = tmp[6]*src.a[9] + tmp[11]*src.a[11] + tmp[3]*src.a[8];
			dst.a[14]-= tmp[10]*src.a[11] + tmp[2]*src.a[8] + tmp[7]*src.a[9];
			dst.a[15] = tmp[10]*src.a[10] + tmp[4]*src.a[8] + tmp[9]*src.a[9];
			dst.a[15]-= tmp[8]*src.a[9] + tmp[11]*src.a[10] + tmp[5]*src.a[8];
			
			det=src.a[0]*dst.a[0]+src.a[1]*dst.a[1]+src.a[2]*dst.a[2]+src.a[3]*dst.a[3];
			
			det = 1.0f / det;
			for (int j = 0; j < 16; j++)
				dst.a[j] *= det;
			
			return dst;
		}
		
		static Matrix TranslateMatrix(const Vec3 &v)
		{
			return TranslateMatrix(v.x, v.y, v.z);
		}

		static Matrix TranslateMatrix(float x, float y, float z)
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
			mat.a[12] = x;
			mat.a[13] = y;
			mat.a[14] = z;
			mat.a[15] = 1.0f;

			return mat;
		}

		static Matrix ScaleMatrix(const Vec3 &v)
		{
			return ScaleMatrix(v.x, v.y, v.z);
		}

		static Matrix ScaleMatrix(float x, float y, float z)
		{
			Matrix mat;

			mat.a[0] = x;
			mat.a[1] = 0.0f;
			mat.a[2] = 0.0f;
			mat.a[3] = 0.0f;
			mat.a[4] = 0.0f;
			mat.a[5] = y;
			mat.a[6] = 0.0f;
			mat.a[7] = 0.0f;
			mat.a[8] = 0.0f;
			mat.a[9] = 0.0f;
			mat.a[10] = z;
			mat.a[11] = 0.0f;
			mat.a[12] = 0.0f;
			mat.a[13] = 0.0f;
			mat.a[14] = 0.0f;
			mat.a[15] = 1.0f;

			return mat;
		}

		static Matrix ScaleMatrix(float value)
		{
			Matrix mat;

			mat.a[0] = value;
			mat.a[1] = 0.0f;
			mat.a[2] = 0.0f;
			mat.a[3] = 0.0f;
			mat.a[4] = 0.0f;
			mat.a[5] = value;
			mat.a[6] = 0.0f;
			mat.a[7] = 0.0f;
			mat.a[8] = 0.0f;
			mat.a[9] = 0.0f;
			mat.a[10] = value;
			mat.a[11] = 0.0f;
			mat.a[12] = 0.0f;
			mat.a[13] = 0.0f;
			mat.a[14] = 0.0f;
			mat.a[15] = 1.0f;

			return mat;
		}

		static Matrix RotateAxisMatrix(float angle, const Vec3 &axis)
		{
			return RotateAxisMatrix(angle, axis.x, axis.y, axis.z);
		}

		static Matrix RotateAxisMatrix(float angle, float x, float y, float z)
		{
			Matrix matrix;

			float c = cosf(angle);
			float s = sinf(angle);

			float oneMinusC = (1 - c);

			matrix.a[0] = x * x * oneMinusC + c;
			matrix.a[1] = y * x * oneMinusC + z * s;
			matrix.a[2] = x * z * oneMinusC - y * s;
			matrix.a[3] = 0.0f;
			matrix.a[4] = x * y * oneMinusC - z * s;
			matrix.a[5] = y * y * oneMinusC + c;
			matrix.a[6] = y * z * oneMinusC + x * s;
			matrix.a[7] = 0.0f;
			matrix.a[8] = x * z * oneMinusC + y * s;
			matrix.a[9] = y * z * oneMinusC - x * s;
			matrix.a[10] = z * z * oneMinusC + c;
			matrix.a[11] = 0.0f;
			matrix.a[12] = 0.0f;
			matrix.a[13] = 0.0f;
			matrix.a[14] = 0.0f;
			matrix.a[15] = 1.0f;

			return matrix;
		}

		static Matrix PerspectiveMatrix(float fovH, float aspect, float znear, float zfar)
		{
			Matrix matrix = Identity;

			float fovV = 2.0f * atanf(tanf(fovH / 2.0f) / aspect);

			float f = 1.0f / tanf(fovV / 2.0f); // equal to cotangens((fov * aspect) / 2.0f)
			float nearMinusFar = znear - zfar;

			matrix.a[0] = f / aspect;
			matrix.a[5] = f;
			matrix.a[10] = (zfar + znear) / nearMinusFar;
			matrix.a[11] = -1;
			matrix.a[14] = (2.0f * znear * zfar) / nearMinusFar;
			matrix.a[15] = 0.0f;

			return matrix;
		}

		static Matrix Ortho2DMatrix(float left, float right, float bottom, float top)
		{
			float _near = -100.0f;
			float _far = 100.0f;

			Matrix matrix = Identity;

			matrix.a[0] = 2.0f / (right - left);
			matrix.a[5] = 2.0f / (top - bottom);
			matrix.a[10] = -2.0f / (_far - _near);

			matrix.a[12] = -(right + left) / (right - left);
			matrix.a[13] = -(top + bottom) / (top - bottom);
			matrix.a[14] = -(_far + _near) / (_far - _near);

			return matrix;
		}

		bool operator == (const Matrix &matrix) const
		{
			for (int i = 0; i < 16; i++)
				if (a[i] != matrix.a[i])
					return false;

			return true;
		}

		Matrix operator * (const Matrix &right) const
		{	
			Matrix left;

			left.a[0] = a[0] * right.a[0] + a[4] * right.a[1] + a[8] * right.a[2] + a[12] * right.a[3];
			left.a[1] = a[1] * right.a[0] + a[5] * right.a[1] + a[9] * right.a[2] + a[13] * right.a[3];
			left.a[2] = a[2] * right.a[0] + a[6] * right.a[1] + a[10] * right.a[2] + a[14] * right.a[3];
			left.a[3] = a[3] * right.a[0] + a[7] * right.a[1] + a[11] * right.a[2] + a[15] * right.a[3];

			left.a[4] = a[0] * right.a[4] + a[4] * right.a[5] + a[8] * right.a[6] + a[12] * right.a[7];
			left.a[5] = a[1] * right.a[4] + a[5] * right.a[5] + a[9] * right.a[6] + a[13] * right.a[7];
			left.a[6] = a[2] * right.a[4] + a[6] * right.a[5] + a[10] * right.a[6] + a[14] * right.a[7];
			left.a[7] = a[3] * right.a[4] + a[7] * right.a[5] + a[11] * right.a[6] + a[15] * right.a[7];

			left.a[8] = a[0] * right.a[8] + a[4] * right.a[9] + a[8] * right.a[10] + a[12] * right.a[11];
			left.a[9] = a[1] * right.a[8] + a[5] * right.a[9] + a[9] * right.a[10] + a[13] * right.a[11];
			left.a[10] = a[2] * right.a[8] + a[6] * right.a[9] + a[10] * right.a[10] + a[14] * right.a[11];
			left.a[11] = a[3] * right.a[8] + a[7] * right.a[9] + a[11] * right.a[10] + a[15] * right.a[11];

			left.a[12] = a[0] * right.a[12] + a[4] * right.a[13] + a[8] * right.a[14] + a[12] * right.a[15];
			left.a[13] = a[1] * right.a[12] + a[5] * right.a[13] + a[9] * right.a[14] + a[13] * right.a[15];
			left.a[14] = a[2] * right.a[12] + a[6] * right.a[13] + a[10] * right.a[14] + a[14] * right.a[15];
			left.a[15] = a[3] * right.a[12] + a[7] * right.a[13] + a[11] * right.a[14] + a[15] * right.a[15];

			return left;			
		}

		Vec3 operator * (const Vec3 &right) const
		{
			return Vec3(
				a[0] * right.x + a[4] * right.y + a[8] * right.z + a[12],
				a[1] * right.x + a[5] * right.y + a[9] * right.z + a[13],
				a[2] * right.x + a[6] * right.y + a[10] * right.z + a[14]
				);
		}

		Vec4 operator * (const Vec4 &right) const
		{
			return Vec4(
				a[0] * right.x + a[4] * right.y + a[8] * right.z + a[12] * right.w,
				a[1] * right.x + a[5] * right.y + a[9] * right.z + a[13] * right.w,
				a[2] * right.x + a[6] * right.y + a[10] * right.z + a[14] * right.w,
				a[3] * right.x + a[7] * right.y + a[11] * right.z + a[15] * right.w
				);
		}

		Matrix &operator *= (const Matrix &right)
		{
			(*this) = (*this) * right;
			return *this;
		}

		void operator =(const float *a)
		{
			for (int i = 0; i < 16; i++)
				 this ->a[i] = a[i];
		}

		void SetValue(int row, int column, float value)
		{
			a[(column * 4) + row] = value;
		}

		static Matrix CreateLookAt(const sm::Vec3 &direction, const sm::Vec3 &up)
		{
			sm::Matrix rot = sm::Matrix::Identity;

			sm::Vec3 right = (direction * up).GetNormalized();
			sm::Vec3 yAxis = right * direction;

			rot.a[0] = right.x;
			rot.a[1] = right.y;
			rot.a[2] = right.z;

			rot.a[4] = up.x;
			rot.a[5] = up.y;
			rot.a[6] = up.z;

			rot.a[8] = direction.x;
			rot.a[9] = direction.y;
			rot.a[10] = direction.z;

			return rot;
		}
	};
}

#endif