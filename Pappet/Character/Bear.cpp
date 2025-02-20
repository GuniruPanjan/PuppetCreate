#include "Bear.h"
#include "Ui/UI.h"
#include "Manager/EffectManager.h"
#include "Manager/SEManager.h"

namespace
{
	//キャラクター名
	constexpr const char* cCharacterName = "bear";
	//モデルパス
	constexpr const char* cModelPath = "Data/Enemy/BearModel.mv1";
	//モデルのサイズの拡大率
	constexpr float cModelSize = 0.7f;
	//モデルの長さ
	constexpr float cCapsuleLen = 40.0f;
	//モデルの半径
	constexpr float cCapsuleRadius = 25.0f;
	//モデルの座標を合わせる
	constexpr float cModelPosY = 24.0f;
	//死亡終了
	bool cDead = false;
	//死亡したときのアニメーション
	constexpr float cDeadFrame = 137.0f;
	//索敵範囲
	constexpr float cSearchRadius = 120.0f;
	//攻撃範囲1
	constexpr float cAttackRadius1 = 40.0f;
	//攻撃範囲2
	constexpr float cAttackRadius2 = 60.0f;
	//攻撃範囲3
	constexpr float cAttackRadius3 = 130.0f;

	int a = 0;
	int b = 0;
	int c = 0;

	//敵を回転させない
	bool cTurn = false;

	//シングルトン
	EffectManager& cEffect = EffectManager::GetInstance();

	bool cOne = false;
}

/// <summary>
/// コンストラクタ
/// </summary>
Bear::Bear() :
	EnemyBase(Collidable::Priority::High),
	m_attackPos(),
	m_walk(0)
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
Bear::~Bear()
{
	//メモリ解放
	MV1DeleteModel(m_modelHandle);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Bear::Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
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

	//索敵判定をする当たり判定を作成
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

	m_bossName = "熊の傀儡人形";
	m_subName = "H A R I B O";

	cOne = false;
	m_deadOne = false;

	m_walk = 0;

	m_bossjudg = true;
}

/// <summary>
/// ゲームの仕様上での初期化処理
/// </summary>
/// <param name="posX">X座標</param>
/// <param name="posY">Y座標</param>
/// <param name="posZ">Z座標</param>
/// <param name="physics">物理クラス</param>
void Bear::GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
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

	m_bossName = "熊の傀儡人形";
	m_subName = "H  A  R  I  B  O";

	cOne = false;
	m_deadOne = false;

	m_walk = 0;

	m_bossjudg = true;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="playerPos"></param>
