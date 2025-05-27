#include "BodyPart.h"

BodyPart::BodyPart(const std::string& name, const Vector2D& pos, int w, int h)
    : name(name), position(pos), width(w), height(h) 
{
}