#pragma once
#include <filesystem>

class TUniqueDir {
    using path_t = std::filesystem::path; 
    path_t Path;
public:
    TUniqueDir(path_t path = std::filesystem::temp_directory_path()/"ucc");
    ~TUniqueDir();
    path_t const & path() const noexcept;
};

class TCWDGuard {
private:
    using path_t = std::filesystem::path; 
    path_t Path_to_restore;

public:
    TCWDGuard(path_t const & wishes_path);
    ~TCWDGuard();
};
