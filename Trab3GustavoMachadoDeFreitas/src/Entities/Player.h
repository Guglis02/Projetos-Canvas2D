#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <functional>

#include "../gl_canvas2d.h"
#include "../Utils/VectorHomo.h"
#include "../Utils/VectorArts.h"
#include "Entity.h"
#include "../Utils/FpsController.h"

class Player : public Entity
{
public:
    Player(VectorHomo transform, function<void()> shootCallback, function<void()> onDeathCallback)
        : Entity(transform)
    {
        this->moveSpeed = 500;
        this->movementDirection = VectorHomo(0, 0);
        this->shootCallback = shootCallback;
        this->onDeathCallback = onDeathCallback;

        this->hitbox.push_back(VectorHomo(-32, -32));
        this->hitbox.push_back(VectorHomo(-32, 50));
        this->hitbox.push_back(VectorHomo(32, 50));
        this->hitbox.push_back(VectorHomo(32, -32));
    }

    void Update()
    {
        HandleMovement();
        HandleShooting();

        if (isInvincible)
        {
            invincibilityTimer -= FpsController::getInstance().GetDeltaTime();;
            if (invincibilityTimer <= 0)
            {
                isInvincible = false;
            }
        }

        Render();
    }

    bool CheckCollision(vector<VectorHomo> hitbox)
    {
        if (Entity::CheckCollision(hitbox))
        {
            OnHit();
            return true;
        }

        return false;
    }

    void OnHit()
    {
        if (!isInvincible)
        {
            hp--;
            isInvincible = true;
            invincibilityTimer = invincibilityDuration;
        }

        if (hp <= 0)
        {
            onDeathCallback();
        }
    }

    int GetHP()
    {
        return hp;
    }

    void SetHP(int hp)
    {
        this->hp = hp;
    }

    int GetMaxHP()
    {
        return maxHp;
    }

    void SetPosition(VectorHomo position)
    {
        transform = position;
    }

    // Callbacks
    void StartMovingUp()
    {
        isMovingUp = true;
    }

    void StopMovingUp()
    {
        isMovingUp = false;
        movementDirection.y = 0;
    }

    void StartMovingLeft()
    {
        isMovingLeft = true;
    }

    void StopMovingLeft()
    {
        isMovingLeft = false;
        movementDirection.x = 0;
    }

    void StartMovingDown()
    {
        isMovingDown = true;
    }

    void StopMovingDown()
    {
        isMovingDown = false;
        movementDirection.y = 0;
    }

    void StartMovingRight()
    {
        isMovingRight = true;
    }

    void StopMovingRight()
    {
        isMovingRight = false;
        movementDirection.x = 0;
    }

    void StartShooting()
    {
        isShooting = true;
    }

    void StopShooting()
    {
        isShooting = false;
    }

protected:
    void Render()
    {
        if (isInvincible && fmod(invincibilityTimer, 0.2f) < 0.1f)
        {
            DrawMillenniumFalconWireframe(transform);
        }
        else
        {
            DrawMillenniumFalcon(transform);
        }
    }

    void HandleMovement()
    {
        if (isMovingUp)
        {
            movementDirection += VectorHomo(0, 1);
        }
        if (isMovingDown)
        {
            movementDirection += VectorHomo(0, -1);
        }
        if (isMovingLeft)
        {
            movementDirection += VectorHomo(-1, 0);
        }
        if (isMovingRight)
        {
            movementDirection += VectorHomo(1, 0);
        }

        if (movementDirection == VectorHomo(0, 0))
        {
            return;
        }

        movementDirection.normalize();

        VectorHomo oldTransform = transform;        
        Move(movementDirection);

        if (IsOutOfBounds(GetHitbox()))
        {
            transform = oldTransform;
        }
    }

    void HandleShooting()
    {
        timeSinceLastShot += FpsController::getInstance().GetDeltaTime();

        if (isShooting && (timeSinceLastShot >= shootCooldown))
        {
            shootCallback();
            timeSinceLastShot = 0;
        }
    }

    bool isMovingUp = false;
    bool isMovingLeft = false;
    bool isMovingRight = false;
    bool isMovingDown = false;
    bool isShooting = false;    
    
    bool isInvincible = false;
    float invincibilityDuration = 2.0f;
    float invincibilityTimer = 0.0f;

    float shootCooldown = 0.3;
    float timeSinceLastShot = shootCooldown;

    float maxHp = 5;
    float hp = maxHp;

    function<void()> shootCallback;
    function<void()> onDeathCallback;

    VectorHomo movementDirection;
};
#endif // PLAYER_H_INCLUDED
