#pragma once
#include <filesystem>
#include <sstream>
#include <vector>

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

template<class T>
std::string ConvertToString(std::vector<T> const & items) {
    thread_local static std::stringstream ss;
    ss.str("");
    ss << ' ';
    for (auto const & s : items) 
        ss << s << ' ';
    
    return ss.str();
}