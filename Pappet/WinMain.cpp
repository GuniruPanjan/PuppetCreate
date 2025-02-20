#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "Scene/SceneManager.h"
#include "Manager/EffectManager.h"
#include <cmath>
#include <memory>
#include "Icon/Icon.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//名前変更
	SetWindowText("Puppet Graves");

	//画面の大きさ
	// windowモード設定
	SetGraphMode(1600, 1000, 32);
	ChangeWindowMode(false);

	SetWindowIconID(IDI_ICON1);    //アイコンの設定

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{

		return -1;			// エラーが起きたら直ちに終了

	}

	//Effekseerを初期化する
	//引数には画面に表示する最大パーティクル数を設定する
	if (Effekseer_Init(15000) == -1)
	{
		DxLib_End();
		return -1;
	}

	Effekseer_InitDistortion();

	//Dxライブラリのデバイスロストしたときのコールバックを設定
	//ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	//ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	//シーンを管理するポインタ
	std::shared_ptr<SceneManager> pScene = std::make_shared<SceneManager>();

	//初期化
	pScene->Init();

	// ゲームループ
	while (ProcessMessage() == 0)
	{
		// このフレームの開始時刻を覚えておく
		LONGLONG start = GetNowHiPerformanceCount();

		// 描画を行う前に画面をクリアする
		ClearDrawScreen();

		// ゲームの処理
		pScene->Update();
		Effekseer_Sync3DSetting();


		UpdateEffekseer3D();

		//ゲームの描画
		pScene->Draw();

		// 画面が切り替わるのを待つ
		ScreenFlip();


		if (pScene->GetEnd() == true)
		{
			break;
		}

		// escキーでゲーム終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))  break;

		// FPS60に固定する
		while (GetNowHiPerformanceCount() - start < 16667)
		{

			// 16.66ミリ秒(16667マイクロ秒)経過するまで待つ

		}

	}
	pScene->End();


	EffectManager::GetInstance().Destroy();
	Effkseer_End();
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 

}