#include <assets/PrefabFactory.h>

#include <assets/AssetManager.h>
#include <assets/Font.h>
#include <assets/Prefab.h>
#include <core/Scene.h>
#include <core/components/AnimationComponent.h>
#include <core/components/BoxColliderComponent.h>
#include <core/components/CameraComponent.h>
#include <core/components/CameraTargetComponent.h>
#include <core/components/SpriteComponent.h>
#include <core/components/Tags.h>
#include <core/components/TextComponent.h>
#include <core/components/TransformComponent.h>
#include <core/components/VelocityComponent.h>
#include <graphics/Texture.h>

#include <nlohmann/json.hpp>

namespace dagger
{

std::unordered_map<std::string, ComponentBuilderFn> PrefabFactory::builders_;

void PrefabFactory::registerComponent(const std::string& name, ComponentBuilderFn builder)
{
    builders_[name] = builder;
}

void PrefabFactory::compile(Prefab& prefab, const nlohmann::json& data)
{
    if (!data.contains("components") || !data["components"].is_object())
    {
        return; 
    }

    for (auto& [key, value] : data["components"].items())
    {
        auto it = builders_.find(key);
        if (it != builders_.end())
        {
            ComponentBuilderFn builder = it->second;
            
            prefab.compiledBuilders.push_back([builder, value](Entity* e, AssetManager& a) {
                builder(e, value, a);
            });
        }
    }
}

Entity* PrefabFactory::create(PrefabHandle handle, Scene& scene, AssetManager& assets)
{
    auto* prefab = assets.resolve<Prefab>(handle);
    if (!prefab) return nullptr;

    auto entity = scene.createEntity();
    
    for (const auto& buildFn : prefab->compiledBuilders)
    {
        buildFn(entity, assets);
    }
    return entity;
}

void PrefabFactory::registerCoreComponents()
{
    registerComponent<BoxColliderComponent>("BoxColliderComponent");
    registerComponent<CameraComponent>("CameraComponent");
    registerComponent<VelocityComponent>("VelocityComponent");

    registerComponent("AnimationComponent", [](Entity* e, const nlohmann::json& j, AssetManager& a) 
    {
        auto& comp = e->addComponent<AnimationComponent>();
        j.get_to(comp); 

        if (j.contains("sequences") && j["sequences"].is_array()) 
        {
            for (const auto& seqJson : j["sequences"]) 
            {
                AnimationComponent::AnimationSequence seq;
                
                seq.startFrame = seqJson.value("startFrame", 0);
                seq.numFrames = seqJson.value("numFrames", 1);
                seq.frameDuration = seqJson.value("frameDuration", 0.1f);
                seq.isLooping = seqJson.value("isLooping", true);

                if (seqJson.contains("texture")) 
                {
                    seq.texture = a.get<Texture>(seqJson["texture"].get<std::string>());
                }

                comp.sequences.push_back(seq);
            }
        }
    });

    registerComponent("SpriteComponent", [](Entity* e, const nlohmann::json& j, AssetManager& a)
    {
        auto& comp = e->addComponent<SpriteComponent>();
        j.get_to(comp); 

        if (j.contains("texture")) 
        {
            comp.texture = a.get<Texture>(j["texture"].get<std::string>());
        }
    });
    
    registerComponent("TextComponent", [](Entity* e, const nlohmann::json& j, AssetManager& a)
    {
        auto& comp = e->addComponent<TextComponent>();
        j.get_to(comp); 

        if (j.contains("font")) 
        {
            comp.font = a.get<Font>(j["font"].get<std::string>());
        }

        if (j.contains("color"))
        {
            comp.color.r = j["color"].value("r", 255);
            comp.color.g = j["color"].value("g", 255);
            comp.color.b = j["color"].value("b", 255);
            comp.color.a = j["color"].value("a", 255);
        }
    });

    registerComponent("TransformComponent", [](Entity* e, const nlohmann::json& j, AssetManager&) 
    {
        auto& comp = e->addComponent<TransformComponent>();
        
        float startX = j.value("x", 0.0f);
        float startY = j.value("y", 0.0f);
        
        comp.setPosition(startX, startY);
    });
    
    registerTag<CameraTargetComponent>("CameraTargetComponent");
    registerTag<HiddenTag>("HiddenTag");
    registerTag<ShadowTag>("ShadowTag");
    registerTag<UIRenderTag>("UIRenderTag");
}

} // namespace dagger
