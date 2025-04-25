#include "Assassin.h"
#include "Ui/UI.h"
#include "Manager/EffectManager.h"
#include "Manager/SEManager.h"
#include "Item/EnemyWeapon.h"

namespace
{
	//キャラクター名
	constexpr const char* cCharacterName = "Assassin";
	//モデルパス
	constexpr const char* cModelPath = "Data/Enemy/Assassin.mv1";
	//武器のパス
	constexpr const char* cWeaponPath = "Data/Weapon/Dagger.mv1";
	//アタッチする武器のフレーム名
	constexpr const char* cFrameName = "Pattern";
	//攻撃判定にする武器のフレーム名
	constexpr const char* cFrameAttackName = "Tip";
	//チュートリアル判定
	bool cTutorial;
	//モデルのサイズの拡大率
	constexpr float cModelSize = 0.5f;
	//モデルの長さ
	constexpr float cCapsuleLen = 50.0f;
	//モデルの半径
	constexpr float cCapsuleRadius = 14.0f;
	//モデルの座標を合わせる
	constexpr float cModelPosY = 12.0f;
	//死亡終了
	bool cDead = false;
	//死亡したときのアニメーション
	constexpr float cDeadFrame = 65.0f;
	//索敵範囲
	constexpr float cSearchRadius = 120.0f;
	//近距離の行動に移る距離
	constexpr float cNear = 80.0f;
	//攻撃範囲(ナイフ)
	constexpr float cAttackRadiusKnife = 30.0f;
	//攻撃範囲(蹴り)
	constexpr float cAttackRadiusKick = 25.0f;
	//視野の角度
	constexpr float cAngle = 30.0f;
	//視野の距離
	constexpr float cAngleDistance = 400.0f;
	//攻撃時の移動距離
	float cAttackMove = 0.0f;
	//回避時の移動距離
	float cAvoidanceMove = 0.0f;
	//攻撃ヒット時の移動距離
	float cHitMove = 0.0f;

	//足のエフェクトを移動させる
	VECTOR cLigPos = VGet(0.0f, 0.0f, 0.0f);

	bool cA[4];

	//プレイヤーを発見したとき
	bool cPlayerLook = false;

	//武器のポジション
	const VECTOR cWeaponPosition = VGet(0.0f, 0.0f, 0.0f);
	//武器のX回転
	constexpr float cWeaponX = 0.000f;
	//武器のY回転
	constexpr float cWeaponY = 0.000f;
	//武器のZ回転
	constexpr float cWeaponZ = 4.578f;

	//シングルトン
	EffectManager& cEffect = EffectManager::GetInstance();

	bool cOne = false;

	// 定数の定義
	constexpr float cAvoidanceMoveFrameStart = 30.0f;
	constexpr float cHitMoveFrameStart = 5.0f;
	constexpr float cHitMoveFrameEnd = 25.0f;
	constexpr float cDeathFrame = 36.0f;
	constexpr float cHitEffectOffsetY = 30.0f;
	constexpr float cStrongHitEffectOffsetY = 20.0f;
	constexpr float cModelScale = 0.15f;

	// 定数の定義
	constexpr float cAttackMoveDistance1 = 0.5f;
	constexpr float cAttackMoveDistance2 = 0.8f;
	constexpr float cAttackMoveDistance3 = 0.5f;
	constexpr float cAttackMoveDistance4 = -0.4f;
	constexpr float cIdleFrameDuration = 40.0f;
	constexpr float cAttackFrame1 = 5.0f;
	constexpr float cAttackFrame2 = 11.0f;
	constexpr float cAttackFrame3 = 24.0f;
	constexpr float cAttackFrame4 = 25.0f;
	constexpr float cAttackFrame5 = 30.0f;
	constexpr float cAttackFrame6 = 35.0f;
	constexpr float cAttackFrame7 = 40.0f;
	constexpr float cAttackFrame8 = 48.0f;
	constexpr float cAttackFrame9 = 50.0f;
	constexpr float cAttackFrame10 = 55.0f;
	constexpr float cAttackFrame11 = 62.0f;
	constexpr float cAttackFrame12 = 65.0f;
	constexpr float cAttackFrame13 = 70.0f;
	constexpr float cAttackFrame14 = 28.0f;
	constexpr float cAttackFrame15 = 18.0f;
	constexpr float cAttackFrame16 = 20.0f;

	// ランダムアクションの範囲
	constexpr int cRandomActionMax = 6;
	constexpr int cActionFixed = 5;

}

/// <summary>
/// コンストラクタ
/// </summary>
Assassin::Assassin():
	EnemyBase(Collidable::Priority::High),
	m_attackPos(),
	m_moveFrameRightHand(0),
	m_frameRightHand(VGet(0.0f,0.0f,0.0f)),
	m_attackMove(VGet(0.0f,0.0f,0.0f)),
	m_avoidanceMove(VGet(0.0f,0.0f,0.0f)),
	m_hitMove(VGet(0.0f, 0.0f, 0.0f)),
	m_weaponFrameMatrix()
{
	//当たり判定の設定
	InitCollision(MyLibrary::LibVec3(0.0f, 2.0f, 0.0f), cCapsuleLen, cCapsuleRadius);
	//モデルの読み込み
	LoadModel(cModelPath);
	//アニメーションやステータスを取得
	LoadData(cCharacterName);
	//索敵範囲の設定
	m_searchRadius = cSearchRadius;

	for (int i = 0; i < 2; i++)
	{
		m_daggerFrame[i] = 0;
		m_ligLeftLegFrame[i] = 0;
		m_ligRightLegFrame[i] = 0;
		m_daggerPos[i] = VGet(0.0f, 0.0f, 0.0f);
		m_ligLeftLegPos[i] = VGet(0.0f, 0.0f, 0.0f);
		m_ligRightLegPos[i] = VGet(0.0f, 0.0f, 0.0f);
	}

	//アニメーションの判定初期化
	m_anim.s_attack = false;
	m_anim.s_moveflag = false;
	m_anim.s_hit = false;
}

