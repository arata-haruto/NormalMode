#pragma once
#include "../Utility/Vector2D.h"

class GameObject {
protected:
    Vector2D location;   // �I�u�W�F�N�g�̈ʒu
    int image;           // �摜ID
    bool is_active;      // �L�����
    int z_layer;         // �`�惌�C���[
public:
    GameObject();
    virtual ~GameObject();

    virtual void Initialize();
    virtual void Update();
    virtual void Draw() const;
    virtual void Finalize();

    // �ʒu�̎擾�E�ݒ�
    Vector2D GetLocation() const { return location; }
    void SetLocation(const Vector2D& new_location) { location = new_location; }

    // �摜�̎擾�E�ݒ�
    int GetImage() const { return image; }
    void SetImage(int new_image) { image = new_image; }

    // �L����Ԃ̊Ǘ�
    bool IsActive() const { return is_active; }
    void SetActive(bool active) { is_active = active; }
};