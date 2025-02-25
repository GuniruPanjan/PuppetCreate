#include "Assassin.h"
#include "Ui/UI.h"
#include "Manager/EffectManager.h"
#include "Manager/SEManager.h"

namespace
{
	//キャラクター名
	constexpr const char* cCharacterName = "Assassin";
	//モデルパス
	constexpr const char* cModelPath = "Data/Enemy/Assassin.mv1";
	//チュートリアル判定
	bool cTutorial;
	//モデルのサイズの拡大率
	constexpr float cModelSize = 0.5f;
	//モデルの長さ
	constexpr float cCapsuleLen = 40.0f;
	//モデルの半径
	constexpr float cCapsuleRadius = 14.0f;
	//モデルの座標を合わせる
	constexpr float cModelPosY = 12.0f;
	//死亡終了
	bool cDead = false;
	//死亡したときのアニメーション
	constexpr float cDeadFrame = 68.0f;
	//索敵範囲
	constexpr float cSearchRadius = 120.0f;
	//近距離の行動に移る距離
	constexpr float cNear = 100.0f;
	//攻撃範囲(ナイフ)
	constexpr float cAttackRadiusKnife = 10.0f;
	//攻撃範囲(蹴り)
	constexpr float cAttackRadiusKick = 15.0f;
	//視野の角度
	constexpr float cAngle = 30.0f;
	//視野の距離
	constexpr float cAngleDistance = 400.0f;
	//プレイヤーを発見したとき
	bool cPlayerLook = false;

	//シングルトン
	EffectManager& cEffect = EffectManager::GetInstance();

	bool cOne = false;
}

