#include <scenes/GameplayScene.h>

#include <components/HealthComponent.h>
#include <components/PlayerInputComponent.h>
#include <scenes/GameOverScene.h>
#include <systems/CollisionSystem.h>

#include <dagger/assets/AssetManager.h>
#include <dagger/assets/Font.h>
#include <dagger/core/Application.h>
#include <dagger/core/Entity.h>
#include <dagger/core/components/CameraComponent.h>
#include <dagger/core/components/SpriteComponent.h>
#include <dagger/core/components/TextComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/VelocityComponent.h>
#include <dagger/graphics/Texture.h>
#include <dagger/input/Input.h>

#include <cmath>
#include <ctime>

namespace delve
{

void GameplayScene::onInit(dagger::Application& app)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Load assets
    auto& assets = app.getAssets();
    auto playerTexture = assets.load<dagger::Texture>("player", "assets/player.png");
    auto enemyTexture = assets.load<dagger::Texture>("enemy", "assets/crawl-tiles/dc-mon/orc_knight.png");
    auto uiFont = assets.load<dagger::Font>("ui", "assets/fonts/RobotoMono-Regular.ttf", 16);

    floorTex_ = assets.load<dagger::Texture>("floor", "assets/crawl-tiles/dc-dngn/floor/pebble_brown0.png");
    wallTex_ = assets.load<dagger::Texture>("wall", "assets/crawl-tiles/dc-dngn/wall/lair1.png");

    // Map
    tileMap_ = std::make_unique<TileMap>(60, 60, 64.0f);

    auto spawnPoint = tileMap_->generateDungeon(15, 5, 10);

    for (int y = 20; y < 30; ++y)
    {
        for (int x = 20; x < 30; ++x)
        {
            tileMap_->setTile(x, y, TileType::Floor);
        }
    }

    // Camera
    camera_ = createEntity();
    camera_->addComponent<dagger::TransformComponent>();
    camera_->addComponent<dagger::CameraComponent>();

    // Player
    player_ = createEntity();

    auto& pTransform = player_->addComponent<dagger::TransformComponent>();
    pTransform.setPosition(spawnPoint.first, spawnPoint.second);

    auto& pSprite = player_->addComponent<dagger::SpriteComponent>();
    pSprite.texture = playerTexture;
    pSprite.zIndex = 10;
    pSprite.width = 48.0f;
    pSprite.height = 48.0f;

    player_->addComponent<dagger::VelocityComponent>();
    player_->addComponent<PlayerInputComponent>();

    auto& pHealth = player_->addComponent<HealthComponent>();
    pHealth.hp = 3;
    pHealth.maxHp = 3;

    // Enemies
    int numberOfEnemies = 20;

    for (int i = 0; i < numberOfEnemies; ++i)
    {
        int tileX = 0;
        int tileY = 0;
        bool foundValidSpawn = false;

        while (!foundValidSpawn)
        {
            tileX = std::rand() % tileMap_->getWidth();
            tileY = std::rand() % tileMap_->getHeight();

            if (tileMap_->getTile(tileX, tileY) == TileType::Floor)
            {
                foundValidSpawn = true;
            }
        }

        float spawnX = tileX * tileMap_->getTileSize();
        float spawnY = tileY * tileMap_->getTileSize();

        dagger::Entity* newEnemy = createEntity();

        auto& enemyTransform = newEnemy->addComponent<dagger::TransformComponent>();
        enemyTransform.setPosition(spawnX, spawnY);

        auto& enemySprite = newEnemy->addComponent<dagger::SpriteComponent>();
        enemySprite.texture = enemyTexture;
        enemySprite.zIndex = 9;
        enemySprite.width = 48.0f;
        enemySprite.height = 48.0f;

        newEnemy->addComponent<dagger::VelocityComponent>();

        auto& eHealth = newEnemy->addComponent<HealthComponent>();
        eHealth.hp = 2;

        enemies_.push_back(newEnemy);
        renderSystem_.registerEntity(newEnemy);
    }

    // FPS Display
    fpsDisplay_ = createEntity();

    auto& uiTransform = fpsDisplay_->addComponent<dagger::TransformComponent>();
    uiTransform.setPosition(20.0f, 20.0f);

