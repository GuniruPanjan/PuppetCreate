#pragma once
#include <List>
#include "LibVec3.h"

namespace MyLibrary
{
	//デバッグ用の描画情報をまとめ、表示するためのクラス
	class DebugDraw
	{
	public:
		//描画情報の削除
		static void Clear();
		//描画
		static void Draw();

		//カプセルの描画リストに追加する
		static void AddDrawCapsule(const LibVec3& center, const LibVec3& vec, const float& radius, const float& len, const unsigned int& color);

		//攻撃用のカプセル描画リストに追加する
		static void AddDrawAttackCapsule(const LibVec3& pos1, const LibVec3& pos2, const float& radius, const unsigned int& color);

		//球体の描画リストに追加する
		static void AddDrawSphere(const LibVec3& center, const float& radius, const unsigned int& color);

		//矩形の描画リストに追加する
		static void AddDrawRect(const LibVec3& center, const LibVec3::Size& size, const unsigned int& color);

	private:
		//カプセルの描画情報
		struct CapsuleInfo
		{
			LibVec3 center;      //中心座標
			LibVec3 vec;         //ベクター
			float len;           //長さ
			float radius;        //半径
			unsigned int color;  //色
		};

		//攻撃用の描画情報
		struct AttackCapsuleInfo
		{
			LibVec3 pos1;        //座標1
			LibVec3 pos2;        //座標2
			float radius;        //半径
			unsigned int color;  //色
		};

		//球体の描画情報
		struct SphereInfo
		{
			LibVec3 center;      //中心座標
			float radius;        //半径
			unsigned int color;  //色
		};

		//矩形の描画情報
		struct RectInfo
		{
			LibVec3 center;      //中心座標
			LibVec3::Size size;        //サイズ
			unsigned int color;  //色
		};

		//カプセルの描画情報リスト
		static std::list<CapsuleInfo> m_capsuleInfo;
		//攻撃用のカプセルの描画情報リスト
		static std::list<AttackCapsuleInfo> m_attackCapsuleInfo;
		//球体の描画情報リスト
		static std::list<SphereInfo> m_sphereInfo;
		//矩形の描画情報リスト
		static std::list<RectInfo> m_rectInfo;
	};
}