/// <param name="isChase"></param>
void Bear::Update(MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 shieldPos, bool isChase, SEManager& se, std::shared_ptr<MyLibrary::Physics> physics)
{
	//視野の角度を決める
	m_viewAngle = 30.0f;
	//視野の距離を決める
	m_viewDistance = 500.0f;

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

	//歩いている時
	if (m_anim.s_moveflag)
	{
		m_walk++;

		if (m_walk >= 30)
		{
			//歩くSE再生
			PlaySoundMem(se.GetBossWalkSE(), DX_PLAYTYPE_BACK, true);

			m_walk = 0;
		}
	}
	else
	{
		m_walk = 0;
	}


	//攻撃を受けた時
	if (m_isEnterHit)
	{
		m_status.s_hp -= m_col->GetAttack() - m_status.s_defense;
		//Hitエフェクト
		cEffect.EffectCreate("Hit", VGet(rigidbody.GetPos().x, rigidbody.GetPos().y + 30.0f, rigidbody.GetPos().z));
		//HitSE再生
		PlaySoundMem(se.GetHitSE(), DX_PLAYTYPE_BACK, true);
	}
	//プレイヤーがボス部屋に入ったら
	if (m_isBossDiscovery && !cDead)
	{
		Action(playerPos, isChase, se);
	}

	TriggerUpdate();
	HitTriggerUpdate();

	//判定の更新
	MyLibrary::LibVec3 centerPos = rigidbody.GetPos();
	m_pSearch->Update(centerPos);

	//死んだ時
	if (m_status.s_hp <= 0.0f)
	{
		//アニメーションを初期化
		m_anim.s_attack = false;
		m_anim.s_hit = false;
		m_anim.s_moveflag = false;

		Death();

		if (m_nowFrame == 60)
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

/// <summary>
/// ボスの行動
/// </summary>
/// <param name="playerPos">プレイヤーのポジション</param>
/// <param name="isChase">プレイヤー</param>
void Bear::Action(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se)
{
	//敵がプレイヤーの位置によって方向を補正する
	float Cx = m_modelPos.x - playerPos.x;
	float Cz = m_modelPos.z - playerPos.z;

	m_correctionAngle = atan2f(Cx, Cz);
	//m_correctionAngle = NormalizeAngle(m_correctionAngle);

	if (m_correctionAngle >= 3.0f)
	{
		if (m_enemyAnim.s_turnRight)
		{
			int a = 1;
		}

		cTurn = true;
	}
	else if (m_correctionAngle <= -3.0f)
	{
		if (m_enemyAnim.s_turnLeft)
		{
			int a = 1;
		}

		cTurn = true;
	}
	else
	{
		cTurn = false;
		
	}
	//正面
	if (!IsPlayerInView(playerPos))
	{
		a = 1;
	}
	else
	{
		a = 0;
	}

	//右側
	if (IsPlayerOnRight(playerPos))
	{
		b = 1;
	}
	else
	{
		b = 0;
	}

	//左側
	if (IsPlayerOnLeft(playerPos))
	{
		c = 1;
	}
	else
	{
		c = 0;
	}

	
	//歩いている時は回転しなくなった
	if (!cTurn)
	{
		//左に回転する
		if (m_angle > m_correctionAngle + 0.8f)
		{
			m_enemyAnim.s_turnLeft = true;
			m_enemyAnim.s_turnRight = false;
			m_anim.s_moveflag = false;
		}
		//右に回転する
		if (m_angle < m_correctionAngle - 0.8f)
		{
			m_enemyAnim.s_turnRight = true;
			m_enemyAnim.s_turnLeft = false;
			m_anim.s_moveflag = false;
		}
		
	}
	

	//左回りしているとき
	if (m_enemyAnim.s_turnLeft)
	{
		//右回転させる
		if (m_angle < m_correctionAngle - 0.5f)
		{
			m_enemyAnim.s_turnRight = true;
			m_enemyAnim.s_turnLeft = false;
		}
	}
	//右回りしているとき
	else if (m_enemyAnim.s_turnRight)
	{
		//左回転させる
		if (m_angle > m_correctionAngle + 0.5f)
		{
			m_enemyAnim.s_turnLeft = true;
			m_enemyAnim.s_turnRight = false;
		}
	}

	//攻撃してない時
	if (!m_anim.s_attack)
	{
		if (!m_enemyAnim.s_turnLeft && !m_enemyAnim.s_turnRight)
		{
			//方向を決める
			AngleUpdate(playerPos);
		}
	}

	//攻撃してない時
	if (!m_anim.s_attack)
	{
		//動けないようにする
		m_moveVec = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);

		//左回転させる
		if (m_enemyAnim.s_turnLeft)
		{
			m_nowAnimIdx = m_animIdx["LeftWalk"];
			ChangeAnim(m_nowAnimIdx, m_animOne[6], m_animOne);

			//左回り
			if (m_angle > m_correctionAngle + 0.8f)
			{
				m_angle -= 0.05f;
			}

		}
		//右回転させる
		if (m_enemyAnim.s_turnRight)
		{
			m_nowAnimIdx = m_animIdx["RightWalk"];
			ChangeAnim(m_nowAnimIdx, m_animOne[7], m_animOne);

			//右回り
			if (m_angle < m_correctionAngle - 0.8f)
			{
				m_angle += 0.05f;
			}

		}
	}

	//プレイヤーが範囲外だった時
	if (!m_pSearch->GetIsStay() && !m_enemyAnim.s_turnLeft && !m_enemyAnim.s_turnRight)
	{
		WalkUpdate("Walk", 2);

		//攻撃してない時
		if (!m_anim.s_attack)
		{
			//歩くアニメーション
			m_anim.s_moveflag = true;
			//スピード
			m_status.s_speed = 0.01f;

			m_move = VScale(m_difPlayer, m_status.s_speed);

			//移動方向
			m_moveVec = MyLibrary::LibVec3(m_move.x, m_move.y, m_move.z);
		}
		
	}
	//射程圏内に入った
	else if (m_pSearch->GetIsStay() && !m_enemyAnim.s_turnLeft && !m_enemyAnim.s_turnRight)
	{
		//ランダム行動で0が出たら
		if (m_randomAction == 0)
		{
			m_anim.s_moveflag = false;
			m_anim.s_attack = true;

			AttackUpdate("Attack1", 3);
			//攻撃力
			//m_status.s_attack = 30.0f;
		}
		//ランダム行動で1が出たら
		else if (m_randomAction == 1)
		{
			m_anim.s_moveflag = false;
			m_anim.s_attack = true;

			AttackUpdate("Attack2", 4);
			//攻撃力
			//m_status.s_attack = 50.0f;

		}
		//ランダム行動で2が出たら
		else if (m_randomAction == 2)
		{
			m_anim.s_moveflag = false;
			m_anim.s_attack = true;

			AttackUpdate("Attack3", 5);
			//攻撃力
			//m_status.s_attack = 80.0f;
		}

		m_moveVec = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);
	}

	//攻撃での判定
	if (m_anim.s_attack)
	{
		//ランダムで0が出たら
		if (m_randomAction == 0)
		{
			//攻撃の初期化
			if (m_nowFrame == 5.0f)
			{
				//攻撃の初期化
				InitAttack(cAttackRadius1);

				InitAttackDamage(m_status.s_attack);
			}
			else if (m_nowFrame > 5.0f)
			{
				//攻撃判定の更新
				m_attackPos = MyLibrary::LibVec3(rigidbody.GetPos().x + sinf(m_angle) * -75.0f, rigidbody.GetPos().y, rigidbody.GetPos().z - cosf(m_angle) * 75.0f);
				m_pAttack->Update(m_attackPos);
			}

			//アニメーションフレーム中に攻撃判定を出す
			if (m_nowFrame == 7.0f)
			{
				//攻撃SE再生
				PlaySoundMem(se.GetBossAttackSE1(), DX_PLAYTYPE_BACK, true);

				InitAttackUpdate(m_status.s_attack);
			}
			else if (m_nowFrame >= 12.0f)
			{
				//判定をリセット
				m_pAttack->CollisionEnd();
			}

		}
		//ランダムで1が出たら
		else if (m_randomAction == 1)
		{
			//攻撃の初期化
			if (m_nowFrame == 5.0f)
			{
				//攻撃の初期化
				InitAttack(cAttackRadius2);

				InitAttackDamage(m_status.s_attack1);
			}
			else if (m_nowFrame > 5.0f)
			{
				//攻撃判定の更新
				m_attackPos = MyLibrary::LibVec3(rigidbody.GetPos().x + sinf(m_angle) * -75.0f, rigidbody.GetPos().y, rigidbody.GetPos().z - cosf(m_angle) * 75.0f);
				m_pAttack->Update(m_attackPos);
			}

			//攻撃発生まではプレイヤーを向く
			if (m_nowFrame > 0.1f && m_nowFrame < 38.0f)
			{
				AngleUpdate(playerPos);
			}
			//アニメーションフレーム宙に攻撃判定を出す
			else if (m_nowFrame == 38.0f)
			{
				//攻撃SE再生
				PlaySoundMem(se.GetBossAttackSE2(), DX_PLAYTYPE_BACK, true);

				InitAttackUpdate(m_status.s_attack1);
			}
			else if (m_nowFrame >= 45.0f)
			{
				//判定をリセット
				m_pAttack->CollisionEnd();
			}
		}
		//ランダムで2が出たら
		else if (m_randomAction == 2)
		{
			//攻撃の初期化
			if (m_nowFrame == 5.0f)
			{
				//攻撃の初期化
				InitAttack(cAttackRadius3);

				InitAttackDamage(m_status.s_attack2);

				//咆哮SE再生
				PlaySoundMem(se.GetBossVoiceSE(), DX_PLAYTYPE_BACK, true);
			}
			else if (m_nowFrame > 5.0f)
			{
				//攻撃判定の更新
				m_attackPos = MyLibrary::LibVec3(rigidbody.GetPos().x, rigidbody.GetPos().y, rigidbody.GetPos().z);
				m_pAttack->Update(m_attackPos);
			}

			//エフェクトを出す
			if (m_nowFrame == 25.0f)
			{
				//攻撃SE再生
				PlaySoundMem(se.GetBossAttackSE3(), DX_PLAYTYPE_BACK, true);

				cEffect.EffectCreate("BearLance", VGet(rigidbody.GetPos().x, rigidbody.GetPos().y - 25.0f, rigidbody.GetPos().z));
			}

			//アニメーションフレーム中に攻撃判定を出す
			if (m_nowFrame == 58.0f)
			{
				InitAttackUpdate(m_status.s_attack2);
			}
			else if (m_nowFrame >= 68.0f)
			{
				//判定をリセット
				m_pAttack->CollisionEnd();
			}
		}
	}

	//アニメーションが終わったら角度を入れる
	if (m_enemyAnim.s_turnLeft && m_isAnimationFinish && !m_anim.s_attack && !m_anim.s_moveflag)
	{
		m_angle = m_correctionAngle;
		m_enemyAnim.s_turnLeft = false;
	}
	//アニメーションが終わったら角度を入れる
	if (m_enemyAnim.s_turnRight && m_isAnimationFinish && !m_anim.s_attack && !m_anim.s_moveflag)
	{
		m_angle = m_correctionAngle;
		m_enemyAnim.s_turnRight = false;
	}

	//アニメーションが終わる度にランダムな行動を行う
	if (m_isAnimationFinish && m_anim.s_attack)
	{
		m_anim.s_attack = false;

		//近いときのランダム行動
		if (m_difPSize <= 80.0f)
		{
			m_randomAction = GetRand(2);
			//m_randomAction = 1;
		}
		//近くないときのランダム行動
		else
		{
			m_randomAction = GetRand(1) + 1;
			//m_randomAction = 1;
		}
		
	}

	if (!m_anim.s_attack)
	{
		InitAttackDamage(0.0f);
	}

}

