#pragma once
#include <filesystem>
#include "UniqueDir.hpp"

class TCompilerBase {
public:
    using path_t = std::filesystem::path;

private:
    TUniqueDir WorkDir;

protected:
    virtual path_t CompileHandler(path_t const & file) = 0;

public:
    TCompilerBase(path_t && path);
    virtual ~TCompilerBase() = default;

    path_t Compile(path_t const & file);
};

class THaskellCompiler : public TCompilerBase {
    path_t CompileHandler(path_t const & file) override;

public:
    THaskellCompiler(path_t const & path);
};