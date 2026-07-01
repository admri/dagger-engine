#ifndef DAGGER_CORE_SERIALIZATION_H
#define DAGGER_CORE_SERIALIZATION_H

#include <nlohmann/json.hpp>

/// @brief Generates boilerplate nlohmann::json deserialization code for a Component.
/// Place this macro at the bottom of your component struct file.
#define DAGGER_SERIALIZE_COMPONENT NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT

#endif // DAGGER_CORE_SERIALIZATION_H
