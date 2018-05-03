#pragma once

namespace MyDirectX
{
	struct Matrix
	{
	private:
		//�� �����͸� ������ Ŭ���� ������
		struct Row
		{
			//�� ������ ����
			std::vector<float> vecCol;

			//���ڸ� �޾Ƽ� ���Ϳ� ����
			float& operator[](int index)
			{
				return vecCol[index];
			}

			//���� ������ ũ�⸸ŭ ���͸� ��������
			void Resize(int nDimension)
			{
				vecCol.resize(nDimension);
			}

		};
	private:
		vector<Row> vecRow;

	public:
		Matrix() {}
		Matrix(int nDimension);

		Row& operator[](int nIndex);
		bool operator==(Matrix& mat);
		bool operator!=(Matrix& mat);

		Matrix operator+(Matrix& mat);
		Matrix operator-(Matrix& mat);
		Matrix operator*(Matrix& mat);
		Matrix operator*(float f);
		Matrix Transpose();

		static Matrix Identity(int nDimension);


		//�����
		Matrix Inverse(float& fDet);
		float   Determinent();
		float   Cofactor(int nRow, int nCol);
		float   Minor(int nRow, int nCol);
		Matrix Adjoint();

		D3DXMATRIX ToDXMatrix();

		static Matrix Translation(struct Vector2& v);
		static Matrix View(struct Vector2& eye, struct Vector2& look, struct Vector2& up);
		static Matrix Ortho(float left, float right, float bottom, float top, float zn, float zf);
		static Matrix Viewport(float x, float y, float w, float h, float minZ, float maxZ);
		static Matrix Rotate(float fAngle);

	private:
		void Resize(int nDimension);
		int Dimension();
	};
}
