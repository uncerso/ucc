#include "Utils.hpp"

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

TCWDGuard::TCWDGuard(path_t const & wishes_path) 
    : Path_to_restore(fs::current_path())
{
    fs::current_path(wishes_path);
}

TCWDGuard::~TCWDGuard() {
    fs::current_path(Path_to_restore);
}
