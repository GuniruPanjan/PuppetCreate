#pragma once
#include "CharacterBase.h"

class Weapon;
class Shield;
class Armor;
class EnemyManager;
class CoreManager;
class GameManager;
class Tool;
class SEManager;

class Player : public CharacterBase
{
public:
	//レベル関係の構造体
	struct LevelUpStatus
	{
		int sl_hp;         //体力
		int sl_stamina;    //スタミナ
		int sl_muscle;     //筋力
		int sl_skill;      //技量
		int sl_all;        //すべてのレベル

	}m_levelStatus;

	//ステータスのマックス値
	struct MaxStatus
	{
		int sm_hp;         //体力のマックス
		int sm_stamina;    //スタミナのマックス

	}ms_maxStatus;

	//アニメーション関係の構造体
	struct AnimationChange
	{
		bool sa_dashMove;                    //ダッシュ判断
		bool sa_avoidance;                   //回避判断
		bool sa_recovery;                    //回復判定
		bool sa_taking;                      //アイテムを取得する判定
		bool sa_touch;                       //ギミックを作動させる判定
		bool sa_bossEnter;                   //ボスの入り口に入る判定
		bool sa_enterShield;                 //盾を構える判定
		bool sa_shieldIdle;                  //盾を構えてる判定
		bool sa_imapact;                     //盾受けした判定
		bool sa_strengthAttack;              //強攻撃

	}m_animChange;

	Player();
	virtual ~Player();

	void Init(std::shared_ptr<MyLibrary::Physics> physics, GameManager* manager, Weapon& weapon, Shield& shield, Armor& armor, bool anim);
	void GameInit(std::shared_ptr<MyLibrary::Physics> physics);
	void Finalize();
	void Update(Weapon& weapon, Shield& shield, Armor& armor, EnemyManager& enemy, CoreManager& core, VECTOR restpos, Tool& tool, SEManager& se, bool boss, bool dead, std::shared_ptr<MyLibrary::Physics> physics);
	void Action(VECTOR restpos, Tool& tool, Shield& shield, SEManager& se, bool boss, bool dead);
	void EffectAction();
	void WarpMap();
	void NotWeaponAnimation();
	void AllAnimation();
	void WeaponAnimation(Shield& shield);
	void Draw(Armor& armor, int font);
	void End();

