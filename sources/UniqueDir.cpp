#include "UniqueDir.hpp"

namespace fs = std::filesystem;

TUniqueDir::TUniqueDir(path_t path) 
    : Path()
{
    while (!fs::create_directory(path))
        path += "_";
    
    Path = std::move(path);
}

TUniqueDir::~TUniqueDir() {
    if (!Path.empty())
        fs::remove_all(Path);
}

auto TUniqueDir::path() const noexcept -> path_t const & {
    return Path;
}