/// <summary>
/// 描画処理
/// </summary>
void Bear::Draw(UI& ui)
{
	//当たり判定座標を取得してモデルの描画座標を設定する
	SetDrawModelPos(cModelPosY);
	//3Dモデルの回転地をセットする
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle, 0.0f));
	//モデルの描画
	MV1DrawModel(m_modelHandle);

	//DrawFormatString(200, 400, 0xffffff, "m_playerHit : %d", m_isPlayerHit);

	if (m_isBossDiscovery && !cDead)
	{
		ui.BossHPDraw(m_status.s_hp, m_maxHP, m_bossName, m_subName);
	}

	DrawFormatString(200, 300, 0xffffff, "%d", a);
	DrawFormatString(200, 400, 0xffffff, "右側 : %d", b);
	DrawFormatString(200, 500, 0xffffff, "左側 : %d", c);

#if false
	DrawFormatString(200, 300, 0xffffff, "m_angle : %f", m_angle);
	DrawFormatString(200, 350, 0xffffff, "m_correctionAngle : %f", m_correctionAngle);
	DrawFormatString(200, 400, 0xffffff, "m_left : %d", m_enemyAnim.s_turnLeft);
	DrawFormatString(200, 450, 0xffffff, "m_right : %d", m_enemyAnim.s_turnRight);
#endif
}
