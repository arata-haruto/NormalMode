#include "BodyPart.h"
#include <cstdlib>

BodyPart::BodyPart(const std::string& name, const Vector2D& pos, int w, int h, int dmgMin, int dmgMax)
    : name(name), position(pos), width(w), height(h), damageMin(dmgMin), damageMax(dmgMax)
{
}

int BodyPart::GetRandomDamage() const {
    return damageMin + (rand() % (damageMax - damageMin + 1));
}