#include "PhysicsSystem.h"
#include "../Interface/States.h"


PhysicsSystem::PhysicsSystem()
{
}

PhysicsSystem::~PhysicsSystem()
{
}

bool PhysicsSystem::IsColliding(ECS::ComponentHandle<BoxCollider> touchingBox, ECS::ComponentHandle<BoxCollider> touchedBox, float x, float y)
{
    return touchingBox->rightEdge + x > touchedBox->leftEdge &&
        touchedBox->rightEdge > touchingBox->leftEdge + x &&
        touchingBox->bottomEdge + y > touchedBox->topEdge &&
        touchedBox->bottomEdge > touchingBox->topEdge + y;
}

bool PhysicsSystem::IsColliding(ECS::ComponentHandle<BoxCollider> touchingBox, sf::RectangleShape touchedRectangle, float x, float y)
{
    float _touchedRectLeft = touchedRectangle.getPosition().x;
    float _touchedRectRight = _touchedRectLeft + touchedRectangle.getGlobalBounds().width;
    float _touchedRectTop = touchedRectangle.getPosition().y;
    float _touchedRectBottom = _touchedRectTop + touchedRectangle.getGlobalBounds().height;

    return touchingBox->rightEdge + x > _touchedRectLeft &&
        _touchedRectRight > touchingBox->leftEdge + x &&
        touchingBox->bottomEdge + y > _touchedRectTop &&
        _touchedRectBottom > touchingBox->topEdge + y;
}

bool PhysicsSystem::IsColliding(ECS::ComponentHandle<BoxCollider> touchingBox, ECS::ComponentHandle<BoxCollider> touchedBox)
{
    return touchingBox->rightEdge > touchedBox->leftEdge &&
        touchedBox->rightEdge > touchingBox->leftEdge &&
        touchingBox->bottomEdge > touchedBox->topEdge &&
        touchedBox->bottomEdge > touchingBox->topEdge;
}

void PhysicsSystem::CheckCollisionSides(ECS::ComponentHandle<Transform> transform, ECS::ComponentHandle<BoxCollider> touchingBox, ECS::ComponentHandle<BoxCollider> touchedBox)
{
    // Scenario dealing with collision from the right side by accelerating rightward
    if (transform->xSpeed > 0 &&
        touchedBox->rightEdge + transform->xSpeed > touchedBox->leftEdge &&
        touchingBox->topEdge < touchedBox->bottomEdge &&
        touchingBox->bottomEdge > touchedBox->topEdge)
    {
        transform->xSpeed = 0;
    }

    // Scenario dealing with collision from the bottom side by accelerating downward
    if (transform->ySpeed > 0 &&
        touchedBox->bottomEdge + transform->ySpeed > touchedBox->topEdge &&
        touchingBox->leftEdge < touchedBox->rightEdge &&
        touchingBox->rightEdge > touchedBox->leftEdge)
    {
        transform->ySpeed = 0;
    }

    // Scenario dealing with collision from the left side by accelerating leftward
    if (transform->xSpeed < 0 &&
        touchedBox->leftEdge + transform->xSpeed > touchedBox->rightEdge &&
        touchingBox->topEdge < touchedBox->bottomEdge &&
        touchingBox->bottomEdge > touchedBox->topEdge)
    {
        transform->xSpeed = 0;
    }

    // Scenario dealing with collision from the top side by accelerating upward
    if (transform->ySpeed < 0 &&
        touchedBox->topEdge + transform->ySpeed > touchedBox->bottomEdge &&
        touchingBox->leftEdge < touchedBox->rightEdge &&
        touchingBox->rightEdge > touchedBox->leftEdge)
    {
        transform->ySpeed = 0;
    }
}

