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

#define DECLARE_RUNNER(name)                                    \
class T##name##Runner : public TRunnerBase {                    \
    std::string RunHandler(path_t const & file) const override; \
public:                                                         \
    T##name##Runner(path_t const & path);                       \
};

DECLARE_RUNNER(Lisp)
