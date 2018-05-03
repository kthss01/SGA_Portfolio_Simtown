#include "stdafx.h"
#include "Matrix.h"
namespace MyDirectX
{

	Matrix::Matrix(int nDimension)
	{
		Resize(nDimension);
	}

	Matrix::Row& Matrix::operator[](int nIndex)
	{
		return vecRow[nIndex];
	}

	void Matrix::Resize(int nDimension)
	{
		vecRow.resize(nDimension);
		for (int i = 0; i < nDimension; i++)
		{
			vecRow[i].Resize(nDimension);
		}
	}

	int Matrix::Dimension()
	{
		return vecRow.size();
	}

	bool Matrix::operator==(Matrix& mat)
	{
		for (int i = 0; i < Dimension(); i++)
		{
			for (int j = 0; j < Dimension(); j++)
			{
				if (fabs(mat[i][j] - (*this)[i][j]) > EPSILON)
					return false;
			}
		}

		return true;
	}

	bool Matrix::operator!=(Matrix& mat)
	{
		return !((*this) == mat);
	}

	Matrix Matrix::operator+(Matrix& mat)
	{
		Matrix ret(Dimension());
		for (int i = 0; i < Dimension(); i++)
		{
			for (int j = 0; j < Dimension(); j++)
			{
				ret[i][j] = (*this)[i][j] + mat[i][j];
			}
		}

		return ret;
	}

	Matrix Matrix::operator-(Matrix& mat)
	{
		Matrix ret(Dimension());
		for (int i = 0; i < Dimension(); i++)
		{
			for (int j = 0; j < Dimension(); j++)
			{
				ret[i][j] = (*this)[i][j] - mat[i][j];
			}
		}

		return ret;
	}

	Matrix Matrix::operator*(Matrix& mat)
	{
		Matrix ret(Dimension());

		for (int i = 0; i < Dimension(); ++i)
		{
			for (int j = 0; j < Dimension(); ++j)
			{
				ret[i][j] = 0.f;
				for (int k = 0; k < Dimension(); ++k)
				{
					ret[i][j] += (*this)[i][k] * mat[k][j];
				}
			}
		}

		return ret;
	}

	Matrix Matrix::operator*(float f)
	{
		Matrix ret(Dimension());
		for (int i = 0; i < Dimension(); i++)
		{
			for (int j = 0; j < Dimension(); j++)
			{
				ret[i][j] = (*this)[i][j] * f;
			}
		}

		return ret;
	}

	Matrix Matrix::Transpose()
	{
		Matrix ret(Dimension());
		for (int i = 0; i < Dimension(); i++)
		{
			for (int j = 0; j < Dimension(); j++)
			{
				ret[i][j] = (*this)[j][i];
			}
		}

		return ret;
	}

	Matrix Matrix::Identity(int nDimension)
	{
		Matrix ret(nDimension);
		for (int i = 0; i < nDimension; i++)
		{
			for (int j = 0; j < nDimension; j++)
			{
				ret[i][j] = (i == j) ? 1 : 0;
			}
		}

		return ret;
	}


	//������ ���ؼ� Determinent�� Adjoint�� �ʿ���
	//�и� 0�� �� �����Ƿ�  Determinent�� 0�̸� Identity�� �����ϵ��� ��
	Matrix Matrix::Inverse(float& fDet)
	{
		fDet = Determinent();

		//if (fabs(fDet) < 0.0001f)
		//	return Matrix::Identity(Dimension());

		return Adjoint() * (1 / fDet);
	}

	//2�� ���簢 ����� ��� Determinent�� ad-bc
	//���� �״�� ����
	//������ ���ؼ� Cofactor�� �ʿ���
	float   Matrix::Determinent()
	{
		if (Dimension() == 2)
		{
			return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
		}
		float fDet = 0.0f;

		for (int i = 0; i < Dimension(); ++i)
		{
			fDet += ((*this)[i][0] * Cofactor(i, 0));
		}

		return fDet;
	}


	//���� �״�� ����
	//��� ���� �տ� ���� ��ȣ�� �ٲ�
	//������ ���ؼ� Minor�� �ʿ���
	//Minor�� i�� j�� ������ �� ����� Determinent
	float   Matrix::Cofactor(int nRow, int nCol)
	{
		float fConst = 1.f;

		if ((nRow + nCol) % 2 == 1)
		{
			fConst = -1.f;
		}

		return fConst * Minor(nRow, nCol);
	}

