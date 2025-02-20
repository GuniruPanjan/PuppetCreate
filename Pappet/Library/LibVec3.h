#pragma once
#include "DxLib.h"
#include <cassert>
#include <cmath>

namespace MyLibrary
{
	//3次元ベクトルを扱うクラス
	class LibVec3
	{
	public:
		float x, y, z;

		//引数なしコンストラクタ
		LibVec3() :
			x(0.0f),
			y(0.0f),
			z(0.0f)
		{

		}
		//引数ありコンストラクタ
		LibVec3(float X, float Y, float Z) :
			x(X),
			y(Y),
			z(Z)
		{

		}

		//単項演算子
		LibVec3 Reverse()
		{
			return LibVec3(-x, -y, -z);
		}
		//足し算
		LibVec3 operator+(const LibVec3& vec) const
		{
			return LibVec3(x + vec.x, y + vec.y, z + vec.z);
		}
		//Vec3 += Vec3
		void operator+=(const LibVec3& vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
		}
		//引き算
		LibVec3 operator-(const LibVec3& vec) const
		{
			return LibVec3(x - vec.x, y - vec.y, z - vec.z);
		}
		//Vec3 -= Vec3
		void operator-=(const LibVec3& vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
		}
		//掛け算
		LibVec3 operator*(float scale) const
		{
			return LibVec3(x * scale, y * scale, z * scale);
		}
		//Vec3 *= scale
		void operator*=(float scale)
		{
			x *= scale;
			y *= scale;
			z *= scale;
		}
		//割り算
		LibVec3 operator/(float scale) const
		{
			return LibVec3(x / scale, y / scale, z / scale);
		}
		//Vec3 /= scale
		void operator/=(float scale)
		{
			x /= scale;
			y /= scale;
			z /= scale;
		}
		//Vec3 == Vec3だった場合
		bool operator==(const LibVec3& vec) const
		{
			return x == vec.x && y == vec.y && z == vec.z;
		}
		//Vec3 != Vec3だった場合
		bool operator!=(const LibVec3& vec) const
		{
			return !(*this == vec);
		}

		//Dxlibの関数を使う際に、VECTOR型に変換する関数
		VECTOR GetVector() const { return VGet(x, y, z); }

		//長さを取得
		float Length() const
		{
			return sqrtf(SqLength());
		}

		//長さの2乗を取得
		float SqLength() const
		{
			return x * x + y * y + z * z;
		}

		//自身の正規化
		LibVec3 Normalize()
		{
			LibVec3 ret;
			float len = Length();
			//長さ0の場合
			if (len == 0.0f)
			{
				//assert(false);
				x = 0.0f;
				y = 0.0f;
				z = 0.0f;
				return ret;
			}

			ret.x = this->x / len;
			ret.y = this->y / len;
			ret.z = this->z / len;

			return ret;
		}

		//正規化した結果を取得
		LibVec3 GetNormalized() const
		{
			float len = Length();
			//長さ0の場合
			if (len == 0.0f)
			{
				return LibVec3();
			}

			return LibVec3(x / len, y / len, z / len);
		}

		/// <summary>
		/// サイズ構造体
		/// </summary>
		struct Size
		{
			//幅
			float width = 0.0f;
			//高さ
			float height = 0.0f;
			//奥行
			float depth = 0.0f;

			Size() :
				width(0.0f),
				height(0.0f),
				depth(0.0f)
			{
			}
			Size(float inwidth, float inheight, float indepth) :
				width(inwidth),
				height(inheight),
				depth(indepth)
			{

			}
			//足し算
			Size operator+(const Size& size) const
			{
				return Size(width + size.width, height + size.height, depth + size.depth);
			}
			//掛け算
			Size operator*(float scale) const
			{
				return Size(width * scale, height * scale, depth * scale);
			}
		};


		/// <summary>
        /// 線形補間
        /// </summary>
        /// <param name="start">スタート</param>
        /// <param name="end">終了</param>
        /// <param name="t">割合</param>
        /// <returns>補間後</returns>
		LibVec3 Lerp(const LibVec3& start, const LibVec3& end, float t)
		{
			float x = start.x + t * (end.x - start.x);
			float y = start.y + t * (end.y - start.y);
			float z = start.z + t * (end.z - start.z);

			return LibVec3(x, y, z);
		}

		/// <summary>
		/// 内積
		/// </summary>
		/// <param name="vec1">ベクトル1</param>
		/// <param name="vec2">ベクトル2</param>
		/// <returns></returns>
		float Dot(const LibVec3& vec1, const LibVec3& vec2)
		{
			return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
		}

		/// <summary>
		/// 外積
		/// </summary>
		/// <param name="vec1">ベクトル1</param>
		/// <param name="vec2">ベクトル2</param>
		/// <returns></returns>
		LibVec3 Cross(const LibVec3& vec1, const LibVec3& vec2)
		{
			LibVec3 vec;

			vec.x = vec1.y * vec2.z - vec1.z * vec2.y;
			vec.y = vec1.z * vec2.x - vec1.x * vec2.z;
			vec.z = vec1.x * vec2.y - vec1.y * vec2.x;

			return vec;
		}

		//VECTOR型に変換
		VECTOR ConversionToVECTOR()
		{
			VECTOR ret = VGet(0.0f, 0.0f, 0.0f);
			ret.x = this->x;
			ret.y = this->y;
			ret.z = this->z;
			return ret;
		}

		using Pos3 = LibVec3;
	};
}


