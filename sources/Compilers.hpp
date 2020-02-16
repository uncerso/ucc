#pragma once
#include <filesystem>
#include <array>
#include "Utils.hpp"

class TCompilerBase {
public:
    using path_t = std::filesystem::path;

private:
    TUniqueDir WorkDir;

protected:
    virtual std::string CompileHandler(path_t const & file) const = 0;

public:
    TCompilerBase(path_t && path);
    virtual ~TCompilerBase() = default;

    virtual path_t Compile(path_t const & file) const;
};

#define DECLARE_COMPILER(name)                                      \
class T##name##Compiler : public TCompilerBase {                    \
    std::string CompileHandler(path_t const & file) const override; \
public:                                                             \
    T##name##Compiler(path_t const & path);                         \
};

DECLARE_COMPILER(Haskell)
DECLARE_COMPILER(C)
DECLARE_COMPILER(CPP)
DECLARE_COMPILER(Pascal)
DECLARE_COMPILER(Fortran)
DECLARE_COMPILER(D)
DECLARE_COMPILER(Ada)
DECLARE_COMPILER(OCaml)
DECLARE_COMPILER(OnlyCopy)

template <class ... Args>
class TCompilerComposer : public TCompilerBase {
    using path_t = std::filesystem::path;
    using Compilers_t = std::array<std::unique_ptr<TCompilerBase>, sizeof...(Args)>;
    Compilers_t Compilers;

public:
    TCompilerComposer(path_t const & path) 
        : TCompilerBase(path / "composer")
        , Compilers({std::make_unique<Args>(path / "composer")...})
    {}

    path_t Compile(path_t const & file) const override {
        path_t result;
        for (auto const & compiler : Compilers) {
            result = compiler->Compile(file);
            if (!result.empty())
                return result;
        }
        return result;
    }

private:
    std::string CompileHandler(path_t const & file) const override { return{}; };
};