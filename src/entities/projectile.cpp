#include <entities/projectile.hpp>

Projectile::Projectile(int x, int y, const char *texturePath, struct FrameData *frameInfo, int speed)
    : Animated(x, y, texturePath, frameInfo), speed(speed)
{
    // this->texture.loadFromFile("assets/super_projectile_sheet.png");
    // this->sprite.setTexture(this->texture);
    // this->sprite.scale(Entity::spriteScale);

    // Hardcoded animation (for now)
    // for (int i = 0; i < 14; i++)
    // {
    //     frames[i] = sf::IntRect(i * 16, 0, 16, 16);
    // }

    // this->sprite.setTextureRect(frames[0]);
}

Projectile::~Projectile()
{
}

void Projectile::update()
{
    // frameTime++;
    // if (frameTime % frameLength == 0)
    // {
    //     currentFrame = (currentFrame + 1) % 14;
    //     frameTime = 0;
    // }
    // this->sprite.setTextureRect(frames[currentFrame]);
    // if (y < 0)
    // {
    //     puts("I'm offscreen");
    //     return;
    // }
    animate();
    this->y -= speed * 1;
    sprite.setPosition(sf::Vector2f(this->x, this->y));
}
