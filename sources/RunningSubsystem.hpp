#pragma once 
#include <filesystem>
#include "Utils.hpp"
#include <vector>

class TRunnerBase {
public:
    using path_t = std::filesystem::path;
private:
    TUniqueDir WorkDir;
    path_t File;
protected:
    virtual std::string RunHandler(path_t const & file) const;
public:
    TRunnerBase(path_t const & file);
    
    void Run(std::vector<std::string_view> const & args) const;
    
    virtual ~TRunnerBase() = default;
};