	//i�� j�� ������ �� ����� Determinent
	//������ ���ؼ� Determinent�� �ʿ���
	float  Matrix::Minor(int nRow, int nCol)
	{
		Matrix mat(Dimension() - 1);
		int nMinorRow = 0;
		int nMinorCol = 0;
		for (int i = 0; i < Dimension(); ++i)
		{
			if (i == nRow)
				continue;
			nMinorCol = 0;
			for (int j = 0; j < Dimension(); ++j)
			{
				if (j == nCol)
					continue;
				mat[nMinorRow][nMinorCol] = (*this)[i][j];
				++nMinorCol;
			}
			++nMinorRow;
		}
		return mat.Determinent();

	}

	//cofactor�� ��ġ���
	Matrix Matrix::Adjoint()
	{
		Matrix ret(Dimension());

		for (int i = 0; i < Dimension(); ++i)
		{
			for (int j = 0; j < Dimension(); ++j)
			{
				ret[i][j] = Cofactor(i, j);
			}
		}

		return ret.Transpose();
	}

	D3DXMATRIX Matrix::ToDXMatrix()
	{
		return D3DXMATRIX(
			(*this)[0][0], (*this)[0][1], (*this)[0][2], (*this)[0][3],
			(*this)[1][0], (*this)[1][1], (*this)[1][2], (*this)[1][3],
			(*this)[2][0], (*this)[2][1], (*this)[2][2], (*this)[2][3],
			(*this)[3][0], (*this)[3][1], (*this)[3][2], (*this)[3][3]
		);
	}


	Matrix Matrix::Translation(Vector2& v)
	{
		Matrix mat = Matrix::Identity(4);
		mat[3][0] = v.x;
		mat[3][1] = v.y;
		mat[3][2] = v.z;
		return mat;
	}


	Matrix Matrix::View(Vector2& vEye, Vector2& vLookAt, Vector2& vUp)
	{
		Vector2 l = (vLookAt - vEye).Normalize();
		Vector2 r = Vector2::Cross(vUp, l).Normalize();
		Vector2 u = Vector2::Cross(l, r).Normalize();
		Matrix mat = Matrix::Identity(4);
		mat[0][0] = r.x; mat[0][1] = u.x; mat[0][2] = l.x;
		mat[1][0] = r.y; mat[1][1] = u.y; mat[1][2] = l.y;
		mat[2][0] = r.z; mat[2][1] = u.z; mat[2][2] = l.z;
		mat[3][0] = -Vector2::Dot(r, vEye);
		mat[3][1] = -Vector2::Dot(u, vEye);
		mat[3][2] = -Vector2::Dot(l, vEye);
		//���̵�ƼƼ�� �ʱ�ȭ ������ [3][3]�� �� �ص� ��
		return mat;
	}

	Matrix Matrix::Ortho(float left, float right, float bottom, float top, float zn, float zf)
	{
		// ���� ��� �������� �Ϸ��� �ߴµ� 2D������ ������
		// ���� ������� �༮ left top �������� �Ǿ��ִµ�
		// ���� �ϴ����� �ٲ��ٲ� ���� �ö󰥶� +�� �Ҳ���

		Matrix mat = Matrix::Identity(4);
		mat[0][0] = 2 / (right - left);
		//mat[1][1] = 2 / (bottom - top);
		mat[1][1] = 2 / (top - bottom);
		mat[2][2] = 1 / (zf - zn);

		//mat[3][0] = (left + right) / (left - right);
		////mat[3][1] = (bottom + top) / (bottom - top);
		//mat[3][1] = (top + bottom) / (top - bottom);
		
		mat[3][2] = zn / (zn - zf);
		//���̵�ƼƼ�� �ʱ�ȭ ������ [3][3]�� �� �ص� ��
		return mat;
	}

	Matrix Matrix::Viewport(float x, float y, float w, float h, float minz, float maxz)
	{
		Matrix mat = Matrix::Identity(4);
		mat[0][0] = w / 2.0f;
		mat[1][1] = -h / 2.0f;
		mat[2][2] = maxz - minz;
		mat[3][0] = x + w / 2.0f;
		mat[3][1] = y + h / 2.0f;
		mat[3][2] = minz;
		//���̵�ƼƼ�� �ʱ�ȭ ������ [3][3]�� �� �ص� ��
		return mat;
	}

	Matrix Matrix::Rotate(float fRadian)
	{
		// {  cos sin 0 0
		//   -sin cos 0 0
		//     0   0  1 0
		//     0   0  0 1 }
		Matrix matRet = Matrix::Identity(4);
		matRet[0][0] = cosf(fRadian);
		matRet[0][1] = sinf(fRadian);
		matRet[1][0] = -sinf(fRadian);
		matRet[1][1] = cosf(fRadian);
		return matRet;
	}

}