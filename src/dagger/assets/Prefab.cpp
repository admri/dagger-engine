#include <assets/Prefab.h>

#include <assets/PrefabFactory.h>

#include <fstream>

namespace dagger
{

bool Prefab::load(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    data = nlohmann::json::parse(file, nullptr, false);
    if (data.is_discarded()) return false;

    PrefabFactory::compile(*this, data);
    return true;
}

} // namespace dagger
