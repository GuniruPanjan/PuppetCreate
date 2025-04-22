#include "Immortal.h"
#include "Ui/UI.h"
#include "Manager/EffectManager.h"
#include "Manager/SEManager.h"

namespace
{
	//キャラクター名
	constexpr const char* cCharacterName = "Immortal";
	//モデルパス
	constexpr const char* cModelPath = "Data/Enemy/immortalModel.mv1";
	//モデルのサイズの拡大率
	constexpr float cModelSize = 0.4f;
	//モデルの長さ
	constexpr float cCapsuleLen = 40.0f;
	//モデルの半径
	constexpr float cCapsuleRadius = 14.0f;
	//モデルの座標を合わせる
	constexpr float cModelPosY = 12.0f;
	//死亡終了
	bool cDead = false;
	//死亡したときのアニメーション
	constexpr float cDeadFrame = 116.0f;
	//索敵範囲
	constexpr float cSearchRadius = 120.0f;
	//近距離の行動に移る距離
	constexpr float cNear = 50.0f;
	//攻撃判定の半径
	constexpr float cAttackRadius = 18.0f;

	//シングルトン
	EffectManager& cEffect = EffectManager::GetInstance();

	// アニメーションフレーム
	constexpr float cAttackStartFrame = 5.0f;
	constexpr float cAttackHitFrame = 22.0f;
	constexpr float cAttackEndFrame = 35.0f;
	constexpr float cDeathSoundFrame = 50.0f;

	// スピード
	constexpr float cWalkSpeed = 0.01f;
	constexpr float cRunSpeed = 0.3f;

	// ランダムアクションの範囲
	constexpr int cRandomActionMax = 2;
}

/// <summary>
/// コンストラクタ
/// </summary>
Immortal::Immortal() :
	EnemyBase(Collidable::Priority::Middle)
{
	//当たり判定の設定
	InitCollision(MyLibrary::LibVec3(0.0f, 2.0f, 0.0f), cCapsuleLen, cCapsuleRadius);
	//モデルの読み込み
	LoadModel(cModelPath);
	//アニメーションやステータスを取得
	LoadData(cCharacterName);
	//索敵範囲の設定
	m_searchRadius = cSearchRadius;

	//アニメーションの判定初期化
	m_anim.s_attack = false;
	m_anim.s_moveflag = false;
	m_anim.s_hit = false;

	for (int i = 0; i < 2; i++)
	{
		m_frameRightHand[i] = -1;
		m_ligRightPos[i] = VGet(0.0f, 0.0f, 0.0f);
	}
}

/// <summary>
/// デストラクタ
/// </summary>
Immortal::~Immortal()
{
	//メモリ解放
	MV1DeleteModel(m_modelHandle);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="physics">物理クラスのポインタ</param>
void Immortal::Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon)
{
	//代入
	m_pPhysics = physics;

	m_isExist = true;

	Collidable::Init(m_pPhysics);

	//物理クラスの初期化
	InitRigidbody(posX, posY + 14.0f, posZ);

	//中心座標の設定
	CalculationCenterPos(1.0f, cModelSize);

	//モデルの座標を設定
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//索敵判定をする当たり判定を作成
	InitSearch(cSearchRadius, 0.0f);
	InitAttack(0.0f);
	InitLigAttack(MyLibrary::LibVec3(0.0f,0.0f,0.0f), MyLibrary::LibVec3(0.0f,0.0f,0.0f), 0.0f);
	InitAttackDamage(0.0f);

	//モデルのサイズ設定
	MV1SetScale(m_modelHandle, VGet(cModelSize, cModelSize, cModelSize));

	//アニメーション設定
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];

	float totalAnimFrame = MV1GetAttachAnimTotalTime(m_modelHandle, m_nowAnimNo);

	//チュートリアル用HP
	if (tutorial)
	{
		m_status.s_hp = 50.0f;
	}

	//最大HPを取得
	m_maxHP = m_status.s_hp;

	//死をfalseにする
	m_anim.s_isDead = false;
	cDead = false;
	m_deadOne = false;

	m_bossjudg = false;
}

/// <summary>
/// ゲームの仕様上での初期化
/// </summary>
/// <param name="posX">X座標</param>
/// <param name="posY">Y座標</param>
/// <param name="posZ">Z座標</param>
/// <param name="physics">物理クラス</param>
void Immortal::GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon)
{
	m_pPhysics = physics;

	Collidable::Init(m_pPhysics);

	//索敵判定をする当たり判定を作成
	InitSearch(cSearchRadius, 0.0f);
	InitAttack(0.0f);
	InitLigAttack(MyLibrary::LibVec3(0.0f, 0.0f, 0.0f), MyLibrary::LibVec3(0.0f, 0.0f, 0.0f), 0.0f);
	InitAttackDamage(0.0f);

	//中心座標の設定
	CalculationCenterPos(1.0f, cModelSize);

	//物理クラスの初期化
	InitRigidbody(posX, posY + 14.0f, posZ);

	//モデルの座標を設定
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//モデルのサイズ設定
	MV1SetScale(m_modelHandle, VGet(cModelSize, cModelSize, cModelSize));

	//アニメーション設定
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];

	//チュートリアル用HP
	if (tutorial)
	{
		m_status.s_hp = 50.0f;
	}

	//最大HPを取得
	m_maxHP = m_status.s_hp;

	m_anim.s_isDead = false;
	cDead = false;
	m_deadOne = false;

	m_bossjudg = false;

}

