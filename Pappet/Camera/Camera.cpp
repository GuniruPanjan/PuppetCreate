#include "Camera.h"
#include "Character/Player.h"
#include "Manager/EnemyManager.h"
#include "Ui/UI.h"
#include <algorithm>
#include <functional>
//度をラジアンに変換
#define D2R(deg) ((deg)*DX_PI_F/180.0f)

namespace
{
	VECTOR cPlayerPos = VGet(0.0f, 0.0f, 0.0f);

	int cRxl = 0;
	int cRxr = 0;

	int cSize = 0;
	int cTargetSize = 0;

	constexpr float cInitialRate = 1.1f;
	constexpr float cInitialRadius = 3.0f;
	constexpr float cInitialCameraPosY = 50.0f;
	constexpr float cInitialCameraPosZ = 100.0f;
	constexpr float cInitialCameraTargetY = 5.0f;
	constexpr float cInitialCameraAngleX = D2R(-20.0f);
	constexpr float cInitialCameraAngleY = 97.4f;
	constexpr float cCameraNear = 0.001f;
	constexpr float cCameraFar = 1000.0f;
	constexpr float cRateIncrement = 0.01f;
	constexpr float cRateMax = 1.3f;
	constexpr float cCameraAngleIncrement = D2R(2.0f);
	constexpr float cCameraPosYMin = 15.2f;
	constexpr float cCameraAngleXMax = 0.7f;
	constexpr float cDirectionY = 112.0f;
	constexpr float cDirectionZ = -112.0f;
	constexpr int cInputThreshold = 5;
	constexpr float cFilterRange = 200.0f;
	constexpr float cEndTargetPosY = 20.0f;
	constexpr float cPosTargetXMultiplier = 130.0f;
	constexpr float cPosTargetZMultiplier = 130.0f;
	constexpr float cPosTargetY = 80.0f;
	constexpr float cEasingIncrement = 0.016f;
	constexpr float cDrawRotaGraphX = 800.0f;
	constexpr float cDrawRotaGraphY = 450.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera() :
	input(),
	m_cameraPos(VGet(0.0f,0.0f,0.0f)),
	m_cameraTarget(VGet(0.0f, 0.0f, 0.0f)),
	m_cameraAngle(VGet(0.0f, 0.0f, 0.0f)),
	m_x(0.0f),
	m_z(0.0f),
	m_radius(0.0f),
	m_easingTime(0.0f),
	m_easingDuration(1.0f),
	m_currentTargetIndex(0),
	m_rate(0.0f),
	m_isRate(true)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{
}

/// <summary>
/// 初期化処理
/// </summary>
void Camera::Init()
{
	m_rate = cInitialRate;

	m_radius = cInitialRadius;

	//基準となるカメラの座標
	m_cameraPos = VGet(0.0f, cInitialCameraPosY, cInitialCameraPosZ);

	//カメラのターゲット座標初期化
	m_cameraTarget = VGet(0.0f, cInitialCameraTargetY, 0.0f);

	//カメラのアングル初期設定
	m_cameraAngle = VGet(cInitialCameraAngleX, cInitialCameraAngleY, 0.0f);

	SetCameraNearFar(cCameraNear, cCameraFar);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="player">プレイヤーを呼び出す</param>
void Camera::Update(Player& player)
{
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	//判定
	if (m_rate <= 1.0f)
	{
		m_isRate = true;
	}
	else if (m_rate >= cRateMax)
	{
		m_isRate = false;
	}
	//拡大させる
	if (m_isRate)
	{
		m_rate += cRateIncrement;
	}
	else
	{
		m_rate -= cRateIncrement;
	}

	if (!player.GetLock())
	{
		m_easingTime = 0.0f;

		cPlayerPos = player.GetPos().GetVector();

		//左キー
		if (input.Rx < 0)
		{
			m_cameraAngle.y -= cCameraAngleIncrement;
		}
		//右キー
		if (input.Rx > 0)
		{
			m_cameraAngle.y += cCameraAngleIncrement;
		}
		//上キー
		if (input.Ry < 0)
		{
			//カメラが地面にめりこまないように
			if (m_cameraPos.y >= cCameraPosYMin + player.GetPos().y)
			{
				m_cameraAngle.x -= cCameraAngleIncrement;
			}
		}
		//下キー
		if (input.Ry > 0)
		{
			//カメラがプレイヤーを超えないくらいまで
			if (m_cameraAngle.x <= cCameraAngleXMax)
			{
				m_cameraAngle.x += cCameraAngleIncrement;
			}
		}

		//基準のベクトル
		VECTOR Direction = VGet(0.0f, cDirectionY, cDirectionZ);

		//X軸回転行列
		MATRIX matrixX = MGetRotX(m_cameraAngle.x);
		//Y軸回転行列
		MATRIX matrixY = MGetRotY(m_cameraAngle.y);

		//行列の合成
		MATRIX matrix = MMult(matrixX, matrixY);

		//基準ベクトルを行列で変換
		Direction = VTransform(Direction, matrix);

		//カメラ座標はプレイヤー座標から少しはなれたところ
		m_cameraPos = VAdd(cPlayerPos, Direction);

		//注視点の座標はプレイヤー座標の少し上
		m_cameraTarget = VAdd(cPlayerPos, VGet(0.0f, cInitialCameraPosY, 0.0f));

		SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);
	}
	else
	{
		//ターゲットの選択
		if (input.Rx < 0)
		{
			cRxl++;
			if (cRxl == cInputThreshold)
			{
				SelectNextTarget();
			}
		}
		else
		{
			cRxl = 0;
		}

		if (input.Rx > 0)
		{
			cRxr++;
			if (cRxr == cInputThreshold)
			{
				SelectPreviousTarget();
			}
		}
		else
		{
			cRxr = 0;
		}

		SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);
	}
}