	//衝突したとき
	void OnCollideEnter(const std::shared_ptr<Collidable>& collidable) override;
	void OnCollideStay(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;

	//装備関係の関数
	void ArmorChange(int one, std::string path);
	//レベル関係のステータス
	void ChangeStatus();

	//プレイヤーが生きているかを取得
	const bool IsGetPlayerDead() const { return m_anim.s_isDead; }

	//カメラ関係
	void SetCameraAngle(float angle) { m_cameraAngle = angle; }
	float SetAngle(float angle) { return m_lockAngle = angle; }
	bool GetLock() { return m_lockonTarget; }
	float GetSearch() { return m_searchRadius; }

	//メニュー関係
	bool GetMenu() { return m_menuOpen; }
	bool SetMenu(bool set) { return m_menuOpen = set; }

	//マップ関係
	int SetMapNow(int set) { return m_mapNow = set; }

	//休息関係
	bool GetRest() { return m_rest; }
	bool GetBigRest() { return m_bigRest; }
	bool GetRestTouch() { return m_restTouch; }
	bool SetRest(bool set) { return m_restTouch = set; }
	bool SetNotRest(bool set) { return m_rest = set; }

	//マップアイテム関係
	bool SetItemPick(bool set) { return m_itemPick = set; }
	bool GetItemPick() { return m_itemPick; }
	bool GetTaking() { return m_animChange.sa_taking; }
	float GetNowFrame() { return m_nowFrame; }

	//メッセージ関係
	bool SetMessegePick(bool set) { return m_message = set; }
	bool GetMessagePick() { return m_message; }
	bool SetMessageRead(bool set) { return m_read = set; }
	bool GetMessageRead() { return m_read; }

	//ワープ関係
	bool SetWarp(bool set) { return m_warp = set; }
	bool GetWarp() { return m_warp; }

	//ボス部屋に入るための判定関係
	bool SetBossStart(bool set) { return m_bossStart = set; }
	bool GetBossEnter() { return m_bossStart; }
	bool GetBossStart() { return m_animChange.sa_bossEnter; }

	//死亡関係
	bool GetDead() { return m_deadReset; }

	int SetRoll(int set) { return m_notRoll = set; }

	//ステータス関係
	Status GetStatus() { return m_status; }
	LevelUpStatus GetLevelStatus() { return m_levelStatus; }
	MaxStatus GetMaxStatus() { return ms_maxStatus; }

	//アイテムなどのステータス関係
	int GetLevel() { return m_levelStatus.sl_all; }
	int GetCore() { return m_status.s_core; }
	int SetUseCore(int core) { return m_status.s_core = m_status.s_core - core; }
	int SetReturnCore(int core) { return m_status.s_core = m_status.s_core + core; }
	int GetHPLevel() { return m_levelStatus.sl_hp; }
	int SetHPLevel(int set) { return m_levelStatus.sl_hp = set; }
	int GetStaminaLevel() { return m_levelStatus.sl_stamina; }
	int SetStaminaLevel(int set) { return m_levelStatus.sl_stamina = set; }
	int GetMuscleLevel() { return m_levelStatus.sl_muscle; }
	int SetMuscleLevel(int set) { return m_levelStatus.sl_muscle = set; }
	int GetSkillLevel() { return m_levelStatus.sl_skill; }
	int SetSkillLevel(int set) { return m_levelStatus.sl_skill = set; }
	float GetAttackDamage() { return m_attackDamage; }
	float GetAttackMuscle() { return m_equipmentMuscle; }
	float GetAttackSkill() { return m_equipmentSkill; }

	const MyLibrary::LibVec3 GetPos() const { return rigidbody.GetPos(); }
	const MyLibrary::LibVec3 GetShieldPos() const { return m_shieldPos; }

private:
	void SetModelPos();
private:
	std::shared_ptr<AttackObject> m_pStrengthAttack;       //強攻撃判定
	std::shared_ptr<AttackLigObject> m_pLigAttack;         //リグ毎の攻撃判定
	std::shared_ptr<PlayerSearchObject> m_pSearch;         //索敵判定
	std::shared_ptr<PlayerSearchObject> m_pShieldSearch;   //盾で防げるかの索敵判定
	std::shared_ptr<ShieldObject> m_pShield;               //盾の判定

	EnemyAttackObject* m_pEnemyAttackCol;    //敵の攻撃

	MyLibrary::LibVec3 m_shieldPos;
	MyLibrary::LibVec3::Size m_shieldSize;
	MyLibrary::LibVec3 m_shieldSearchPos;

	XINPUT_STATE m_xpad;                //パッド入力
	int m_mapNow;                       //現在のマップ
	float m_cameraAngle;                //カメラ情報
	float m_searchRadius;               //索敵範囲
	float m_lockAngle;                  //ロックオンしたときのアングル
	float m_updateX;                    //休息したときの初期化位置X
	float m_updateY;                    //休息したときの初期化位置Y
	float m_updateZ;                    //休息したときの初期化位置Z
	float m_attackDamage;               //攻撃力を格納する変数
	float m_equipmentMuscle;            //武器の筋力補正変数
	float m_equipmentSkill;             //武器の技量補正変数
	bool m_menuOpen;                    //メニューを開く判定
	bool m_restTouch;                   //休息できるかの判定
	bool m_rest;                        //休息判定
	bool m_itemPick;                    //アイテムを拾える判定
	bool m_bigRest;                     //レベルが上げられる休息判定
	bool m_lockonTarget;                //ターゲット判定
	bool m_warp;                        //マップをワープするための判定
	bool m_bossStart;                   //ボス部屋に入るための判定
	bool m_shieldOne;                   //盾の判定初期化
	bool m_armorOne[10];                //防具の初期化(適当に作っておく)
	bool m_staminaBreak;                //スタミナ切れの状態
	bool m_message;                     //メッセージ変数
	bool m_read;                        //読む変数

	//アニメーション用変数
	int m_attackNumber;                 //現在の攻撃段階の代入
	bool m_avoidanceNow;                //フレーム回避中の判断
	bool m_shieldNow;                   //防御中の判断
	bool m_animReverse;                 //アニメーションを逆再生させるための判定
	bool m_deadReset;                   //死亡完了した判定

	//フレーム用変数
	int m_moveAnimFrameIndex;           //アニメーションで移動しているフレームの番号を検索する変数
	int m_moveAnimFrameRight;
	int m_attackLig1;                   //攻撃判定のリグ1
	int m_attackLig2;                   //攻撃判定のリグ2
	int m_moveAnimShieldFrameIndex;     //盾を構えるときのアニメーションのフレーム取得
	int m_moveAnimShieldFrameHandIndex;
	int m_notRoll;                      //ローリングできないようにする
	MATRIX m_moveWeaponFrameMatrix;     //武器をアタッチするフレームのローカル座標をワールド変換行列を取得する
	MATRIX m_moveShieldFrameMatrix;
	VECTOR m_attackLigPos1;             //攻撃判定リグのポジション1
	VECTOR m_attackLigPos2;             //攻撃判定リグのポジション2
	VECTOR m_rollMove;                  //回避で移動する距離
	VECTOR m_moveVector;                //モーション中の移動
	VECTOR m_attackMove;                //攻撃で移動する距離
};