/// <summary>
/// 更新
/// </summary>
/// <param name="playerPos">プレイヤー座標</param>
/// <param name="isChase">プレイヤーと戦えるかどうか</param>
void Immortal::Update(MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 shieldPos, bool isChase, SEManager& se, std::shared_ptr<MyLibrary::Physics> physics, EnemyWeapon& weapon)
{
	//アニメーションで移動しているフレームの番号を検索する
	m_frameRightHand[0] = MV1SearchFrame(m_modelHandle, "mixamorig:RightForeArm");
	m_frameRightHand[1] = MV1SearchFrame(m_modelHandle, "mixamorig:RightHandThumb4");

	m_ligRightPos[0] = MV1GetFramePosition(m_modelHandle, m_frameRightHand[0]);
	m_ligRightPos[1] = MV1GetFramePosition(m_modelHandle, m_frameRightHand[1]);

	float totalAnimFrame = MV1GetAttachAnimTotalTime(m_modelHandle, m_nowAnimNo);

	//アニメーションの更新
	if (!cDead)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);

	}
	//死亡した時のアニメーション更新
	else if (cDead && m_nowFrame <= cDeadFrame)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}

	UpdateAnimationBlend();

	DistanceUpdate(playerPos, shieldPos);
	AttackDistance();

	//怯みモーション
	HitUpdate(6);

	//怯み状態を解除する
	if (m_anim.s_hit && m_isAnimationFinish)
	{
		m_anim.s_hit = false;
	}
	//怯んでいる時
	else if (m_anim.s_hit)
	{
		//動かないようにする
		m_move = VGet(0.0f, 0.0f, 0.0f);
		m_moveVec = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);
	}

	//攻撃終了
	if (m_anim.s_attack && m_isAnimationFinish)
	{
		m_anim.s_attack = false;
	}
	

	//怯んでないとき
	if (!m_anim.s_hit && !m_anim.s_isDead)
	{
		//アクション
		Action(playerPos, isChase, se);
	}

	//移動処理
	MoveUpdate();

	//ターゲット状態
	TargetNow();

	//攻撃を受けた時
	if (m_isEnterHit)
	{
		m_status.s_hp -= m_col->GetAttack() - m_status.s_defense;

		//Hitエフェクト
		cEffect.EffectCreate("Hit", VGet(rigidbody->GetPos().x, rigidbody->GetPos().y + 20.0f, rigidbody->GetPos().z));

		//HitSE再生
		PlaySoundMem(se.GetHitSE(), DX_PLAYTYPE_BACK, true);

		//判定をリセット
		m_pAttack->CollisionEnd();
		m_pLigAttack->CollisionEnd();

		//HPが0になるとヒットしない
		if (m_status.s_hp > 0.0f)
		{
			m_anim.s_hit = true;
		}
	}
	else if (m_isEnterStrengtHit)
	{
		m_status.s_hp -= m_strengthCol->GetAttack() - m_status.s_defense;

		//Hitエフェクト
		cEffect.EffectCreate("Hit", VGet(rigidbody->GetPos().x, rigidbody->GetPos().y + 20.0f, rigidbody->GetPos().z));

		//HitSE再生
		PlaySoundMem(se.GetHitSE(), DX_PLAYTYPE_BACK, true);

		//判定をリセット
		m_pAttack->CollisionEnd();
		m_pLigAttack->CollisionEnd();

		//HPが0になるとヒットしない
		if (m_status.s_hp > 0.0f)
		{
			m_anim.s_hit = true;
		}
	}

	TriggerUpdate();
	HitTriggerUpdate();


	//判定の更新
	MyLibrary::LibVec3 centerPos = rigidbody->GetPos();
	m_pSearch->Update(centerPos);

	//死んだとき
	if (m_status.s_hp <= 0.0f)
	{
		//アニメーションを初期化
		m_anim.s_attack = false;
		m_anim.s_hit = false;
		m_anim.s_moveflag = false;

		Death();

		if (m_nowFrame == cDeathSoundFrame)
		{
			//死亡SE再生
			PlaySoundMem(se.GetDiedSE(), DX_PLAYTYPE_BACK, true);
		}
		

		cDead = true;
	}
}