void Camera::SelectNextTarget()
{
	if (!m_enemyPositions.empty())
	{
		m_currentTargetIndex = (m_currentTargetIndex + 1) % m_enemyPositions.size();
		m_easingTime = 0.0f;
	}
}

void Camera::SelectPreviousTarget()
{
	if (!m_enemyPositions.empty())
	{
		m_currentTargetIndex = (m_currentTargetIndex - 1 + m_enemyPositions.size()) % m_enemyPositions.size();
		m_easingTime = 0.0f;
	}
}

void Camera::FilterEnemiesInRange(Player& player, EnemyManager& enemy, float range)
{
	m_filterEnemyPositions.clear();
	VECTOR playerPos = player.GetPos().GetVector();

	int index = 0;
	for (const auto& enemyPos : m_enemyPositions)
	{
		VECTOR enemyVector = VGet(enemyPos.x, enemyPos.y, enemyPos.z);
		float distance = VSize(VSub(playerPos, enemyVector));

		if (distance <= range && !enemy.m_enemyIsDead[index])
		{
			m_filterEnemyPositions.push_back(enemyPos);
		}
		index++;
	}
}

/// <summary>
/// ロックオン処理
/// </summary>
/// <param name="player">プレイヤーを呼び出す</param>
/// <param name="enemy">エネミーを呼び出す</param>
void Camera::LockUpdate(Player& player, EnemyManager& enemy)
{
	m_enemyPositions.clear();
	float minDistance = FLT_MAX;

	for (const auto& enemyPos : enemy.GetEnemyPos())
	{
		m_enemyPositions.push_back(enemyPos);
	}

	if (m_enemyPositions.empty())
	{
		return;
	}

	//フィルタリングされた敵のリストを使用
	FilterEnemiesInRange(player, enemy, cFilterRange);

	if (m_filterEnemyPositions.empty())
	{
		return;
	}

	m_currentTargetIndex = min(m_currentTargetIndex, static_cast<int>(m_filterEnemyPositions.size()) - 1);
	m_enemyPos = m_filterEnemyPositions[m_currentTargetIndex].GetVector();

	VECTOR pPos = VGet(player.GetPos().x, player.GetPos().y, player.GetPos().z);

	//注視点は敵の座標にする
	m_endTargetPos = VAdd(m_enemyPos, VGet(0.0f, cEndTargetPosY, 0.0f));

	//プレイヤーとエネミーのX座標の差を求める
	float X = m_enemyPos.x - pPos.x;
	float lockX = pPos.x - m_enemyPos.x;

	//プレイヤーとエネミーのZ座標の差を求める
	float Z = m_enemyPos.z - pPos.z;
	float lockZ = pPos.z - m_enemyPos.z;

	//角度を出す
	float angle = atan2f(X, Z);

	//プレイヤーの方向も変える
	player.SetAngle(atan2f(lockX, lockZ));

	//敵からプレイヤーに伸びる基準のベクトルを求める
	VECTOR pos = VSub(pPos, m_enemyPos);

	//ベクトルの正規化
	VECTOR posTarget = VNorm(pos);

	posTarget.x *= cPosTargetXMultiplier;
	posTarget.z *= cPosTargetZMultiplier;

	//カメラがどれだけプレイヤーの座標より高いかを設定
	posTarget.y = cPosTargetY;

	m_cameraAngle.y = angle;

	//プレイヤーの座標に求めたベクトルを足してカメラの座標とする
	m_endPos = VAdd(pPos, posTarget);

	//イージングの更新
	if (m_easingTime < m_easingDuration)
	{
		m_easingTime += cEasingIncrement;
		float t = m_easingTime / m_easingDuration;
		m_cameraPos = m_lerp.Lerp(MyLibrary::LibVec3(m_cameraPos.x, m_cameraPos.y, m_cameraPos.z), MyLibrary::LibVec3(m_endPos.x, m_endPos.y, m_endPos.z), t).GetVector();
		m_cameraTarget = m_lerpTarget.Lerp(MyLibrary::LibVec3(m_cameraTarget.x, m_cameraTarget.y, m_cameraTarget.z), MyLibrary::LibVec3(m_endTargetPos.x, m_endTargetPos.y, m_endTargetPos.z), t).GetVector();
	}
	else
	{
		m_cameraTarget = VAdd(m_enemyPos, VGet(0.0, cEndTargetPosY, 0.0f));
		m_cameraPos = VAdd(pPos, posTarget);
	}

	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);
}

