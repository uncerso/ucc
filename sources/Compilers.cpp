#include "Compilers.hpp"
#include <utility>
#include <string>

using namespace std;
namespace fs = filesystem;

namespace {
    fs::path MakeSymlink(fs::path const & file, char const * wishes_extension) {
        auto sym_path = fs::current_path() / ("out."s + wishes_extension);
        fs::create_symlink(file, sym_path);
        return sym_path;
    }
}

TCompilerBase::TCompilerBase(path_t && path) 
    : WorkDir(move(path))
{}

auto TCompilerBase::Compile(path_t const & file) -> path_t {
    TCWDGuard cwd_guard(WorkDir.path());
    auto result_path = CompileHandler(file);
    return result_path;
}

THaskellCompiler::THaskellCompiler(path_t const & path) 
    : TCompilerBase(path / "haskell")
{}

auto THaskellCompiler::CompileHandler(path_t const & file) -> path_t {
    using namespace string_literals;
    path_t result_file;
    auto new_file = MakeSymlink(file, "hs");
    string command = "ghc -dynamic "s + new_file.filename().c_str();
    if (!system(command.c_str()))
        result_file = fs::current_path() / new_file.stem(); // remove extension

    return result_file;
}

TCCompiler::TCCompiler(path_t const & path) 
    : TCompilerBase(path / "C")
{}

auto TCCompiler::CompileHandler(path_t const & file) -> path_t {
    using namespace string_literals;
    path_t result_file;
    auto new_file = MakeSymlink(file, "c");
    string command = "gcc -std=c11 -O3 "s + new_file.filename().c_str() + " -o "s + new_file.stem().c_str();
    if (!system(command.c_str()))
        result_file = fs::current_path() / new_file.stem(); // remove extension

    return result_file;
}

TCPPCompiler::TCPPCompiler(path_t const & path) 
    : TCompilerBase(path / "CPP")
{}

auto TCPPCompiler::CompileHandler(path_t const & file) -> path_t {
    using namespace string_literals;
    path_t result_file;
    auto new_file = MakeSymlink(file, "cpp");
    string command = "g++ -std=c++11 -O3 "s + new_file.filename().c_str() + " -o "s + new_file.stem().c_str();
    if (!system(command.c_str()))
        result_file = fs::current_path() / new_file.stem(); // remove extension

    return result_file;
}