    auto& textComp = fpsDisplay_->addComponent<dagger::TextComponent>();
    textComp.setFont(uiFont);
    textComp.setColor(dagger::Colors::White);
    textComp.setText("FPS: 0");
    textComp.zIndex = 100;

    renderSystem_.registerEntity(camera_);
    renderSystem_.registerEntity(player_);
    renderSystem_.registerEntity(fpsDisplay_);
}

void GameplayScene::onFixedUpdate(float dt, dagger::Application& app)
{
    if (fpsDisplay_)
    {
        auto* textComp = fpsDisplay_->getComponent<dagger::TextComponent>();
        textComp->setText("FPS: " + std::to_string(app.getFPS()));
    }

    if (!player_) return;

    auto* pTransform = player_->getComponent<dagger::TransformComponent>();
    auto* pSprite = player_->getComponent<dagger::SpriteComponent>();
    auto* pVelocity = player_->getComponent<dagger::VelocityComponent>();
    auto* pInput = player_->getComponent<PlayerInputComponent>();
    auto* pHealth = player_->getComponent<HealthComponent>();
        
    if (pHealth && pHealth->invulnerabilityTimer > 0.0f)
    {
        pHealth->invulnerabilityTimer -= dt;
    }

    for (auto* enemy : enemies_)
    {
        if (enemy && enemy->isActive())
        {
            auto* eTransform = enemy->getComponent<dagger::TransformComponent>();
            auto* eVelocity = enemy->getComponent<dagger::VelocityComponent>();
            auto* eSprite = enemy->getComponent<dagger::SpriteComponent>();

            if (eTransform && eVelocity && eSprite)
            {
                float dirX = pTransform->x - eTransform->x;
                float dirY = pTransform->y - eTransform->y;
                float distance = std::sqrt(dirX * dirX + dirY * dirY);

                if (distance > 0.0f && distance < 400.0f)
                {
                    dirX /= distance;
                    dirY /= distance;

                    float enemySpeed = 120.0f;
                    eVelocity->vx = dirX * enemySpeed;
                    eVelocity->vy = dirY * enemySpeed;
                }
                else
                {
                    eVelocity->vx = 0.0f;
                    eVelocity->vy = 0.0f;
                }

                movementSystem_.update(dt, *eTransform, *eVelocity, *tileMap_, eSprite->width, eSprite->height);
            }

            if (pHealth && pHealth->invulnerabilityTimer <= 0.0f)
            {
                if (CollisionSystem::checkAABB(pTransform->x, pTransform->y, pSprite->width, pSprite->height,
                                               eTransform->x, eTransform->y, eSprite->width, eSprite->height))
                {
                    pHealth->hp -= 1;
                    pHealth->invulnerabilityTimer = 1.0f;

                    if (pHealth->hp <= 0)
                    {
                        app.setScene(std::make_unique<GameOverScene>());
                        return;
                    }
                }
            }
        }
    }

    if (pTransform && pSprite && pVelocity && pInput)
    {
        inputSystem_.update(app.getInput(), *pVelocity, *pInput);
        movementSystem_.update(dt, *pTransform, *pVelocity, *tileMap_, pSprite->width, pSprite->height);

        static float shootCooldown = 0.0f;
        shootCooldown -= dt;

        if (app.getInput().isLeftMouseDown() && shootCooldown <= 0.0f)
        {
            shootCooldown = 0.25f; 

            int mouseX, mouseY;
            app.getInput().getMousePosition(&mouseX, &mouseY);

            float camX = 0.0f; float camY = 0.0f;
            if (camera_) {
                auto* cTransform = camera_->getComponent<dagger::TransformComponent>();
                camX = cTransform->x;
                camY = cTransform->y;
            }

            float targetX = mouseX + camX;
            float targetY = mouseY + camY;
            float startX = pTransform->x + (pSprite->width / 2.0f);
            float startY = pTransform->y + (pSprite->height / 2.0f);
            
            float dirX = targetX - startX;
            float dirY = targetY - startY;
            float length = std::sqrt(dirX * dirX + dirY * dirY);
            if (length > 0) { dirX /= length; dirY /= length; }

            dagger::Entity* fireball = createEntity();
            auto& fTransform = fireball->addComponent<dagger::TransformComponent>();
            fTransform.setPosition(startX - 8.0f, startY - 8.0f); 

            auto& fSprite = fireball->addComponent<dagger::SpriteComponent>();
            fSprite.texture = app.getAssets().load<dagger::Texture>("fireball", "assets/crawl-tiles/spells/conjuration/orb_of_destruction.png"); 
            fSprite.width = 16.0f; fSprite.height = 16.0f; fSprite.zIndex = 5;

            auto& fVelocity = fireball->addComponent<dagger::VelocityComponent>();
            fVelocity.vx = dirX * 600.0f; 
            fVelocity.vy = dirY * 600.0f;

            projectiles_.push_back(fireball);
            renderSystem_.registerEntity(fireball);
        }

        // Projectile movement
        for (auto it = projectiles_.begin(); it != projectiles_.end(); )
        {
            dagger::Entity* proj = *it;
            auto* pTransform = proj->getComponent<dagger::TransformComponent>();
            auto* pVelocity = proj->getComponent<dagger::VelocityComponent>();
            auto* pSprite = proj->getComponent<dagger::SpriteComponent>();

            bool hitWall = movementSystem_.updateProjectile(dt, *pTransform, *pVelocity, *tileMap_, pSprite->width, pSprite->height);
            bool hitEnemy = false;

            for (auto enemyIt = enemies_.begin(); enemyIt != enemies_.end(); )
            {
                dagger::Entity* enemy = *enemyIt;
                
                if (enemy && enemy->isActive())
                {
                    auto* eTransform = enemy->getComponent<dagger::TransformComponent>();
                    auto* eSprite = enemy->getComponent<dagger::SpriteComponent>();

                    if (eTransform && eSprite &&
                        CollisionSystem::checkAABB(pTransform->x, pTransform->y, pSprite->width, pSprite->height,
                                                   eTransform->x, eTransform->y, eSprite->width, eSprite->height))
                    {
                        hitEnemy = true;
                        
                        auto* eHealth = enemy->getComponent<HealthComponent>();
                        if (eHealth)
                        {
                            eHealth->hp -= 1;
                            if (eHealth->hp <= 0)
                            {
                                enemy->setActive(false);
                                enemyIt = enemies_.erase(enemyIt);
                                break;
                            }
                        }
                    }
                    else
                    {
                        ++enemyIt;
                    }
                }
                else
                {
                    ++enemyIt;
                }
            }

            if (hitWall || hitEnemy)
            {
                proj->setActive(false); 
                it = projectiles_.erase(it); 
            }
            else
            {
                ++it; 
            }
        }

        if (camera_)
        {
            auto* camTransform = camera_->getComponent<dagger::TransformComponent>();
            if (camTransform)
            {
                camTransform->prevX = camTransform->x;
                camTransform->prevY = camTransform->y;

                float screenW = static_cast<float>(app.getWindowWidth());
                float screenH = static_cast<float>(app.getWindowHeight());

                camTransform->x = pTransform->x - (screenW / 2.0f) + (pSprite->width / 2.0f);
                camTransform->y = pTransform->y - (screenH / 2.0f) + (pSprite->height / 2.0f);
            }
        }
    }
}

void GameplayScene::onUpdate(float dt, dagger::Application& app)
{
}

void GameplayScene::onRender(dagger::Renderer &renderer, dagger::AssetManager &assets, float alpha)
{
    float camX = 0.0f;
    float camY = 0.0f;

    if (camera_)
    {
        auto* camTransform = camera_->getComponent<dagger::TransformComponent>();
        if (camTransform)
        {
            camX = camTransform->prevX + (camTransform->x - camTransform->prevX) * alpha;
            camY = camTransform->prevY + (camTransform->y - camTransform->prevY) * alpha;
        }
    }

    if (tileMap_)
    {
        auto* floor = assets.resolve<dagger::Texture>(floorTex_);
        auto* wall = assets.resolve<dagger::Texture>(wallTex_);
        if (floor && wall)
        {
            tileMap_->render(renderer, *floor, *wall, camX, camY);
        }
    }

    renderSystem_.render(renderer, assets, alpha);
}

} // namespace delve