void Camera::LockBossUpdate(Player& player, EnemyManager& enemy)
{
	cTargetSize = 0;
	cSize = 0;

	for (auto& enemyTarget : enemy.GetJudg())
	{
		cTargetSize++;

		if (enemyTarget == true)
		{
			for (auto& enemyPos : enemy.GetEnemyPos())
			{
				cSize++;

				if (cTargetSize == cSize)
				{
					//ここがターゲットになるやつ
					m_enemyPos = VGet(enemyPos.x, enemyPos.y, enemyPos.z);

					break;
				}
			}
		}
	}

	VECTOR pPos = VGet(player.GetPos().x, player.GetPos().y, player.GetPos().z);

	//注視点は敵の座標にする
	m_endTargetPos = VAdd(m_enemyPos, VGet(0.0f, cEndTargetPosY, 0.0f));

	//プレイヤーとエネミーのX座標の差を求める
	float X = m_enemyPos.x - pPos.x;
	float lockX = pPos.x - m_enemyPos.x;

	//プレイヤーとエネミーのZ座標の差を求める
	float Z = m_enemyPos.z - pPos.z;
	float lockZ = pPos.z - m_enemyPos.z;

	//角度を出す
	float angle = atan2f(X, Z);

	//プレイヤーの方向も変える
	player.SetAngle(atan2f(lockX, lockZ));

	//敵からプレイヤーに伸びる基準のベクトルを求める
	VECTOR pos = VSub(pPos, m_enemyPos);

	//ベクトルの正規化
	VECTOR posTarget = VNorm(pos);

	posTarget.x *= cPosTargetXMultiplier;
	posTarget.z *= cPosTargetZMultiplier;

	//カメラがどれだけプレイヤーの座標より高いかを設定
	posTarget.y = cPosTargetY;

	m_cameraAngle.y = angle;

	//プレイヤーの座標に求めたベクトルを足してカメラの座標とする
	m_endPos = VAdd(pPos, posTarget);

	//イージングの更新
	if (m_easingTime < m_easingDuration)
	{
		m_easingTime += cEasingIncrement;
		float t = m_easingTime / m_easingDuration;
		m_cameraPos = m_lerp.Lerp(MyLibrary::LibVec3(m_cameraPos.x, m_cameraPos.y, m_cameraPos.z), MyLibrary::LibVec3(m_endPos.x, m_endPos.y, m_endPos.z), t).GetVector();
		m_cameraTarget = m_lerpTarget.Lerp(MyLibrary::LibVec3(m_cameraTarget.x, m_cameraTarget.y, m_cameraTarget.z), MyLibrary::LibVec3(m_endTargetPos.x, m_endTargetPos.y, m_endTargetPos.z), t).GetVector();
	}
	else
	{
		m_cameraTarget = VAdd(m_enemyPos, VGet(0.0, cEndTargetPosY, 0.0f));
		m_cameraPos = VAdd(pPos, posTarget);
	}

	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);
}

/// <summary>
/// 描画処理
/// </summary>
void Camera::Draw(UI& ui)
{
	DrawRotaGraph(cDrawRotaGraphX, cDrawRotaGraphY, m_rate, 0.0f, ui.GetLockUI(), true);
}

/// <summary>
/// 終了処理
/// </summary>
void Camera::End()
{
}

/// <summary>
/// カメラが向いてる方向ベクトルを取得
/// </summary>
/// <returns>カメラが向いてる方向</returns>
const MyLibrary::LibVec3 Camera::GetDirection() const
{
	return (MyLibrary::LibVec3(m_cameraTarget.x, m_cameraTarget.y, m_cameraTarget.z)
		- MyLibrary::LibVec3(m_cameraPos.x, m_cameraPos.y, m_cameraPos.z)).Normalize();
}
