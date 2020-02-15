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

class THaskellCompiler : public TCompilerBase {
    std::string CompileHandler(path_t const & file) const override;

public:
    THaskellCompiler(path_t const & path);
};

class TCCompiler : public TCompilerBase {
    std::string CompileHandler(path_t const & file) const override;

public:
    TCCompiler(path_t const & path);
};

class TCPPCompiler : public TCompilerBase {
    std::string CompileHandler(path_t const & file) const override;

public:
    TCPPCompiler(path_t const & path);
};

class TPascalCompiler : public TCompilerBase {
    std::string CompileHandler(path_t const & file) const override;

public:
    TPascalCompiler(path_t const & path);
};

class TFortranCompiler : public TCompilerBase {
    std::string CompileHandler(path_t const & file) const override;

public:
    TFortranCompiler(path_t const & path);
};

class TDCompiler : public TCompilerBase {
    std::string CompileHandler(path_t const & file) const override;

public:
    TDCompiler(path_t const & path);
};

class TAdaCompiler : public TCompilerBase {
    std::string CompileHandler(path_t const & file) const override;

public:
    TAdaCompiler(path_t const & path);
};

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