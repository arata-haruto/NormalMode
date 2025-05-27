#pragma once
#include "../Utility/Vector2D.h"

class GameObject {
protected:
    Vector2D location;   // オブジェクトの位置
    int image;           // 画像ID
    bool is_active;      // 有効状態
    int z_layer;         // 描画レイヤー
public:
    GameObject();
    virtual ~GameObject();

    virtual void Initialize();
    virtual void Update();
    virtual void Draw() const;
    virtual void Finalize();

    // 位置の取得・設定
    Vector2D GetLocation() const { return location; }
    void SetLocation(const Vector2D& new_location) { location = new_location; }

    // 画像の取得・設定
    int GetImage() const { return image; }
    void SetImage(int new_image) { image = new_image; }

    // 有効状態の管理
    bool IsActive() const { return is_active; }
    void SetActive(bool active) { is_active = active; }
};