#pragma once
#include "LibVec3.h"

#include <list>
#include <vector>
#include <unordered_map>

namespace MyLibrary
{
	class Collidable;

	namespace ColInfo
	{
		//最大当たり判定ポリゴン数
		constexpr int kMaxColHitPoly = 3000;
		//壁押し出し処理の最大試行回数
		constexpr int kMaxColHitTry = 20;
		//壁押し出し時にスライドさせる距離
		constexpr float kColHitSlideLength = 1.0f;
	}

	/// <summary>
	/// 物理・衝突判定するオブジェクトを登録し、物理移動・衝突を通知する
	/// </summary>
	class Physics final
	{
	private:
		//当たり判定の種類
		enum class eOnCollideInfoKind
		{
			CollideEnter,
			CollideStay,
			CollideExit,
			TriggerEnter,
			TriggerStay,
			TriggerExit
		};

		//何かと当たったオブジェクトの情報
		struct OnCollideInfoData
		{
			std::shared_ptr<Collidable> mySelf;    //自分自信
			std::shared_ptr<Collidable> person;    //当たった相手
			eOnCollideInfoKind kind;               //種類
		};

		using SendCollideInfo = std::unordered_map<std::shared_ptr<Collidable>, std::list<std::shared_ptr<Collidable>>>;

	public:
		//コンストラクタ
		Physics(int mapStageCollisionHandle);

		//衝突物の登録・登録解除
		void Entry(std::shared_ptr<Collidable> collidable);
		void Exit(std::shared_ptr<Collidable> collidable);

		//更新(登録オブジェクトの物理移動、衝突通知)
		void Update();

		void CheckUpdate();

		void CheckEnemyUpdate();

		bool GetFlag() { return m_isHitWallFlag; }

		VECTOR GetVECTOR() { return m_ret; }

	private:
		//当たり判定チェック
		void CheckCollide();
		//当たり判定チェック
		void CheckCollide1();
		//当たり判定チェック
		void CheckCollide2();
		//当たり判定チェック
		void CheckCollide3();
		//当たり判定チェック
		void CheckCollide4();
		//当たり判定チェック
		void CheckCollide5();
		//二つのオブジェクトが接続しているかどうか
		bool IsCollide(const Rigidbody& rigidA, const Rigidbody& rigidB, CollidableData* colliderA, CollidableData* colliderB) const;
		//二つのオブジェクトが接続しているかどうか
		bool IsCollide1(const Rigidbody& rigidA, const Rigidbody& rigidB, CollidableData* colliderA, CollidableData* colliderB) const;
		//二つのオブジェクトが接続しているかどうか
		bool IsCollide2(const Rigidbody& rigidA, const Rigidbody& rigidB, CollidableData* colliderA, CollidableData* colliderB) const;
		//二つのオブジェクトが接続しているかどうか
		bool IsCollide3(const Rigidbody& rigidA, const Rigidbody& rigidB, CollidableData* colliderA, CollidableData* colliderB) const;
		//二つのオブジェクトが接続しているかどうか
		bool IsCollide4(const Rigidbody& rigidA, const Rigidbody& rigidB, CollidableData* colliderA, CollidableData* colliderB) const;
		//二つのオブジェクトが接続しているかどうか
		bool IsCollide5(const Rigidbody& rigidA, const Rigidbody& rigidB, CollidableData* colliderA, CollidableData* colliderB) const;
		//当たったオブジェクトのペアを登録する
		void AddNewCollideInfo(const std::shared_ptr<Collidable>& objA, const std::shared_ptr<Collidable>& objB, SendCollideInfo& info);
		//移動予定の座標を修正する
		void FixNextPosition(const Rigidbody& primaryRigid, Rigidbody& secondaryRigid, CollidableData* primaryCollider, CollidableData* secondaryCollider) const;
		//ジャンプ中の移動予定の座標を修正する
		void FixJumpNextPosition(const Rigidbody& primaryRigid, Rigidbody& secondaryRigid, CollidableData* primaryCollider, CollidableData* secondaryCollider) const;
		//種類毎に衝突通知を飛ばす配列に追加する
		void CheckSendOnCollideInfo(SendCollideInfo& preSendInfo, SendCollideInfo& newSendInfo, bool isTrigger);
		//衝突通知を飛ばす配列に追加する
		void AddOnCollideInfo(const std::shared_ptr<Collidable>& mySelf, const std::shared_ptr<Collidable>& person, eOnCollideInfoKind kind);
		//最終的な位置を決定する
		void FixPosition();

