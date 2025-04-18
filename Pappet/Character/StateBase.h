#pragma once
#include <memory>
#include <string>
#include "Library/MyLibrary.h"

class CharacterBase;

class StateBase : public std::enable_shared_from_this<StateBase>
{
public:
	//ステートKind
	enum class StateKind
	{
		Null,
		Idle,        //待機
		Walk,        //歩く
		Roll,        //回避
		Dash,        //走る
		Jump,        //ジャンプ
		Attack,      //攻撃
		StrongAttack,//強攻撃
		Guard,      //盾

		Parry,       //パリィ   //まだやっていない
		
		Damage,      //ダメージ
		Item,        //回復など
		Taking,      //アイテム取得
		EnterBoss,   //ボス部屋に入るモーション
		Death,       //死亡
	};

protected:
	//左スティックの入力方向
	const enum eDir : int
	{
		Forward,
		ForwardRight,
		Right,
		BackRight,
		Back,
		BackLeft,
		Left,
		ForwardLeft,
		None
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="chara">所有者</param>
	StateBase(std::shared_ptr<CharacterBase> chara);

	virtual ~StateBase();

	//初期化
	virtual void Init(int md) abstract;

	//更新
	virtual void Update() abstract;

	//現在のStateを返す
	StateKind GetState() { return m_nowState; }
	//遷移予定のStateを返す
	const StateKind GetNextState() { return m_nextState; }
	//ステートを遷移する
	void ChangeState(StateKind kind);
	//持ち主を判断する
	bool CheckState();
	//引数の二つの値から角度を計算する
	const eDir GetDirection(float x, float y)const;

	//遷移予定のStateを設定する
	void SetNextKind(StateKind state) { m_nextState = state; }
	//ターゲット状態だとアニメーションが変わる
	bool SetTarget(bool set) { return m_targetState = set; }
	//装備を付けているとアニメーションが変わる
	bool SetEquipment(bool set) { return m_equipmentState = set; }
	//ヒット時の判定を決める
	bool SetHit(bool set) { return m_hit = set; }
	//装備が剣などの場合
	bool SetSword(bool set) { return m_equipmentSword = set; }

protected:
	std::weak_ptr<CharacterBase> m_pChara;   //所有者のポインタ

	int m_stageCol;    //ステージの当たり判定

	StateKind m_nowState;      //現在のステート
	StateKind m_nextState;     //次のステート

	float m_leftX, m_leftZ;
	bool m_targetState;
	bool m_equipmentState;
	//攻撃がプレイヤーにヒットしたかの判定
	bool m_hit;
	//どの種類の装備をしているか
	bool m_equipmentFist = false;
	bool m_equipmentSword = false;
	bool m_equipmentShield = false;
	//ジャンプ中の判定
	bool m_jumping = false; 

	bool m_revese = false;     //アニメーションを逆再生させるかの判断
};