/// <summary>
/// デストラクタ
/// </summary>
Assassin::~Assassin()
{
	//メモリ解放
	MV1DeleteModel(m_modelHandle);
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Assassin::Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon)
{
	cTutorial = tutorial;

	//代入
	m_pPhysics = physics;

	m_isExist = true;

	Collidable::Init(m_pPhysics);

	//物理クラスの初期化
	InitRigidbody(posX, posY + cCapsuleRadius, posZ);

	//中心座標の設定
	CalculationCenterPos(1.0f, cModelSize);

	//モデルの座標を設定
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//索敵範囲をする当たり判定作成
	InitSearch(cSearchRadius, 35.0f);

	//モデルのサイズ設定
	MV1SetScale(m_modelHandle, VGet(cModelSize, cModelSize, cModelSize));

	weapon.Init(cWeaponPath, cWeaponPosition, cWeaponX, cWeaponY, cWeaponZ, cModelSize * 0.15f);

	//アニメーション設定
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];

	float totalAnimFrame = MV1GetAttachAnimTotalTime(m_modelHandle, m_nowAnimNo);

	m_anim.s_isDead = false;
	cDead = false;

	//最大HPを取得
	m_maxHP = m_status.s_hp;

	if (cTutorial)
	{
		m_bossName = " 混沌の影 ";
		m_subName = " P E P U S I [チュートリアル] ";

		m_bossjudg = true;
	}
	else
	{
		m_bossjudg = false;
	}


	cOne = false;
	m_deadOne = false;

	
}

/// <summary>
/// ゲーム仕様上での初期化処理
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Assassin::GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon)
{
	m_pPhysics = physics;

	Collidable::Init(m_pPhysics);

	//索敵判定をする当たり判定を作成
	InitSearch(cSearchRadius, 35.0f);

	//中心座標の設定
	CalculationCenterPos(1.0f, cModelSize);

	//物理クラスの初期化
	InitRigidbody(posX, posY + cCapsuleRadius, posZ);

	//モデルの座標を設定
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//モデルのサイズ設定
	MV1SetScale(m_modelHandle, VGet(cModelSize, cModelSize, cModelSize));

	weapon.Init(cWeaponPath, cWeaponPosition, cWeaponX, cWeaponY, cWeaponZ, cModelSize * 0.15f);

	//アニメーション設定
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];

	m_anim.s_isDead = false;
	cDead = false;

	//最大HPを取得
	m_maxHP = m_status.s_hp;

	if (cTutorial)
	{
		m_bossName = " 混沌の影 ";
		m_subName = " P E P U S I [チュートリアル] ";

		m_bossjudg = true;
	}
	else
	{
		m_bossjudg = false;
	}

	cOne = false;
	m_deadOne = false;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="playerPos"></param>
