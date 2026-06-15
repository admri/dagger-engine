#ifndef DAGGER_CORE_COMPONENTS_TAGS_H
#define DAGGER_CORE_COMPONENTS_TAGS_H

#include <dagger/core/Component.h>

namespace dagger
{

struct HiddenTag : public Component {};
struct ShadowTag : public Component {};
struct UIRenderTag : public Component {};

} // namespace dagger

#endif // DAGGER_CORE_COMPONENTS_TAGS_H