	private:
		//チェックしたポリゴンが壁ポリゴンか床ポリゴンかを判断し保持する
		void CheckWallAndFloor(std::shared_ptr<Collidable>& col);
		//壁ポリゴンとの当たり判定をチェックし、移動させる
		void FixPositionWithWall(std::shared_ptr<Collidable>& col);
		//壁の中から押し出す
		void FixPositionWithWallInternal(std::shared_ptr<Collidable>& col);
		//床ポリゴンとの当たり判定をチェックし、移動させる
		void FixNowPositionWithFloor(std::shared_ptr<Collidable>& col);

		//チェックしたポリゴンが壁ポリゴンか床ポリゴンかを判断し保持する
		void CheckWallAndFloorEnemy(std::shared_ptr<Collidable>& col);
		//壁ポリゴンとの当たり判定をチェックし、移動させる
		void FixPositionWithWallEnemy(std::shared_ptr<Collidable>& col);
		//壁の中から押し出す
		void FixPositionWithWallInternalEnemy(std::shared_ptr<Collidable>& col);
		//床ポリゴンとの当たり判定をチェックし、移動させる
		void FixNowPositionWithFloorEnemy(std::shared_ptr<Collidable>& col);


	private:
		//登録されたCollidableのリスト
		std::list<std::shared_ptr<Collidable>> m_collidables;
		//衝突したオブジェクトのリスト
		std::list<OnCollideInfoData> m_onCollideInfo;

		//衝突したオブジェクトのリスト
		SendCollideInfo m_newCollideInfo;
		//衝突したオブジェクトのリストのログ
		SendCollideInfo m_preCollideInfo;
		//衝突した押し出ししないオブジェクトのリスト
		SendCollideInfo m_newTriggerInfo;
		//衝突した押し出ししないオブジェクトのリストのログ
		SendCollideInfo m_preTriggerInfo;

		//マップ当たり判定のハンドル
		int m_stageCollisionHandle;

	private:
		//移動したかどうか
		bool m_isMoveFlag = false;
		//ポリゴンに当たったかどうか
		bool m_isHitFlag = false;
		//ポリゴンにあたったかどうか(Enemy)
		bool m_isHitEnemyFlag = false;
		//壁ポリゴンに当たったかどうか
		bool m_isHitWallFlag;
		//壁ポリゴンと判断されたポリゴン数
		int m_wallNum = 0;
		//床ポリゴンと判断されたポリゴン数
		int m_floorNum = 0;

		//地面に触れているかの判定
		bool m_playerFloor = false;
		bool m_enemyFloor = false;

		VECTOR m_ret;

		//当たり判定結果構造体
		MV1_COLL_RESULT_POLY_DIM m_hitDim{};
		//当たり判定結果構造体
		MV1_COLL_RESULT_POLY_DIM m_hitEnemyDim{};
		//壁ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
		MV1_COLL_RESULT_POLY* m_pWallPoly[ColInfo::kMaxColHitPoly]{};
		//床ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
		MV1_COLL_RESULT_POLY* m_pFloorPoly[ColInfo::kMaxColHitPoly]{};
		//ポリゴンの構造体にアクセスするために仕使用するポインタ
		MV1_COLL_RESULT_POLY* m_pPoly = nullptr;
	};
}