/// <param name="shieldPos"></param>
/// <param name="isChase"></param>
/// <param name="se"></param>
/// <param name="physics"></param>
void Assassin::Update(MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 shieldPos, bool isChase, SEManager& se, std::shared_ptr<MyLibrary::Physics> physics, EnemyWeapon& weapon)
{
	// アニメーションで移動しているフレームの番号を検索する
	m_moveFrameRightHand = MV1SearchFrame(m_modelHandle, "mixamorig:RightHandThumb2");
	m_ligLeftLegFrame[0] = MV1SearchFrame(m_modelHandle, "mixamorig:LeftLeg");
	m_ligLeftLegFrame[1] = MV1SearchFrame(m_modelHandle, "mixamorig:LeftToe_End_end");
	m_ligRightLegFrame[0] = MV1SearchFrame(m_modelHandle, "mixamorig:RightLeg");
	m_ligRightLegFrame[1] = MV1SearchFrame(m_modelHandle, "mixamorig:RightToe_End_end");

	m_frameRightHand = MV1GetFramePosition(m_modelHandle, m_moveFrameRightHand);
	m_ligLeftLegPos[0] = MV1GetFramePosition(m_modelHandle, m_ligLeftLegFrame[0]);
	m_ligLeftLegPos[1] = MV1GetFramePosition(m_modelHandle, m_ligLeftLegFrame[1]);
	m_ligRightLegPos[0] = MV1GetFramePosition(m_modelHandle, m_ligRightLegFrame[0]);
	m_ligRightLegPos[1] = MV1GetFramePosition(m_modelHandle, m_ligRightLegFrame[1]);

	// 判定の更新
	MyLibrary::LibVec3 attackKnifePos = MyLibrary::LibVec3(m_frameRightHand.x, m_frameRightHand.y, m_frameRightHand.z);
	MyLibrary::LibVec3 attackLeftKickPos1 = MyLibrary::LibVec3(m_ligLeftLegPos[0].x, m_ligLeftLegPos[0].y, m_ligLeftLegPos[0].z);
	MyLibrary::LibVec3 attackLeftKickPos2 = MyLibrary::LibVec3(m_ligLeftLegPos[1].x, m_ligLeftLegPos[1].y, m_ligLeftLegPos[1].z);
	MyLibrary::LibVec3 attackRightKickPos1 = MyLibrary::LibVec3(m_ligRightLegPos[0].x, m_ligRightLegPos[0].y, m_ligRightLegPos[0].z);
	MyLibrary::LibVec3 attackRightKickPos2 = MyLibrary::LibVec3(m_ligRightLegPos[1].x, m_ligRightLegPos[1].y, m_ligRightLegPos[1].z);

	// ローカル→ワールド変換行列を取得する
	m_weaponFrameMatrix = MV1GetFrameLocalWorldMatrix(m_modelHandle, m_moveFrameRightHand);

	weapon.WeaponFrame(cFrameName, cFrameAttackName);
	weapon.Update(m_weaponFrameMatrix, cModelScale);

	// 視野の角度を決める
	m_viewAngle = cAngle;
	// 視野の距離を決める
	m_viewDistance = cAngleDistance;

	// アニメーションの更新
	if (!cDead)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}
	// 死亡したときのアニメーション更新
	else if (cDead && m_nowFrame <= cDeadFrame)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}

	UpdateAnimationBlend();

	DistanceUpdate(playerPos, shieldPos);
	AttackDistance();

	// 怯みモーション
	HitUpdate(10);

	// 怯み状態を解除する
	if (m_anim.s_hit && m_isAnimationFinish)
	{
		m_anim.s_hit = false;
	}
	// 怯んでいる時
	else if (m_anim.s_hit)
	{
		// 動かないようにする
		m_move = VGet(0.0f, 0.0f, 0.0f);
		m_moveVec = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);

		m_enemyAnim.s_rool = false;
		m_anim.s_moveflag = false;

		if (m_anim.s_attack)
		{
			// 判定をリセット
			m_pAttack->CollisionEnd();
			m_pLigAttack->CollisionEnd();

			m_anim.s_attack = false;
		}

		// 怯み移動処理
		if (m_nowFrame > cHitMoveFrameStart && m_nowFrame <= cHitMoveFrameEnd)
		{
			cHitMove = -1.0f;
		}
		else
		{
			cHitMove = 0.0f;
		}
	}

	// 攻撃終了
	if (m_anim.s_attack && m_isAnimationFinish)
	{
		m_anim.s_attack = false;
	}
	// 回避終了
	if (m_enemyAnim.s_rool && m_isAnimationFinish)
	{
		m_enemyAnim.s_rool = false;
	}

	// 回避時の行動
	if (m_enemyAnim.s_rool)
	{
		if (m_nowFrame <= cAvoidanceMoveFrameStart)
		{
			cAvoidanceMove = -1.0f;

			m_avoidnaceNow = true;
		}
		else
		{
			cAvoidanceMove = 0.0f;

			m_avoidnaceNow = false;
		}
	}

	if (!m_anim.s_attack && !m_enemyAnim.s_rool && !m_anim.s_hit)
	{
		cAttackMove = 0.0f;
		cAvoidanceMove = 0.0f;
		cHitMove = 0.0f;

		// 移動処理
		MoveUpdate();

		cA[0] = true;

		for (int i = 0; i < 4; i++)
		{
			if (i != 0)
			{
				cA[i] = false;
			}
		}
	}
	// 攻撃行動移動
	else if (m_anim.s_attack && !m_enemyAnim.s_rool && !m_anim.s_hit)
	{
		cAvoidanceMove = 0.0f;
		cHitMove = 0.0f;

		m_attackMove = VScale(VGet(sinf(m_angle), 0.0f, cosf(m_angle)), cAttackMove);

		// 攻撃移動処理
		MoveAnimUpdate(m_attackMove);

		cA[1] = true;

		for (int i = 0; i < 4; i++)
		{
			if (i != 1)
			{
				cA[i] = false;
			}
		}
	}
	// 回避行動移動
	else if (m_enemyAnim.s_rool && !m_anim.s_attack && !m_anim.s_hit)
	{
		cAttackMove = 0.0f;
		cHitMove = 0.0f;

		m_avoidanceMove = VScale(VGet(sinf(m_angle), 0.0f, cosf(m_angle)), cAvoidanceMove);

		// 回避移動処理
		MoveAnimUpdate(m_avoidanceMove);

		cA[2] = true;

		for (int i = 0; i < 4; i++)
		{
			if (i != 2)
			{
				cA[i] = false;
			}
		}
	}
	// 怯み行動移動
	else if (m_anim.s_hit && !m_enemyAnim.s_rool && !m_anim.s_attack)
	{
		cAttackMove = 0.0f;
		cAvoidanceMove = 0.0f;

		m_hitMove = VScale(VGet(sinf(m_angle), 0.0f, cosf(m_angle)), cHitMove);

		// 怯み移動処理
		MoveAnimUpdate(m_hitMove);

		cA[3] = true;

		for (int i = 0; i < 4; i++)
		{
			if (i != 3)
			{
				cA[i] = false;
			}
		}
	}

	// ターゲット状態
	TargetNow();

	// 攻撃していないとき
	if (!m_anim.s_attack)
	{
		if (!cOne)
		{
			InitAttack(0.0f);
			InitLigAttack(attackLeftKickPos1, attackLeftKickPos2, cAttackRadiusKick);
			InitAttackDamage(0.0f);

			cOne = true;
		}
	}

	// 攻撃を受けた時
	if (m_isEnterHit)
	{
		m_status.s_hp -= m_col->GetAttack() - m_status.s_defense;
		// Hitエフェクト
		cEffect.EffectCreate("Hit", VGet(rigidbody->GetPos().x, rigidbody->GetPos().y + cHitEffectOffsetY, rigidbody->GetPos().z));
		// HitSE再生
		PlaySoundMem(se.GetHitSE(), DX_PLAYTYPE_BACK, true);

		// HPが0になるとヒットしない
		if (m_status.s_hp > 0.0f)
		{
			m_anim.s_hit = true;
		}
	}
	else if (m_isEnterStrengtHit)
	{
		m_status.s_hp -= m_strengthCol->GetAttack() - m_status.s_defense;

		// Hitエフェクト
		cEffect.EffectCreate("Hit", VGet(rigidbody->GetPos().x, rigidbody->GetPos().y + cStrongHitEffectOffsetY, rigidbody->GetPos().z));

		// HitSE再生
		PlaySoundMem(se.GetHitSE(), DX_PLAYTYPE_BACK, true);

		// HPが0になるとヒットしない
		if (m_status.s_hp > 0.0f)
		{
			m_anim.s_hit = true;
		}
	}

	if (cTutorial)
	{
		// プレイヤーがボス部屋に入ったら
		if (m_isBossDiscovery && !cDead && !m_anim.s_hit)
		{
			BossAction(playerPos, isChase, se, weapon);
		}
	}
	else
	{
		// 怯んでない時
		if (!m_anim.s_hit)
		{
			// まだできていないため後でボスアクションの中身を写す
			Action(playerPos, isChase, se, weapon);
		}
	}

	TriggerUpdate();
	HitTriggerUpdate();

	// 判定の更新
	MyLibrary::LibVec3 centerPos = rigidbody->GetPos();
	m_pSearch->Update(centerPos);

	// チュートリアルだった場合
	if (cTutorial)
	{
		// 死んだ時
		if (m_status.s_hp <= 0.0f)
		{
			// アニメーションを初期化
			m_anim.s_attack = false;
			m_anim.s_hit = false;
			m_anim.s_moveflag = false;

			Death();

			if (m_nowFrame == cDeathFrame)
			{
				// 死亡SE再生
				PlaySoundMem(se.GetDiedSE(), DX_PLAYTYPE_BACK, true);
			}

			cDead = true;
			m_isBossDead = true;
		}
		else
		{
			m_isBossDead = false;
		}
	}
	else
	{
		// 死んだとき
		if (m_status.s_hp <= 0.0f)
		{
			// アニメーションを初期化
			m_anim.s_attack = false;
			m_anim.s_hit = false;
			m_anim.s_moveflag = false;

			Death();

			if (m_nowFrame == cDeathFrame)
			{
				// 死亡SE再生
				PlaySoundMem(se.GetDiedSE(), DX_PLAYTYPE_BACK, true);
			}

			cDead = true;
		}
	}
	
}