/// <summary>
/// 怯んでないとき以外のアクション
/// </summary>
/// <param name="playerPos">プレイヤーのポジション</param>
/// <param name="isChase">プレイヤーと戦えるかどうか</param>
void Immortal::Action(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se)
{
	MyLibrary::LibVec3 attackRightHandPos1 = MyLibrary::LibVec3(m_ligRightPos[0].x, m_ligRightPos[0].y, m_ligRightPos[0].z);
	MyLibrary::LibVec3 attackRightHandPos2 = MyLibrary::LibVec3(m_ligRightPos[1].x, m_ligRightPos[1].y, m_ligRightPos[1].z);


	//プレイヤーを見つけた時
	if (m_pSearch->GetIsStay())
	{
		//攻撃してない時
		if (!m_anim.s_attack)
		{
			//方向を決める
			AngleUpdate(playerPos);
		}

		//角度を出しプレイヤーの周りを旋回運動させる
		MATRIX mtx = MGetRotY(D2R(m_moveTurning) + DX_PI_F / 2);
		MATRIX mtxR = MGetRotY(D2R(m_moveReverseTurning) - DX_PI_F / 2);

		//近くじゃないときの行動
		if (m_difPSize > cNear)
		{
			WalkUpdate("Walk", 2);

			//攻撃してないときの処理
			if (!m_anim.s_attack)
			{
				//歩くアニメーション
				m_anim.s_moveflag = true;

				m_status.s_speed = cWalkSpeed;

				m_move = VScale(m_difPlayer, m_status.s_speed);
			}
		}
		//近くに行った時の行動
		else if (m_difPSize <= cNear)
		{
			m_status.s_speed = cRunSpeed;

			m_move = VNorm(m_difPlayer);

			m_move = VScale(m_move, m_status.s_speed);

			//ランダム行動で0が出た場合
			//左周り
			if (m_randomAction == 0)
			{
				//歩くモーションさせる
				m_anim.s_moveflag = true;

				m_move = VTransform(m_move, mtxR);

				WalkUpdate("LeftWalk", 3);

			}
			//ランダム行動で1が出た場合
			//右周り
			else if (m_randomAction == 1)
			{
				//歩くモーションさせる
				m_anim.s_moveflag = true;

				m_move = VTransform(m_move, mtx);

				WalkUpdate("RightWalk", 4);
			}
			//ランダム行動で2が出た場合
			else if (m_randomAction == 2)
			{
				//攻撃モーションさせる
				m_anim.s_attack = true;

				m_move = VGet(0.0f, 0.0f, 0.0f);

				AttackUpdate("Attack1", 5);

				m_anim.s_moveflag = false;
			}

		}

		//攻撃での判定
		if (m_randomAction == 2 && m_anim.s_attack)
		{
			InitAttackLigUpdate(attackRightHandPos1, attackRightHandPos2);

			//エフェクトを再生する
			if (m_nowFrame >= cAttackHitFrame && m_nowFrame <= cAttackEndFrame)
			{
				//攻撃の切っ先にエフェクトをつける
				cEffect.UpdateEffectPosition("EnemyAttack", VGet(attackRightHandPos2.x, attackRightHandPos2.y, attackRightHandPos2.z));
				cEffect.UpdateEffectRotation("EnemyAttack", VGet(0.0f, m_angle - (DX_PI_F / 2.0f), 0.0f));

			}


			if (m_nowFrame == cAttackStartFrame)
			{
				InitLigAttack(attackRightHandPos1, attackRightHandPos2, cAttackRadius);
				InitAttackDamage(m_status.s_attack);
			}
			//アニメーションフレーム中に攻撃判定を出す
			if (m_nowFrame == cAttackHitFrame)
			{
				//エフェクトを作る
				cEffect.EffectCreate("EnemyAttack", VGet(attackRightHandPos2.x, attackRightHandPos2.y, attackRightHandPos2.z));

				//攻撃SE再生
				PlaySoundMem(se.GetAttackSE(), DX_PLAYTYPE_BACK, true);

				InitAttackUpdate(m_status.s_attack);
			}
			else if (m_nowFrame >= cAttackEndFrame)
			{
				InitAttackDamage(0.0f);
				//判定をリセット
				m_pAttack->CollisionEnd();
				m_pLigAttack->CollisionEnd();
			}
		}


		//アニメーションが終わる度にランダムな行動を行う
		if (m_isAnimationFinish)
		{
			m_randomAction = GetRand(cRandomActionMax);
		}

		//移動方向
		m_moveVec = MyLibrary::LibVec3(m_move.x, m_move.y, m_move.z);
	}
	//プレイヤーを見失った時
	else if (m_pSearch->GetIsExit())
	{
		//アイドル状態にする
		IdleUpdate();
		//歩かないようにする
		m_anim.s_moveflag = false;
		//攻撃しないようにする
		m_anim.s_attack = false;
		//判定をリセット
		m_pAttack->CollisionEnd();
		m_pLigAttack->CollisionEnd();

		m_moveVec = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);
	}
}

/// <summary>
/// 描画
/// </summary>
void Immortal::Draw(EnemyWeapon& weapon)
{
	//当たり判定座標を取得してモデルの描画座標を設定する
	SetDrawModelPos(cModelPosY);
	//3Dモデルの回転地をセットする
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle, 0.0f));
	//モデルの描画
	MV1DrawModel(m_modelHandle);
	
}

/// <summary>
/// UI描画
/// </summary>
void Immortal::DrawUI(UI& ui)
{
	//戦闘中だと描画
	if (m_pSearch->GetIsStay())
	{
		//HPバーの描画
		ui.EnemyHPDraw(rigidbody->GetPos().GetVector(), m_status.s_hp, m_maxHP);
	}
}
