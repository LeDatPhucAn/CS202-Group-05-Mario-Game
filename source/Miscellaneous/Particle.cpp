#include "Particle.hpp"
#include "Block.hpp"
void Particle::spawnParticles(const Block& block, std::vector<Particle>& particles) {
    // chia srcRec thành 4 miếng
    float w2 = block.srcRec.width*0.5f;
    float h2 = block.srcRec.height*0.5f;


    // 4 vận tốc cố định: giống New SMB
    static const Vector2 initVel[4] = {
        { -150.0f, -250.0f },   // top-left bay mạnh lên
        {  150.0f, -250.0f },   // top-right bay mạnh lên
        { -150.0f, -150.0f },   // bot-left bay lên thấp hơn
        {  150.0f, -150.0f }    // bot-right bay lên thấp hơn
    };

    // load texture 1 lần (không trong vòng for)
    static Texture2D debrisTex = LoadTexture("assets/Map/Map1/image/collided_animation.png");

    for (int i = 0; i < 4; ++i) {
        Particle p{
            block.getPosition(),           // pos
            { w2, h2 },                    // size
            debrisTex,                     // texture
            {0,0, 16,16},                      // srcRec
            initVel[i],                    // initialVelocity
            0.6f                           // lifetimeSeconds
        };
        particles.push_back(p);
    }
}


void Particle::display(float dt) {
    if (!isActive) return;
    DrawTexturePro(texture, srcRec, {pos.toPixels().x, pos.toPixels().y, 12,12}, {6, 6}, rotationAngle, WHITE);
}

void Particle::update() {
    float dt = GetFrameTime();
    move.update(pos.vec, dt);     // Movement sẽ cập nhật pos và age
    if (age >= maxLifetime) {
        isActive = false;
    }
    rotationAngle += angularSpeed * dt;   
    if (rotationAngle >= 360.0f)        
        rotationAngle -= 360.0f;
}
