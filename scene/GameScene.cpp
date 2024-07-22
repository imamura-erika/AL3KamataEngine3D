#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() { // デストラクタ
	delete model_; // 3Dモデル
	delete player_; // プレイヤー
	delete blockModel_; // ブロック3Dモデル
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		delete worldTransformBlock;
	}
	worldTransformBlocks_.clear(); // 配列

	delete debugCamera_; // デバッグカメラ
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("uvChecker.png");
	// 3Dモデルの生成
	model_ = Model::Create();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Intialize(model_, textureHandle_, &viewProjection_);

	// ブロック3Dモデルの生成
	blockModel_ = Model::Create();
	// 要素数
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック1個分の情報
	const float kBlockWidth = 2.0f;
	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockHorizontal);

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockHorizontal; ++i) {
		worldTransformBlocks_[i] = new WorldTransform();
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = kBlockWidth * i;
		worldTransformBlocks_[i]->translation_.y = 0.0f;
	}


	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	// ブロックの更新
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {

		// アフィン変換行列の作成
		Matrix4x4 affineMatrix = Matrix4x4::MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

		worldTransformBlock->matWorld_ = affineMatrix; // アフィン変換行列

		// 定数バッファに転送する
		worldTransformBlock->TransferMatrix();
	}

// デバッグカメラの更新
// debugCamera_->Update();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_BACKSPACE)) {
		isDebugCameraActive_ = true; // デバッグカメラ有効フラグをトグル
	}
	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update(); // デバッグカメラの更新
		viewProjection_.matView = debugCamera_.view; //デバッグカメラのビュー行列
		viewProjection_.matProjection = debugCamera_.projection; //デバッグカメラのプロジェクション行列
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
	//ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
		

	#endif

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	player_->Draw();

	// ブロックの描画
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		blockModel_->Draw(*worldTransformBlock, viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}