void Assassin::Action(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se, EnemyWeapon& weapon)
{
	//判定の更新
	MyLibrary::LibVec3 attackKnifePos = MyLibrary::LibVec3(m_frameRightHand.x, m_frameRightHand.y, m_frameRightHand.z);
	MyLibrary::LibVec3 attackKnifePos1 = MyLibrary::LibVec3(weapon.GetFramePos1().x, weapon.GetFramePos1().y, weapon.GetFramePos1().z);
	MyLibrary::LibVec3 attackKnifePos2 = MyLibrary::LibVec3(weapon.GetFramePos2().x, weapon.GetFramePos2().y, weapon.GetFramePos2().z);
	MyLibrary::LibVec3 attackLeftKickPos1 = MyLibrary::LibVec3(m_ligLeftLegPos[0].x, m_ligLeftLegPos[0].y, m_ligLeftLegPos[0].z);
	MyLibrary::LibVec3 attackLeftKickPos2 = MyLibrary::LibVec3(m_ligLeftLegPos[1].x, m_ligLeftLegPos[1].y, m_ligLeftLegPos[1].z);
	MyLibrary::LibVec3 attackRightKickPos1 = MyLibrary::LibVec3(m_ligRightLegPos[0].x, m_ligRightLegPos[0].y, m_ligRightLegPos[0].z);
	MyLibrary::LibVec3 attackRightKickPos2 = MyLibrary::LibVec3(m_ligRightLegPos[1].x, m_ligRightLegPos[1].y, m_ligRightLegPos[1].z);

	//正面
	if (!IsPlayerInView(playerPos))
	{
		cPlayerLook = true;
	}
	else
	{
		cPlayerLook = false;
	}

	//プレイヤーを見つけた時
	if (m_pSearch->GetIsStay() || cPlayerLook)
	{
		// 攻撃してない時
		if (!m_anim.s_attack)
		{
			// 方向を決める
			AngleUpdate(playerPos);
		}

		// 近くじゃない時の行動
		if (m_difPSize > cNear && !m_enemyAnim.s_rool && m_randomAction != 6)
		{
			WalkUpdate("Walk", 2);

			// 攻撃してないときの処理
			if (!m_anim.s_attack)
			{
				// 歩くアニメーション
				m_anim.s_moveflag = true;

				m_status.s_speed = 0.02f;

				m_move = VScale(m_difPlayer, m_status.s_speed);

				m_enemyAnim.s_rool = false;
			}
		}
		// 近くに行った時の行動
		else if (m_difPSize <= cNear)
		{
			// ランダム行動で0が出た場合
			// 攻撃1
			if (m_randomAction == 0)
			{
				// 攻撃モーションさせる
				m_anim.s_attack = true;

				m_move = VGet(0.0f, 0.0f, 0.0f);

				AttackUpdate("Attack1", 3);

				m_enemyAnim.s_rool = false;
				m_anim.s_moveflag = false;
			}
			// ランダム行動で1が出た場合
			// 攻撃2
			else if (m_randomAction == 1)
			{
				// 攻撃モーションさせる
				m_anim.s_attack = true;

				m_move = VGet(0.0f, 0.0f, 0.0f);

				AttackUpdate("Attack2", 4);

				// 回避状態終了
				m_avoidnaceNow = false;

				m_enemyAnim.s_rool = false;
				m_anim.s_moveflag = false;
			}
			// ランダム行動で2が出た場合
			// 攻撃3
			else if (m_randomAction == 2)
			{
				// 攻撃モーションさせる
				m_anim.s_attack = true;

				m_move = VGet(0.0f, 0.0f, 0.0f);

				AttackUpdate("Attack3", 5);

				// 回避状態終了
				m_avoidnaceNow = false;

				m_enemyAnim.s_rool = false;
				m_anim.s_moveflag = false;
			}
			// ランダム行動で3が出た場合
			// 攻撃4
			else if (m_randomAction == 3)
			{
				// 攻撃モーションさせる
				m_anim.s_attack = true;

				m_move = VGet(0.0f, 0.0f, 0.0f);

				AttackUpdate("Attack4", 6);

				// 回避状態終了
				m_avoidnaceNow = false;

				m_enemyAnim.s_rool = false;
				m_anim.s_moveflag = false;
			}
			// ランダム行動で4が出た場合
			// 攻撃5
			else if (m_randomAction == 4)
			{
				// 攻撃モーションさせる
				m_anim.s_attack = true;

				m_move = VGet(0.0f, 0.0f, 0.0f);

				AttackUpdate("Attack5", 7);

				// 回避状態終了
				m_avoidnaceNow = false;

				m_anim.s_moveflag = false;
				m_enemyAnim.s_rool = false;
			}
			// ランダム行動で5が出た場合
			// 回避
			else if (m_randomAction == 5)
			{
				m_enemyAnim.s_rool = true;

				m_nowAnimIdx = m_animIdx["Roll"];
				ChangeAnim(m_nowAnimIdx, m_animOne[8], m_animOne);

				m_anim.s_moveflag = false;
				m_anim.s_attack = false;
			}
			// ランダム行動で6は出た場合
			else if (m_randomAction == 6)
			{
				// 回避状態終了
				m_avoidnaceNow = false;

				m_enemyAnim.s_rool = false;
				m_anim.s_moveflag = false;
				m_anim.s_attack = false;
			}
		}

		// 攻撃時の行動
		if (m_anim.s_attack)
		{
			if (m_randomAction == 0)
			{
				// 攻撃判定のポジション
				InitAttackLigUpdate(attackKnifePos1, attackKnifePos2);

				// 攻撃時の移動する距離
				if (m_nowFrame <= cAttackFrame4)
				{
					cAttackMove = cAttackMoveDistance1;
				}
				else
				{
					cAttackMove = 0.0f;
				}

				if (m_nowFrame == cAttackFrame1)
				{
					InitLigAttack(attackKnifePos1, attackKnifePos2, cAttackRadiusKnife);
					InitAttackDamage(m_status.s_attack);
				}
				// アニメーションフレーム中に攻撃判定を出す
				if (m_nowFrame == cAttackFrame3)
				{
					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame == cAttackFrame5)
				{
					InitAttackDamage(m_status.s_attack);
					// 判定をリセット
					m_pAttack->CollisionEnd();
					m_pLigAttack->CollisionEnd();
				}
				// アニメーションフレーム中に攻撃判定を出す
				if (m_nowFrame == cAttackFrame7)
				{
					// 攻撃SE再生
					PlaySoundMem(se.GetKnifeSE(), DX_PLAYTYPE_BACK, true);

					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame >= cAttackFrame8)
				{
					InitAttackDamage(0.0f);
					// 判定をリセット
					m_pAttack->CollisionEnd();
					m_pLigAttack->CollisionEnd();
				}
			}
			else if (m_randomAction == 1)
			{
				// 攻撃判定のポジション
				InitAttackLigUpdate(attackKnifePos1, attackKnifePos2);

				// 攻撃時の移動する距離
				cAttackMove = 0.0f;

				if (m_nowFrame == cAttackFrame1)
				{
					InitLigAttack(attackKnifePos1, attackKnifePos2, cAttackRadiusKnife);
					InitAttackDamage(m_status.s_attack);
				}
				// アニメーションフレーム中に攻撃判定を出す
				if (m_nowFrame == cAttackFrame3)
				{
					// 攻撃SE再生
					PlaySoundMem(se.GetKnifeSE(), DX_PLAYTYPE_BACK, true);

					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame >= cAttackFrame14)
				{
					InitAttackDamage(0.0f);
					// 判定をリセット
					m_pAttack->CollisionEnd();
					m_pLigAttack->CollisionEnd();
				}
			}
			else if (m_randomAction == 2)
			{
				// 攻撃判定のポジション
				InitAttackLigUpdate(attackKnifePos1, attackKnifePos2);

				// 攻撃時の移動する距離
				cAttackMove = 0.0f;

				if (m_nowFrame == cAttackFrame1)
				{
					InitLigAttack(attackKnifePos1, attackKnifePos2, cAttackRadiusKnife);
					InitAttackDamage(m_status.s_attack);
				}
				// アニメーションフレーム中に攻撃判定を出す
				if (m_nowFrame == cAttackFrame2)
				{
					// 攻撃SE再生
					PlaySoundMem(se.GetKnifeSE(), DX_PLAYTYPE_BACK, true);

					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame >= cAttackFrame15)
				{
					InitAttackDamage(0.0f);
					// 判定をリセット
					m_pAttack->CollisionEnd();
					m_pLigAttack->CollisionEnd();
				}
			}
			else if (m_randomAction == 3)
			{
				// 攻撃判定のポジション
				InitAttackLigUpdate(attackLeftKickPos1, attackLeftKickPos2);

				// 攻撃時の移動する距離
				if (m_nowFrame <= cAttackFrame16)
				{
					cAttackMove = 0.0f;
				}
				else if (m_nowFrame > cAttackFrame16 && m_nowFrame <= cAttackFrame7)
				{
					cAttackMove = cAttackMoveDistance2;
				}
				else if (m_nowFrame > cAttackFrame7 && m_nowFrame <= cAttackFrame12)
				{
					cAttackMove = cAttackMoveDistance3;
				}
				else if (m_nowFrame > cAttackFrame12 && m_nowFrame <= cAttackFrame13)
				{
					cAttackMove = cAttackMoveDistance2;
				}
				else
				{
					cAttackMove = 0.0f;
				}

				if (m_nowFrame == cAttackFrame1)
				{
					InitLigAttack(attackLeftKickPos1, attackLeftKickPos2, cAttackRadiusKick);
					InitAttackDamage(m_status.s_attack);
				}
				// アニメーションフレーム中に攻撃判定を出す
				if (m_nowFrame == cAttackFrame11)
				{
					// 攻撃SE再生
					PlaySoundMem(se.GetKickSE(), DX_PLAYTYPE_BACK, true);

					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame >= cAttackFrame13)
				{
					InitAttackDamage(0.0f);
					// 判定をリセット
					m_pAttack->CollisionEnd();
					m_pLigAttack->CollisionEnd();
				}
			}
			else if (m_randomAction == 4)
			{
				// 攻撃判定のポジション
				InitAttackLigUpdate(attackRightKickPos1, attackRightKickPos2);

				// 攻撃時の移動する距離
				if (m_nowFrame <= cAttackFrame3)
				{
					cAttackMove = 0.0f;
				}
				else if (m_nowFrame > cAttackFrame3 && m_nowFrame <= cAttackFrame4)
				{
					cAttackMove = cAttackMoveDistance2;
				}
				else if (m_nowFrame > cAttackFrame6 && m_nowFrame <= cAttackFrame10)
				{
					cAttackMove = cAttackMoveDistance4;
				}
				else
				{
					cAttackMove = 0.0f;
				}

				if (m_nowFrame == cAttackFrame1)
				{
					InitLigAttack(attackRightKickPos1, attackRightKickPos2, cAttackRadiusKick);
					InitAttackDamage(m_status.s_attack);
				}
				// アニメーションフレーム中に攻撃判定を出す
				if (m_nowFrame == cAttackFrame4)
				{
					// 攻撃SE再生
					PlaySoundMem(se.GetKickSE(), DX_PLAYTYPE_BACK, true);

					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame >= cAttackFrame6)
				{
					InitAttackDamage(0.0f);
					// 判定をリセット
					m_pAttack->CollisionEnd();
					m_pLigAttack->CollisionEnd();
				}
			}
		}

		if (m_randomAction == 6)
		{
			m_nowAnimIdx = m_animIdx["Idle"];
			ChangeAnim(m_nowAnimIdx, m_animOne[9], m_animOne);

			cAttackMove = 0.0f;
			m_move = VGet(0.0f, 0.0f, 0.0f);
			cAvoidanceMove = 0.0f;

			if (m_nowFrame >= cIdleFrameDuration)
			{
				m_isAnimationFinish = true;
			}
		}

		// アニメーションが終わる度にランダムな行動を行う
		if (m_isAnimationFinish)
		{
			// プレイヤーが見えている時
			if (cPlayerLook)
			{
				m_randomAction = GetRand(cRandomActionMax);
			}
			// プレイヤーが見えていない時
			else
			{
				// 回避するようにする
				m_randomAction = cActionFixed;
			}
		}

		// 移動方向
		m_moveVec = MyLibrary::LibVec3(m_move.x, m_move.y, m_move.z);
	}
	//プレイヤーを見失った時
	else if (m_pSearch->GetIsExit() && !cPlayerLook)
	{
		//アイドル状態にする
		IdleUpdate();
		//歩かないようにする
		m_anim.s_moveflag = false;
		//攻撃しないようにする
		m_anim.s_attack = false;
		//判定をリセット
		m_pAttack->CollisionEnd();

		m_enemyAnim.s_rool = false;


		m_moveVec = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);
	}
}