void PhysicsSystem::CheckCollisionSides(ECS::ComponentHandle<Transform> transform, ECS::ComponentHandle<BoxCollider> touchingBox, sf::RectangleShape touchedRectangle)
{
    float _touchedRectLeft = touchedRectangle.getPosition().x;
    float _touchedRectRight = _touchedRectLeft + touchedRectangle.getGlobalBounds().width;
    float _touchedRectTop = touchedRectangle.getPosition().y;
    float _touchedRectBottom = _touchedRectTop + touchedRectangle.getGlobalBounds().height;

    // Scenario dealing with collision from the right side by accelerating rightward
    if (transform->xSpeed > 0 &&
        touchingBox->rightEdge + transform->xSpeed > _touchedRectLeft &&
        touchingBox->topEdge < _touchedRectBottom &&
        touchingBox->bottomEdge > _touchedRectTop)
    {
        transform->xSpeed = 0;
    }

    // Scenario dealing with collision from the bottom side by accelerating downward
    if (transform->ySpeed > 0 &&
        touchingBox->bottomEdge + transform->ySpeed > _touchedRectTop &&
        touchingBox->leftEdge < _touchedRectRight &&
        touchingBox->rightEdge > _touchedRectLeft)
    {
        transform->ySpeed = 0;
    }

    // Scenario dealing with collision from the left side by accelerating leftward
    if (transform->xSpeed < 0 &&
        touchingBox->leftEdge + transform->xSpeed > _touchedRectRight &&
        touchingBox->topEdge < _touchedRectBottom &&
        touchingBox->bottomEdge > _touchedRectTop)
    {
        transform->xSpeed = 0;
    }

    // Scenario dealing with collision from the top side by accelerating upward
    if (transform->ySpeed < 0 &&
        touchingBox->topEdge + transform->ySpeed > _touchedRectBottom &&
        touchingBox->leftEdge < _touchedRectRight &&
        touchingBox->rightEdge > _touchedRectLeft)
    {
        transform->ySpeed = 0;
    }
}

void PhysicsSystem::PushEntity(ECS::Entity* touchingEntity, ECS::Entity* touchedEntity)
{
    float _newTouchingX = touchingEntity->get<struct Transform>()->xPos;
    float _newTouchingY = touchingEntity->get<struct Transform>()->yPos;
    
    float _newTouchingXSpeed = touchingEntity->get<struct Transform>()->xSpeed;
    float _newTouchingYSpeed = touchingEntity->get<struct Transform>()->ySpeed;

    float _newTouchedX = touchedEntity->get<struct Transform>()->xPos;
    float _newTouchedY = touchedEntity->get<struct Transform>()->yPos;

    // traveling rightward pushing from the left side
    if (_newTouchingXSpeed > 0 && _newTouchingX < _newTouchedX)
    {
        touchedEntity->get<struct Transform>()->xPos++;
    }
    // traveling leftward pushing from the right side
    else if (_newTouchingXSpeed < 0 && _newTouchingX > _newTouchedX)
    {
        touchedEntity->get<struct Transform>()->xPos--;
    }
    // traveling downward pushing from the top side
    else if (_newTouchingYSpeed > 0 && _newTouchingY < _newTouchedY)
    {
        touchedEntity->get<struct Transform>()->yPos++;
    }
    // traveling upward pushing from the bottom side
    else if (_newTouchingYSpeed < 0 && _newTouchingY > _newTouchedY)
    {
        touchedEntity->get<struct Transform>()->yPos--;
    }
}

void PhysicsSystem::tick(ECS::World* world, float deltaTime)
{
    if (States::GetPausedState() == false)
    {
        world->each<struct BoxCollider, struct Sprite2D, struct Transform>(
            [&](ECS::Entity* entity,
                ECS::ComponentHandle<struct BoxCollider> box,
                ECS::ComponentHandle<struct Sprite2D> sprite,
                ECS::ComponentHandle<struct Transform> transform
                ) -> void
        {
            box->Update(transform->xPos, transform->yPos,
                sprite->picture.getTextureRect().width,
                sprite->picture.getTextureRect().height);
        });

        world->each<struct BoxCollider, struct Transform>(
            [&](ECS::Entity* touchingEntity,
                ECS::ComponentHandle<BoxCollider> touchingBox,
                ECS::ComponentHandle<Transform> transform1
                ) -> void
        {
            world->each<struct BoxCollider, struct Transform>(
                [&](ECS::Entity* touchedEntity,
                    ECS::ComponentHandle<struct BoxCollider> touchedBox,
                    ECS::ComponentHandle<struct Transform> transform2
                    ) -> void
            {
                // statement to avoid comparing same entity to itself
                if (touchingEntity->getEntityId() != touchedEntity->getEntityId())
                {
                    // initial collision check
                    if (IsColliding(touchingBox, touchedBox) == true)
                    {
                        // final collision check
                        PushEntity(touchingEntity, touchedEntity);
                    }
                }
            });
        });

        world->each<struct Transform>(
            [&](ECS::Entity* entity,
                ECS::ComponentHandle<struct Transform> transform
                ) -> void
        {
            transform->Move();
        });
    }
}
