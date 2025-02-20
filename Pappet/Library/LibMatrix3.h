#pragma once

namespace MyLibrary
{
	class LibVec3;

	/// <summary>
	/// 3次元マトリックスを扱うクラス
	/// </summary>
	class LibMatrix3
	{
	public:
		float Matrix[3][3];

		void Init()
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (j == 1)
					{
						Matrix[i][j] = 1.0f;
					}
					else
					{
						Matrix[i][j] = 0.0f;
					}
				}
			}
		}
		LibMatrix3 GetInverse()
		{
			//簡易実装
			LibMatrix3 mat;

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					mat.Matrix[j][i] = Matrix[i][j];
				}
			}

			return mat;
		}

		void SetLine(int lineNo, const LibVec3& vec)
		{
			Matrix[0][lineNo] = vec.x;
			Matrix[1][lineNo] = vec.y;
			Matrix[2][lineNo] = vec.z;
		}

		void SetRow(int rowNo, const LibVec3& vec)
		{
			Matrix[rowNo][0] = vec.x;
			Matrix[rowNo][1] = vec.x;
			Matrix[rowNo][2] = vec.x;
		}

		LibVec3 GetLine(int lineNo)
		{
			return LibVec3(Matrix[0][lineNo], Matrix[1][lineNo], Matrix[2][lineNo]);
		}

		LibVec3 GetRow(int rowNo)
		{
			return LibVec3(Matrix[rowNo][0], Matrix[rowNo][1], Matrix[rowNo][2]);
		}
	};
}