/// <summary>
/// ボスの時の行動
/// </summary>
/// <param name="playerPos"></param>
/// <param name="isChase"></param>
/// <param name="se"></param>
void Assassin::BossAction(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se, EnemyWeapon& weapon)
{
	// 判定の更新
	MyLibrary::LibVec3 attackKnifePos = MyLibrary::LibVec3(m_frameRightHand.x, m_frameRightHand.y, m_frameRightHand.z);
	MyLibrary::LibVec3 attackKnifePos1 = MyLibrary::LibVec3(weapon.GetFramePos1().x, weapon.GetFramePos1().y, weapon.GetFramePos1().z);
	MyLibrary::LibVec3 attackKnifePos2 = MyLibrary::LibVec3(weapon.GetFramePos2().x, weapon.GetFramePos2().y, weapon.GetFramePos2().z);
	MyLibrary::LibVec3 attackLeftKickPos1 = MyLibrary::LibVec3(m_ligLeftLegPos[0].x, m_ligLeftLegPos[0].y, m_ligLeftLegPos[0].z);
	MyLibrary::LibVec3 attackLeftKickPos2 = MyLibrary::LibVec3(m_ligLeftLegPos[1].x, m_ligLeftLegPos[1].y, m_ligLeftLegPos[1].z);
	MyLibrary::LibVec3 attackRightKickPos1 = MyLibrary::LibVec3(m_ligRightLegPos[0].x, m_ligRightLegPos[0].y, m_ligRightLegPos[0].z);
	MyLibrary::LibVec3 attackRightKickPos2 = MyLibrary::LibVec3(m_ligRightLegPos[1].x, m_ligRightLegPos[1].y, m_ligRightLegPos[1].z);

	// 正面
	if (!IsPlayerInView(playerPos))
	{
		cPlayerLook = true;
	}
	else
	{
		cPlayerLook = false;
	}

	// 攻撃してない時
	if (!m_anim.s_attack)
	{
		// 方向を決める
		AngleUpdate(playerPos);
	}

	// 近くじゃない時の行動
	if (m_difPSize > cNear && !m_enemyAnim.s_rool && m_randomAction != 6)
	{
		WalkUpdate("Walk", 2);

		// 攻撃してないときの処理
		if (!m_anim.s_attack)
		{
			// 歩くアニメーション
			m_anim.s_moveflag = true;

			m_status.s_speed = 0.02f;

			m_move = VScale(m_difPlayer, m_status.s_speed);

			m_enemyAnim.s_rool = false;
		}
	}
	// 近くに行った時の行動
	else if (m_difPSize <= cNear)
	{
		// ランダム行動で0が出た場合
		// 攻撃1
		if (m_randomAction == 0)
		{
			// 攻撃モーションさせる
			m_anim.s_attack = true;

			m_move = VGet(0.0f, 0.0f, 0.0f);

			AttackUpdate("Attack1", 3);

			m_enemyAnim.s_rool = false;
			m_anim.s_moveflag = false;
		}
		// ランダム行動で1が出た場合
		// 攻撃2
		else if (m_randomAction == 1)
		{
			// 攻撃モーションさせる
			m_anim.s_attack = true;

			m_move = VGet(0.0f, 0.0f, 0.0f);

			AttackUpdate("Attack2", 4);

			// 回避状態終了
			m_avoidnaceNow = false;

			m_enemyAnim.s_rool = false;
			m_anim.s_moveflag = false;
		}
		// ランダム行動で2が出た場合
		// 攻撃3
		else if (m_randomAction == 2)
		{
			// 攻撃モーションさせる
			m_anim.s_attack = true;

			m_move = VGet(0.0f, 0.0f, 0.0f);

			AttackUpdate("Attack3", 5);

			// 回避状態終了
			m_avoidnaceNow = false;

			m_enemyAnim.s_rool = false;
			m_anim.s_moveflag = false;
		}
		// ランダム行動で3が出た場合
		// 攻撃4
		else if (m_randomAction == 3)
		{
			// 攻撃モーションさせる
			m_anim.s_attack = true;

			m_move = VGet(0.0f, 0.0f, 0.0f);

			AttackUpdate("Attack4", 6);

			// 回避状態終了
			m_avoidnaceNow = false;

			m_enemyAnim.s_rool = false;
			m_anim.s_moveflag = false;
		}
		// ランダム行動で4が出た場合
		// 攻撃5
		else if (m_randomAction == 4)
		{
			// 攻撃モーションさせる
			m_anim.s_attack = true;

			m_move = VGet(0.0f, 0.0f, 0.0f);

			AttackUpdate("Attack5", 7);

			// 回避状態終了
			m_avoidnaceNow = false;

			m_anim.s_moveflag = false;
			m_enemyAnim.s_rool = false;
		}
		// ランダム行動で5が出た場合
		// 回避
		else if (m_randomAction == 5)
		{
			m_enemyAnim.s_rool = true;

			m_nowAnimIdx = m_animIdx["Roll"];
			ChangeAnim(m_nowAnimIdx, m_animOne[8], m_animOne);

			m_anim.s_moveflag = false;
			m_anim.s_attack = false;
		}
		// ランダム行動で6は出た場合
		else if (m_randomAction == 6)
		{
			// 回避状態終了
			m_avoidnaceNow = false;

			m_enemyAnim.s_rool = false;
			m_anim.s_moveflag = false;
			m_anim.s_attack = false;
		}
	}

	// 攻撃時の行動
	if (m_anim.s_attack)
	{
		if (m_randomAction == 0)
		{
			// 攻撃判定のポジション
			InitAttackLigUpdate(attackKnifePos1, attackKnifePos2);

			// 攻撃時の移動する距離
			if (m_nowFrame <= cAttackFrame4)
			{
				cAttackMove = cAttackMoveDistance1;
			}
			else
			{
				cAttackMove = 0.0f;
			}

			//エフェクトを再生する
			if (m_nowFrame >= cAttackFrame7 && m_nowFrame <= cAttackFrame8)
			{
				//攻撃の切っ先にエフェクトをつける
				cEffect.UpdateEffectPosition("BossAttack", VGet(attackKnifePos2.x, attackKnifePos2.y, attackKnifePos2.z));
			}


			if (m_nowFrame == cAttackFrame1)
			{
				InitLigAttack(attackKnifePos1, attackKnifePos2, cAttackRadiusKnife);
				InitAttackDamage(m_status.s_attack);
			}
			// アニメーションフレーム中に攻撃判定を出す
			if (m_nowFrame == cAttackFrame3)
			{
				//エフェクトを作る
				cEffect.EffectCreate("BossShockWave", VGet(attackKnifePos2.x, attackKnifePos2.y, attackKnifePos2.z));

				// 攻撃SE再生
				PlaySoundMem(se.GetKnifeSE(), DX_PLAYTYPE_BACK, true);

				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame == cAttackFrame5)
			{
				InitAttackDamage(m_status.s_attack);
				// 判定をリセット
				m_pAttack->CollisionEnd();
				m_pLigAttack->CollisionEnd();
			}
			// アニメーションフレーム中に攻撃判定を出す
			if (m_nowFrame == cAttackFrame7)
			{
				//エフェクトを作る
				cEffect.EffectCreate("BossAttack", VGet(attackKnifePos2.x, attackKnifePos2.y, attackKnifePos2.z));

				// 攻撃SE再生
				PlaySoundMem(se.GetKnifeSE(), DX_PLAYTYPE_BACK, true);

				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame >= cAttackFrame8)
			{
				InitAttackDamage(0.0f);
				// 判定をリセット
				m_pAttack->CollisionEnd();
				m_pLigAttack->CollisionEnd();
			}

			//エフェクトを作る
			if (m_nowFrame >= cAttackFrame3 && m_nowFrame <= cAttackFrame5)
			{
				//攻撃判定の周りにエフェクトをつける
				cEffect.UpdateEffectPosition("BossShockWave", VGet(attackKnifePos2.x, attackKnifePos2.y, attackKnifePos2.z));
				cEffect.UpdateEffectRotation("BossShockWave", VGet(0.0f, m_angle, 0.0f));
			}

		}
		else if (m_randomAction == 1)
		{
			// 攻撃判定のポジション
			InitAttackLigUpdate(attackKnifePos1, attackKnifePos2);

			// 攻撃時の移動する距離
			cAttackMove = 0.0f;

			//エフェクトを再生する
			if (m_nowFrame >= cAttackFrame3 && m_nowFrame <= cAttackFrame14)
			{
				//攻撃の切っ先にエフェクトをつける
				cEffect.UpdateEffectPosition("BossAttack", VGet(attackKnifePos2.x, attackKnifePos2.y, attackKnifePos2.z));
			}

			if (m_nowFrame == cAttackFrame1)
			{
				InitLigAttack(attackKnifePos1, attackKnifePos2, cAttackRadiusKnife);
				InitAttackDamage(m_status.s_attack);
			}
			// アニメーションフレーム中に攻撃判定を出す
			if (m_nowFrame == cAttackFrame3)
			{
				//エフェクトを作る
				cEffect.EffectCreate("BossAttack", VGet(attackKnifePos2.x, attackKnifePos2.y, attackKnifePos2.z));

				// 攻撃SE再生
				PlaySoundMem(se.GetKnifeSE(), DX_PLAYTYPE_BACK, true);

				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame >= cAttackFrame14)
			{
				InitAttackDamage(0.0f);
				// 判定をリセット
				m_pAttack->CollisionEnd();
				m_pLigAttack->CollisionEnd();
			}
		}
		else if (m_randomAction == 2)
		{
			// 攻撃判定のポジション
			InitAttackLigUpdate(attackKnifePos1, attackKnifePos2);

			// 攻撃時の移動する距離
			cAttackMove = 0.0f;

			if (m_nowFrame == cAttackFrame1)
			{
				InitLigAttack(attackKnifePos1, attackKnifePos2, cAttackRadiusKnife);
				InitAttackDamage(m_status.s_attack);
			}
			// アニメーションフレーム中に攻撃判定を出す
			if (m_nowFrame == cAttackFrame2)
			{
				//エフェクトを作る
				cEffect.EffectCreate("BossShockWave", VGet(attackKnifePos1.x, attackKnifePos1.y, attackKnifePos1.z));

				// 攻撃SE再生
				PlaySoundMem(se.GetKnifeSE(), DX_PLAYTYPE_BACK, true);

				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame >= cAttackFrame15)
			{
				InitAttackDamage(0.0f);
				// 判定をリセット
				m_pAttack->CollisionEnd();
				m_pLigAttack->CollisionEnd();
			}

			//エフェクトを作る
			if (m_nowFrame >= cAttackFrame2 && m_nowFrame <= cAttackFrame15)
			{
				//攻撃判定の周りにエフェクトをつける
				cEffect.UpdateEffectPosition("BossShockWave", VGet(attackKnifePos1.x, attackKnifePos1.y, attackKnifePos1.z));
				cEffect.UpdateEffectRotation("BossShockWave", VGet(0.0f, m_angle, 0.0f));
			}

		}
		else if (m_randomAction == 3)
		{
			// 攻撃判定のポジション
			InitAttackLigUpdate(attackLeftKickPos1, attackLeftKickPos2);

			// 攻撃時の移動する距離
			if (m_nowFrame <= cAttackFrame16)
			{
				cAttackMove = 0.0f;
			}
			else if (m_nowFrame > cAttackFrame16 && m_nowFrame <= cAttackFrame7)
			{
				cAttackMove = cAttackMoveDistance2;
			}
			else if (m_nowFrame > cAttackFrame7 && m_nowFrame <= cAttackFrame12)
			{
				cAttackMove = cAttackMoveDistance3;
			}
			else if (m_nowFrame > cAttackFrame12 && m_nowFrame <= cAttackFrame13)
			{
				cAttackMove = cAttackMoveDistance2;
			}
			else
			{
				cAttackMove = 0.0f;
			}

			if (m_nowFrame <= cAttackFrame6)
			{
				cLigPos = VGet(attackLeftKickPos1.x, attackLeftKickPos1.y, attackLeftKickPos1.z);
			}
			else if (m_nowFrame >= cAttackFrame6 && m_nowFrame <= cAttackFrame13)
			{
				cLigPos = VGet(attackLeftKickPos2.x, attackLeftKickPos2.y, attackLeftKickPos2.z);
			}

			if (m_nowFrame == cAttackFrame1)
			{
				//エフェクトを作る
				cEffect.EffectCreate("BossWind", cLigPos);

				InitLigAttack(attackLeftKickPos1, attackLeftKickPos2, cAttackRadiusKick);
				InitAttackDamage(m_status.s_attack);

				//嵐SEを流す
				PlaySoundMem(se.GetStormSE(), DX_PLAYTYPE_BACK, true);
			}
			else if (m_nowFrame == cAttackFrame2)
			{
				//エフェクトを作る
				cEffect.EffectCreate("BossWind", cLigPos);
			}
			else if (m_nowFrame == cAttackFrame4)
			{
				//エフェクトを作る
				cEffect.EffectCreate("BossWind", cLigPos);
			}
			else if (m_nowFrame == cAttackFrame6)
			{
				//エフェクトを作る
				cEffect.EffectCreate("BossWind", cLigPos);
			}
			else if (m_nowFrame == cAttackFrame8)
			{
				//エフェクトを作る
				cEffect.EffectCreate("BossWind", cLigPos);
			}

			// アニメーションフレーム中に攻撃判定を出す
			if (m_nowFrame == cAttackFrame11)
			{
				//エフェクトを作る
				cEffect.EffectCreate("BossWind", cLigPos);

				// 攻撃SE再生
				PlaySoundMem(se.GetKickSE(), DX_PLAYTYPE_BACK, true);

				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame >= cAttackFrame13)
			{
				InitAttackDamage(0.0f);
				// 判定をリセット
				m_pAttack->CollisionEnd();
				m_pLigAttack->CollisionEnd();
			}

			//エフェクトを再生する
			if (m_nowFrame >= cAttackFrame1 && m_nowFrame <= cAttackFrame13)
			{
				cEffect.UpdateEffectPosition("BossWind", cLigPos);
			}
		}
		else if (m_randomAction == 4)
		{
			// 攻撃判定のポジション
			InitAttackLigUpdate(attackRightKickPos1, attackRightKickPos2);

			// 攻撃時の移動する距離
			if (m_nowFrame <= cAttackFrame3)
			{
				cAttackMove = 0.0f;
			}
			else if (m_nowFrame > cAttackFrame3 && m_nowFrame <= cAttackFrame4)
			{
				cAttackMove = cAttackMoveDistance2;
			}
			else if (m_nowFrame > cAttackFrame6 && m_nowFrame <= cAttackFrame10)
			{
				cAttackMove = cAttackMoveDistance4;
			}
			else
			{
				cAttackMove = 0.0f;
			}

			if (m_nowFrame == cAttackFrame1)
			{
				//エフェクトを作る
				cEffect.EffectCreate("BossWind", VGet(attackRightKickPos2.x, attackRightKickPos2.y, attackRightKickPos2.z));

				InitLigAttack(attackRightKickPos1, attackRightKickPos2, cAttackRadiusKick);
				InitAttackDamage(m_status.s_attack);
			}
			else if (m_nowFrame == cAttackFrame2)
			{
				//エフェクトを作る
				cEffect.EffectCreate("BossWind", VGet(attackRightKickPos2.x, attackRightKickPos2.y, attackRightKickPos2.z));

				//嵐SEを流す
				PlaySoundMem(se.GetStormSE(), DX_PLAYTYPE_BACK, true);
			}

			// アニメーションフレーム中に攻撃判定を出す
			if (m_nowFrame == cAttackFrame4)
			{
				//エフェクトを作る
				cEffect.EffectCreate("BossWind", VGet(attackRightKickPos2.x, attackRightKickPos2.y, attackRightKickPos2.z));

				// 攻撃SE再生
				PlaySoundMem(se.GetKickSE(), DX_PLAYTYPE_BACK, true);

				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame >= cAttackFrame6)
			{
				InitAttackDamage(0.0f);
				// 判定をリセット
				m_pAttack->CollisionEnd();
				m_pLigAttack->CollisionEnd();
			}

			//エフェクトを再生する
			if (m_nowFrame >= cAttackFrame1 && m_nowFrame <= cAttackFrame6)
			{
				cEffect.UpdateEffectPosition("BossWind", VGet(attackRightKickPos2.x, attackRightKickPos2.y, attackRightKickPos2.z));
			}
		}
	}

	if (m_randomAction == 6)
	{
		m_nowAnimIdx = m_animIdx["Idle"];
		ChangeAnim(m_nowAnimIdx, m_animOne[9], m_animOne);

		cAttackMove = 0.0f;
		m_move = VGet(0.0f, 0.0f, 0.0f);
		cAvoidanceMove = 0.0f;

		if (m_nowFrame >= cIdleFrameDuration)
		{
			m_isAnimationFinish = true;
		}
	}

	// アニメーションが終わる度にランダムな行動を行う
	if (m_isAnimationFinish)
	{
		// プレイヤーが見えている時
		if (cPlayerLook)
		{
			m_randomAction = GetRand(cRandomActionMax);
		}
		// プレイヤーが見えていない時
		else
		{
			// 回避するようにする
			m_randomAction = cActionFixed;
		}
	}

	// 移動方向
	m_moveVec = MyLibrary::LibVec3(m_move.x, m_move.y, m_move.z);
}

void Assassin::Draw(EnemyWeapon& weapon)
{
	//当たり判定座標を取得してモデルの描画座標を設定する
	SetDrawModelPos(cModelPosY);
	//3Dモデルの回転地をセットする
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle, 0.0f));
	weapon.Draw();
	//モデルの描画
	MV1DrawModel(m_modelHandle);

#if false
	DrawFormatString(1000, 500, 0xffffff, "move : %d", cA[0]);
	DrawFormatString(1000, 550, 0xffffff, "attack : %d", cA[1]);
	DrawFormatString(1000, 600, 0xffffff, "avoidance : %d", cA[2]);
	DrawFormatString(1000, 650, 0xffffff, "hit : %d", cA[3]);
	DrawFormatString(1000, 700, 0xffffff, "random : %d", m_randomAction);
#endif
}

/// <summary>
/// UI描画
/// </summary>
/// <param name="ui"></param>
void Assassin::DrawUI(UI& ui)
{
	if (m_isBossDiscovery && !cDead)
	{
		ui.BossHPDraw(m_status.s_hp, m_maxHP, m_bossName, m_subName);
	}
}
