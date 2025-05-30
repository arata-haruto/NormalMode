#pragma once
#include <string>
#include "../../Utility/Vector2D.h"

class BodyPart {
private:
    std::string name;
    int hp;
    Vector2D position;
    int width, height;

    int damageMin;  // ダメージ最小値
    int damageMax;  // ダメージ最大値

public:
    BodyPart(const std::string& name, const Vector2D& pos, int w, int h, int dmgMin, int dmgMax);

    std::string GetName() const { return name; }
    const Vector2D& GetPosition() const { return position; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

    int GetRandomDamage() const;
};