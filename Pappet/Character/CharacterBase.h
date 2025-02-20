#pragma once
#include "Library/MyLibrary.h"
#include "Object/AttackObject.h"
#include "Object/SearchObject.h"
#include "Object/PlayerSearchObject.h"
#include "Object/EnemyAttackObject.h"
#include "Object/ShieldObject.h"
#include <map>
#include <string>
#define ANIMATION_MAX   30
#define D2R(deg) ((deg)*DX_PI_F/180.0f)

class CharacterBase : public MyLibrary::Collidable
{
public:
	//ステータス構造体
	struct Status
	{
		float s_hp;          //体力
		float s_stamina;     //スタミナ
		float s_attack;      //攻撃力
		float s_attack1;     //攻撃力2
		float s_attack2;     //攻撃力
		int s_muscle;        //筋力
		int s_skill;         //技量
		float s_defense;     //防御力
		float s_speed;       //速度
		int s_core;          //コア
	};

	//アニメーション関係の構造体
	struct AnimationChange
	{
		bool s_moveflag;      //キャラが動いたか判断するフラグ
		bool s_isDead;        //キャラの死亡判定
		bool s_hit;           //怯み判定
		bool s_attack;        //キャラが攻撃したかの判定
	};

	//エフェクト関係の構造体
	struct EffectAction
	{
		bool s_heel;          //回復
	};

public:
	CharacterBase(Priority priority, ObjectTag tag);
	virtual ~CharacterBase();

	//衝突したとき
	virtual void OnCollideEnter(const std::shared_ptr<Collidable>& colider) = 0;

protected:
	//アニメーションの更新
	bool UpdateAnim(int attachNo, int max, float startTime = 0.0f);
	//アニメーションの変更
	void ChangeAnim(int animIndex, bool& one, bool(&all)[30], float animSpeed = 0.5f, bool reverse = false, float resetTime = 0.0f);
	//アニメーションのフレームブレンド変更
	void FrameChangeAnim(int animIndex, bool& one, bool& two, int frame);
	//アニメーションのフレームブレンド変更
	void FrameEndAnim(int animIndex, bool& one, bool& two, int frame);
	//アニメーションの未初期化
	void NotInitAnim(bool init = false);


protected:
	//物理クラスのポインタ
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;
	//モデルハンドル
	int m_modelHandle;
	//モデル座標
	MyLibrary::LibVec3 m_modelPos;
	//当たり判定座標
	MyLibrary::LibVec3 m_collisionPos;
	//移動ベクトル
	MyLibrary::LibVec3 m_moveVec;
	//ステータス
	Status m_status;
	//アニメーション
	AnimationChange m_anim;
	//エフェクト
	EffectAction m_effect;

	//アニメーション関係
	std::map<std::string, int> m_animIdx;
	int m_nowAnimIdx;        
	int m_equipAnimNo;        //一つ前のアニメーション
	int m_nowAnimNo;          //現在のアニメーション
	int m_prevAnimNo;         //変更前のアニメーション
	int m_frameAnimNo;        //フレームだけを行うアニメーション
	float m_animBlendRate;    //アニメーションの合成割合
	bool m_animOne[ANIMATION_MAX];      //アニメーション関係をリセットするための判定
	float m_nowFrame;         //アニメーションを進める時間
	float m_animTime;         //アニメーション再生速度
	bool m_isAnimationFinish; //アニメーションが終わったかどうか
	float m_animSpeed;        //アニメーションの進んだタイムを入れる
	bool m_animInit;          //アニメーションを初期化するかどうか
	bool m_animReverse;       //アニメーションを逆再生させるかどうか
	VECTOR m_nowPos;        //現在のフレームの座標を取得する

	//使う変数
	float m_angle;            //キャラのアングル
	float m_attackRadius;     //攻撃の当たり判定
	float m_searchRadius;     //索敵の当たり判定
	int m_heel;               //HPに追加する回復分
	int m_maxHeel;            //HPの最大回復量
	
};

