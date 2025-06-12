#include "Player.h"
#include "../../Utility/ResourceManager.h"
#include "../../Utility/InputManager.h"
#include "../Enemy/Enemy.h"
#include "../TurnManager.h"
#include "DxLib.h"


Player::Player(PlayerID id) : playerID(id), attack_power(20), selectedPartIndex(0)
{
}

Player::~Player() 
{
}

void Player::Initialize() 
{
    ResourceManager* rm = ResourceManager::GetInstance();

    heal_sound = rm->GetSoundResource("Resource/Sound/ゲージ回復1.wav");
    pass_sound = rm->GetSoundResource("Resource/Sound/鞭を振り回す1.mp3");
    attack_sound = rm->GetSoundResource("Resource/Sound/剣で斬る2.wav");
    beep_sound = rm->GetSoundResource("Resource/Sound/ビープ音1.wav");

    effect_image = rm->GetImageResource("Resource/Images/pipo-mapeffect008.png")[0];
    attack_effect_image = rm->GetImageResource("Resource/Images/tor_斬撃_直線.png")[0];

   /* heal_image = rm->GetImageResource("Resource/Images/healcount.png")[0];
    pass_image = rm->GetImageResource("Resource/Images/passcount.png")[0];*/
}

void Player::Update() {
    TurnManager* turnManager = TurnManager::GetInstance();
    InputManager* input = InputManager::GetInstance();

    if (isHealEffectPlaying)
    {
        healEffectTimer++;
        if (healEffectTimer % 5 == 0)
        {
            healEffectFrame++;
            if (healEffectFrame >= 10) // 全フレーム再生完了
            {
                isHealEffectPlaying = false;

                TurnManager::GetInstance()->NextTurn();

            }
        }
        return;
    }

    if (isAttackEffectPlaying)
    {
        attackEffectTimer++;
        if (attackEffectTimer % 5 == 0)
        {
            attackEffectFrame++;
            if (attackEffectFrame >= 10) // フレーム数に合わせて調整
            {
                isAttackEffectPlaying = false;

                TurnManager::GetInstance()->NextTurn(); // エフェクト終了後にターン進行
            }
        }
        return; // エフェクト再生中は操作禁止
    }

    // 自分のターンかチェック
    if ((playerID == PlayerID::Player1 && turnManager->GetCurrentTurn() != Turn::Player1) ||
        (playerID == PlayerID::Player2 && turnManager->GetCurrentTurn() != Turn::Player2))
    {
        return; // 自分のターンじゃない
    }

    // ターン切り替え演出中は操作できない
    if (turnManager->ShowTurnMessage()) {
        return;
    }

    if (input->GetKeyState(KEY_INPUT_X) == eInputState::Pressed ||
        input->GetButtonState(XINPUT_BUTTON_Y) == eInputState::Pressed)
    {
        //Healを呼び出す
        Heal(40);
    }

    if (input->GetKeyState(KEY_INPUT_C) == eInputState::Pressed ||
        input->GetButtonState(XINPUT_BUTTON_X) == eInputState::Pressed)
    {
        //Passを呼び出す
        Pass();
    }

    // 攻撃
    if (input->GetKeyState(KEY_INPUT_Z) == eInputState::Pressed ||
        input->GetButtonState(XINPUT_BUTTON_B) == eInputState::Pressed)
    {
        Enemy* enemy = Enemy::GetInstance();

        int damage = enemy->GetSelectedPartDamage();  // これを用意する

        enemy->TakeDamage(damage);
        //printfDx("Player %d が攻撃！\n", playerID == PlayerID::Player1 ? 1 : 2);
        // 攻撃エフェクトを開始
        isAttackEffectPlaying = true;
        attackEffectFrame = 0;
        attackEffectTimer = 0;

        // 選択中のパーツの位置を取得してセット
        attackEffectPosition = Enemy::GetInstance()->GetSelectedPartPosition();
        PlaySoundMem(attack_sound, DX_PLAYTYPE_BACK);
        
      
    }

}

void Player::Draw() const {
    
    if (isHealEffectPlaying)
    {
        const int frameWidth = 240;  // 1フレームの横幅
        const int frameHeight = 480; // 1フレームの高さ
        const int columns = 10;      // 横に10個フレームが並んでいる

        int fx = (healEffectFrame % columns) * frameWidth;
        int fy = 0;
        /*int fx = (healEffectFrame % columns) * frameWidth;
        int fy = 0;*/

        DrawRectGraph(
            static_cast<int>(healEffectPosition.x),
            static_cast<int>(healEffectPosition.y),
            fx, fy,
            frameWidth, frameHeight,
            effect_image,
            TRUE
        );
    }

    if (isAttackEffectPlaying)
    {
        const int frameWidth = 192;
        const int frameHeight = 192; 

        int columns = 5; 

        int fx = (attackEffectFrame % columns) * frameWidth;
        int fy = 0;

        DrawRectGraph(
            static_cast<int>(attackEffectPosition.x),
            static_cast<int>(attackEffectPosition.y),
            fx, fy,
            frameWidth, frameHeight,
            attack_effect_image,
            TRUE
        );
    }

    __super::Draw();
}

void Player::Heal(int amount)
{
    if (healCount > 0) { // 残り回数チェック
        Enemy::GetInstance()->Heal(40); // プレイヤーのHP回復
        PlaySoundMem(heal_sound, DX_PLAYTYPE_BACK);

        // エフェクト演出
        isHealEffectPlaying = true;
        healEffectTimer = 0;
        healEffectFrame = 0;
        healEffectPosition = {205, 40};

        healCount--;
        
        //printfDx("Player %d が回復！ 残り %d 回\n", playerID == PlayerID::Player1 ? 1 : 2, healCount);
    }
    else {
        PlaySoundMem(beep_sound, DX_PLAYTYPE_BACK); // 使用不可の通知音
    }
}

void Player::Pass()
{
    if (passCount > 0) { // 残り回数チェック
        passCount--;
        PlaySoundMem(pass_sound, DX_PLAYTYPE_BACK);
        Sleep(500); // 0.5秒待機

        TurnManager::GetInstance()->NextTurn();
        //printfDx("Player %d がパス！ 残り %d 回\n", playerID == PlayerID::Player1 ? 1 : 2, passCount);
    }
    else {
        PlaySoundMem(beep_sound, DX_PLAYTYPE_BACK); // 使用不可の通知音
    }
}