/// <summary>
/// コンストラクタ
/// </summary>
Assassin::Assassin():
	EnemyBase(Collidable::Priority::High),
	m_attackPos(),
	m_moveFrameRightHand(0),
	m_moveFrameLeftLeg(0),
	m_moveFrameRightLeg(0),
	m_frameRightHand(VGet(0.0f,0.0f,0.0f)),
	m_frameLeftLeg(VGet(0.0f,0.0f,0.0f)),
	m_frameRightLeg(VGet(0.0f,0.0f,0.0f))
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
void Assassin::Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial)
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
		m_bossName = " ? ? ? ";
		m_subName = " ? ? ? ";

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
void Assassin::GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial)
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

	//アニメーション設定
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];

	m_anim.s_isDead = false;
	cDead = false;

	//最大HPを取得
	m_maxHP = m_status.s_hp;

	if (cTutorial)
	{
		m_bossName = " ? ? ? ";
		m_subName = " ? ? ? ";

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
void Assassin::Update(MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 shieldPos, bool isChase, SEManager& se, std::shared_ptr<MyLibrary::Physics> physics)
{
	//アニメーションで移動しているフレームの番号を検索する
	m_moveFrameRightHand = MV1SearchFrame(m_modelHandle, "mixamorig:RightHandThumb2");
	m_moveFrameLeftLeg = MV1SearchFrame(m_modelHandle, "mixamorig:LeftToeBase");
	m_moveFrameRightLeg = MV1SearchFrame(m_modelHandle, "mixamorig:RightToeBase");

	//視野の角度を決める
	m_viewAngle = cAngle;
	//視野の距離を決める
	m_viewDistance = cAngleDistance;

	//アニメーションの更新
	if (!cDead)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}
	//死亡したときのアニメーション更新
	else if (cDead && m_nowFrame <= cDeadFrame)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}

	UpdateAnimationBlend();

	DistanceUpdate(playerPos, shieldPos);
	AttackDistance();

	//怯みモーション
	HitUpdate(10);

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

	//移動処理
	MoveUpdate();

	//ターゲット状態
	TargetNow();

	//攻撃していないとき
	if (!m_anim.s_attack)
	{
		if (!cOne)
		{
			InitAttack(0.0f);
			InitAttackDamage(0.0f);

			cOne = true;
		}

	}

	//攻撃を受けた時
	if (m_isEnterHit)
	{
		m_status.s_hp -= m_col->GetAttack() - m_status.s_defense;
		//Hitエフェクト
		cEffect.EffectCreate("Hit", VGet(rigidbody.GetPos().x, rigidbody.GetPos().y + 30.0f, rigidbody.GetPos().z));
		//HitSE再生
		PlaySoundMem(se.GetHitSE(), DX_PLAYTYPE_BACK, true);

		//HPが0になるとヒットしない
		if (m_status.s_hp > 0.0f)
		{
			m_anim.s_hit = true;
		}
	
	
	}

	if (cTutorial)
	{
		//プレイヤーがボス部屋に入ったら
		if (m_isBossDiscovery && !cDead && !m_anim.s_hit)
		{
			BossAction(playerPos, isChase, se);
		}
	}
	else
	{
		//怯んでない時
		if (!m_anim.s_hit)
		{
			Action(playerPos, isChase, se);
		}
	}

	TriggerUpdate();
	HitTriggerUpdate();

	//判定の更新
	MyLibrary::LibVec3 centerPos = rigidbody.GetPos();
	m_pSearch->Update(centerPos);

	//チュートリアルだった場合
	if (cTutorial)
	{
		//死んだ時
		if (m_status.s_hp <= 0.0f)
		{
			//アニメーションを初期化
			m_anim.s_attack = false;
			m_anim.s_hit = false;
			m_anim.s_moveflag = false;

			Death();

			if (m_nowFrame == 36)
			{
				//死亡SE再生
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
		//死んだとき
		if (m_status.s_hp <= 0.0f)
		{
			//アニメーションを初期化
			m_anim.s_attack = false;
			m_anim.s_hit = false;
			m_anim.s_moveflag = false;

			Death();

			if (m_nowFrame == 36)
			{
				//死亡SE再生
				PlaySoundMem(se.GetDiedSE(), DX_PLAYTYPE_BACK, true);
			}


			cDead = true;
		}
	}
	
}

void Assassin::Action(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se)
{
	m_frameRightHand = MV1GetFramePosition(m_modelHandle, m_moveFrameRightHand);
	m_frameLeftLeg = MV1GetFramePosition(m_modelHandle, m_moveFrameLeftLeg);
	m_frameRightLeg = MV1GetFramePosition(m_modelHandle, m_moveFrameRightLeg);

	//判定の更新
	MyLibrary::LibVec3 attackKnifePos = MyLibrary::LibVec3(m_frameRightHand.x, m_frameRightHand.y, m_frameRightHand.z);
	MyLibrary::LibVec3 attackLeftKickPos = MyLibrary::LibVec3(m_frameLeftLeg.x, m_frameLeftLeg.y, m_frameLeftLeg.z);
	MyLibrary::LibVec3 attackRightKickPos = MyLibrary::LibVec3(m_frameRightLeg.x, m_frameRightLeg.y, m_frameRightLeg.z);

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
		//攻撃してない時
		if (!m_anim.s_attack)
		{
			//方向を決める
			AngleUpdate(playerPos);
		}

		//近くじゃない時の行動
		if (m_difPSize > cNear)
		{
			WalkUpdate("Walk", 2);

			//攻撃してないときの処理
			if (!m_anim.s_attack)
			{
				//歩くアニメーション
				m_anim.s_moveflag = true;

				m_status.s_speed = 0.02f;

				m_move = VScale(m_difPlayer, m_status.s_speed);

				m_enemyAnim.s_rool = false;

			}
		}
		//近くに行った時の行動
		else if (m_difPSize <= cNear)
		{
			//ランダム行動で0が出た場合
			//攻撃1
			if (m_randomAction == 0)
			{
				//攻撃判定のポジション
				m_pAttack->Update(attackKnifePos);

				//攻撃モーションさせる
				m_anim.s_attack = true;

				m_move = VGet(0.0f, 0.0f, 0.0f);

				AttackUpdate("Attack1", 3);

				if (m_nowFrame == 5)
				{
					InitAttack(cAttackRadiusKnife);
					InitAttackDamage(m_status.s_attack);
				}
				//アニメーションフレーム中に攻撃判定を出す
				if (m_nowFrame == 24)
				{
					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame == 30)
				{
					InitAttackDamage(0.0f);
					//判定をリセット
					m_pAttack->CollisionEnd();
				}
				//アニメーションフレーム中に攻撃判定を出す
				if (m_nowFrame == 40)
				{
					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame >= 48)
				{
					InitAttackDamage(0.0f);
					//判定をリセット
					m_pAttack->CollisionEnd();
				}

				m_enemyAnim.s_rool = false;
				m_anim.s_moveflag = false;

			}
			//ランダム行動で1が出た場合
			//攻撃2
			else if (m_randomAction == 1)
			{
				//攻撃判定のポジション
				m_pAttack->Update(attackKnifePos);

				//攻撃モーションさせる
				m_anim.s_attack = true;

				m_move = VGet(0.0f, 0.0f, 0.0f);

				AttackUpdate("Attack2", 4);

				if (m_nowFrame == 5)
				{
					InitAttack(cAttackRadiusKnife);
					InitAttackDamage(m_status.s_attack);
				}
				//アニメーションフレーム中に攻撃判定を出す
				if (m_nowFrame == 24)
				{
					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame >= 28)
				{
					InitAttackDamage(0.0f);
					//判定をリセット
					m_pAttack->CollisionEnd();
				}

				m_enemyAnim.s_rool = false;
				m_anim.s_moveflag = false;
			}
			//ランダム行動で2が出た場合
			//攻撃3
			else if (m_randomAction == 2)
			{
				//攻撃判定のポジション
				m_pAttack->Update(attackKnifePos);

				//攻撃モーションさせる
				m_anim.s_attack = true;

				m_move = VGet(0.0f, 0.0f, 0.0f);

				AttackUpdate("Attack3", 5);

				if (m_nowFrame == 5)
				{
					InitAttack(cAttackRadiusKnife);
					InitAttackDamage(m_status.s_attack);
				}
				//アニメーションフレーム中に攻撃判定を出す
				if (m_nowFrame == 11)
				{
					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame >= 18)
				{
					InitAttackDamage(0.0f);
					//判定をリセット
					m_pAttack->CollisionEnd();
				}

				m_enemyAnim.s_rool = false;
				m_anim.s_moveflag = false;
			}
			//ランダム行動で3が出た場合
			//攻撃4
			else if (m_randomAction == 3)
			{
				//攻撃判定のポジション
				m_pAttack->Update(attackLeftKickPos);

				//攻撃モーションさせる
				m_anim.s_attack = true;

				m_move = VGet(0.0f, 0.0f, 0.0f);

				AttackUpdate("Attack4", 6);

				if (m_nowFrame == 5)
				{
					InitAttack(cAttackRadiusKick);
					InitAttackDamage(m_status.s_attack);
				}
				//アニメーションフレーム中に攻撃判定を出す
				if (m_nowFrame == 62)
				{
					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame >= 70)
				{
					InitAttackDamage(0.0f);
					//判定をリセット
					m_pAttack->CollisionEnd();
				}

				m_enemyAnim.s_rool = false;
				m_anim.s_moveflag = false;
			}
			//ランダム行動で4が出た場合
			//攻撃5
			else if (m_randomAction == 4)
			{
				//攻撃判定のポジション
				m_pAttack->Update(attackRightKickPos);

				//攻撃モーションさせる
				m_anim.s_attack = true;

				m_move = VGet(0.0f, 0.0f, 0.0f);

				AttackUpdate("Attack5", 7);

				if (m_nowFrame == 5)
				{
					InitAttack(cAttackRadiusKick);
					InitAttackDamage(m_status.s_attack);
				}
				//アニメーションフレーム中に攻撃判定を出す
				if (m_nowFrame == 25)
				{
					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame >= 35)
				{
					InitAttackDamage(0.0f);
					//判定をリセット
					m_pAttack->CollisionEnd();
				}

				m_enemyAnim.s_rool = false;
				m_anim.s_moveflag = false;
			}
			//ランダム行動で5が出た場合
			//回避
			else if (m_randomAction == 5)
			{
				m_enemyAnim.s_rool = true;
			}
		}

		//アニメーションが終わる度にランダムな行動を行う
		if (m_isAnimationFinish)
		{
			if (cPlayerLook)
			{
				m_randomAction = GetRand(6);
			}
			else
			{
				//回避するようにする
				m_randomAction = 5;
			}
		}

		//移動方向
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
void Assassin::BossAction(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se)
{
	m_frameRightHand = MV1GetFramePosition(m_modelHandle, m_moveFrameRightHand);
	m_frameLeftLeg = MV1GetFramePosition(m_modelHandle, m_moveFrameLeftLeg);
	m_frameRightLeg = MV1GetFramePosition(m_modelHandle, m_moveFrameRightLeg);

	//判定の更新
	MyLibrary::LibVec3 attackKnifePos = MyLibrary::LibVec3(m_frameRightHand.x, m_frameRightHand.y, m_frameRightHand.z);
	MyLibrary::LibVec3 attackLeftKickPos = MyLibrary::LibVec3(m_frameLeftLeg.x, m_frameLeftLeg.y, m_frameLeftLeg.z);
	MyLibrary::LibVec3 attackRightKickPos = MyLibrary::LibVec3(m_frameRightLeg.x, m_frameRightLeg.y, m_frameRightLeg.z);

	//正面
	if (!IsPlayerInView(playerPos))
	{
		cPlayerLook = true;
	}
	else
	{
		cPlayerLook = false;
	}

	//攻撃してない時
	if (!m_anim.s_attack)
	{
		//方向を決める
		AngleUpdate(playerPos);
	}

	//近くじゃない時の行動
	if (m_difPSize > cNear)
	{
		WalkUpdate("Walk", 2);

		//攻撃してないときの処理
		if (!m_anim.s_attack)
		{
			//歩くアニメーション
			m_anim.s_moveflag = true;

			m_status.s_speed = 0.02f;

			m_move = VScale(m_difPlayer, m_status.s_speed);
			
			m_enemyAnim.s_rool = false;

		}
	}
	//近くに行った時の行動
	else if (m_difPSize <= cNear)
	{
		//ランダム行動で0が出た場合
		//攻撃1
		if (m_randomAction == 0)
		{
			//攻撃判定のポジション
			m_pAttack->Update(attackKnifePos);

			//攻撃モーションさせる
			m_anim.s_attack = true;

			m_move = VGet(0.0f, 0.0f, 0.0f);

			AttackUpdate("Attack1", 3);

			if (m_nowFrame == 5)
			{
				InitAttack(cAttackRadiusKnife);
				InitAttackDamage(m_status.s_attack);
			}
			//アニメーションフレーム中に攻撃判定を出す
			if (m_nowFrame == 24)
			{
				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame == 30)
			{
				InitAttackDamage(0.0f);
				//判定をリセット
				m_pAttack->CollisionEnd();
			}
			//アニメーションフレーム中に攻撃判定を出す
			if (m_nowFrame == 40)
			{
				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame >= 48)
			{
				InitAttackDamage(0.0f);
				//判定をリセット
				m_pAttack->CollisionEnd();
			}

			m_enemyAnim.s_rool = false;
			m_anim.s_moveflag = false;

		}
		//ランダム行動で1が出た場合
		//攻撃2
		else if (m_randomAction == 1)
		{
			//攻撃判定のポジション
			m_pAttack->Update(attackKnifePos);

			//攻撃モーションさせる
			m_anim.s_attack = true;

			m_move = VGet(0.0f, 0.0f, 0.0f);

			AttackUpdate("Attack2", 4);

			if (m_nowFrame == 5)
			{
				InitAttack(cAttackRadiusKnife);
				InitAttackDamage(m_status.s_attack);
			}
			//アニメーションフレーム中に攻撃判定を出す
			if (m_nowFrame == 24)
			{
				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame >= 28)
			{
				InitAttackDamage(0.0f);
				//判定をリセット
				m_pAttack->CollisionEnd();
			}

			m_enemyAnim.s_rool = false;
			m_anim.s_moveflag = false;
		}
		//ランダム行動で2が出た場合
		//攻撃3
		else if (m_randomAction == 2)
		{
			//攻撃判定のポジション
			m_pAttack->Update(attackKnifePos);

			//攻撃モーションさせる
			m_anim.s_attack = true;

			m_move = VGet(0.0f, 0.0f, 0.0f);

			AttackUpdate("Attack3", 5);

			if (m_nowFrame == 5)
			{
				InitAttack(cAttackRadiusKnife);
				InitAttackDamage(m_status.s_attack);
			}
			//アニメーションフレーム中に攻撃判定を出す
			if (m_nowFrame == 11)
			{
				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame >= 18)
			{
				InitAttackDamage(0.0f);
				//判定をリセット
				m_pAttack->CollisionEnd();
			}

			m_enemyAnim.s_rool = false;
			m_anim.s_moveflag = false;
		}
		//ランダム行動で3が出た場合
		//攻撃4
		else if (m_randomAction == 3)
		{
			//攻撃判定のポジション
			m_pAttack->Update(attackLeftKickPos);

			//攻撃モーションさせる
			m_anim.s_attack = true;

			m_move = VGet(0.0f, 0.0f, 0.0f);

			AttackUpdate("Attack4", 6);

			if (m_nowFrame == 5)
			{
				InitAttack(cAttackRadiusKick);
				InitAttackDamage(m_status.s_attack);
			}
			//アニメーションフレーム中に攻撃判定を出す
			if (m_nowFrame == 62)
			{
				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame >= 70)
			{
				InitAttackDamage(0.0f);
				//判定をリセット
				m_pAttack->CollisionEnd();
			}

			m_enemyAnim.s_rool = false;
			m_anim.s_moveflag = false;
		}
		//ランダム行動で4が出た場合
		//攻撃5
		else if (m_randomAction == 4)
		{
			//攻撃判定のポジション
			m_pAttack->Update(attackRightKickPos);

			//攻撃モーションさせる
			m_anim.s_attack = true;

			m_move = VGet(0.0f, 0.0f, 0.0f);

			AttackUpdate("Attack5", 7);

			if (m_nowFrame == 5)
			{
				InitAttack(cAttackRadiusKick);
				InitAttackDamage(m_status.s_attack);
			}
			//アニメーションフレーム中に攻撃判定を出す
			if (m_nowFrame == 25)
			{
				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame >= 35)
			{
				InitAttackDamage(0.0f);
				//判定をリセット
				m_pAttack->CollisionEnd();
			}

			m_anim.s_moveflag = false;
			m_enemyAnim.s_rool = false;
		}
		//ランダム行動で5が出た場合
		//回避
		else if (m_randomAction == 5)
		{
			m_enemyAnim.s_rool = true;
		}
	}

	//アニメーションが終わる度にランダムな行動を行う
	if (m_isAnimationFinish)
	{
		if (cPlayerLook)
		{
			m_randomAction = GetRand(6);
		}
		else
		{
			//回避するようにする
			m_randomAction = 5;
		}
	}

	//移動方向
	m_moveVec = MyLibrary::LibVec3(m_move.x, m_move.y, m_move.z);
}

void Assassin::Draw(UI& ui)
{
	//当たり判定座標を取得してモデルの描画座標を設定する
	SetDrawModelPos(cModelPosY);
	//3Dモデルの回転地をセットする
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle, 0.0f));
	//モデルの描画
	MV1DrawModel(m_modelHandle);

	if (m_isBossDiscovery && !cDead)
	{
		ui.BossHPDraw(m_status.s_hp, m_maxHP, m_bossName, m_subName);
	}
}
