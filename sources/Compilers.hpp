#pragma once
#include <filesystem>
#include <vector>
#include <array>
#include "Utils.hpp"
#include "RunningSubsystem.hpp"

class TCompilerBase {
public:
    using path_t = std::filesystem::path;
    using return_t = std::unique_ptr<TRunnerBase>;

private:
    TUniqueDir WorkDir;

    mutable std::vector<path_t> CreatedFiles;
protected:
    virtual std::string CompileHandler(std::vector<path_t> const & files) const = 0;
    std::vector<path_t> MakeSymlinks(std::vector<path_t> const & files, std::string_view wishes_extension) const;
public:
    TCompilerBase(path_t && path);
    virtual ~TCompilerBase();

    virtual return_t Compile(std::vector<path_t> const & files, bool verbose) const;
};

#define DECLARE_COMPILER(name)                                                      \
class T##name##Compiler : public TCompilerBase {                                    \
    std::string CompileHandler(std::vector<path_t> const & files) const override;   \
public:                                                                             \
    T##name##Compiler(path_t const & path);                                         \
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
    using Compilers_t = std::array<std::unique_ptr<TCompilerBase>, sizeof...(Args)>;
    Compilers_t Compilers;

public:
    TCompilerComposer(path_t const & path) 
        : TCompilerBase(path / "composer")
        , Compilers({std::make_unique<Args>(path / "composer")...})
    {}

    return_t Compile(std::vector<path_t> const & files, bool verbose) const override {
        return_t result;
        for (auto const & compiler : Compilers) {
            result = compiler->Compile(files, verbose);
            if (result) 
                return result;
        }
        return result;
    }

private:
    std::string CompileHandler(std::vector<path_t> const &) const override { return{}; };
};