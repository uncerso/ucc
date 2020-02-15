#include "Compilers.hpp"
#include <utility>
#include <string>
#include <iostream>

using namespace std;
namespace fs = filesystem;

namespace {
    constexpr auto Result_file_name = "out";

    fs::path MakeSymlink(fs::path const & file, char const * wishes_extension) {
        auto sym_path = fs::current_path() / (Result_file_name + "."s + wishes_extension);
        fs::create_symlink(file, sym_path);
        return sym_path;
    }
}

TCompilerBase::TCompilerBase(path_t && path) 
    : WorkDir(move(path))
{}

auto TCompilerBase::Compile(path_t const & file) const -> path_t {
    cout << "Trying to compile as " << WorkDir.path().filename().c_str() << endl;
    TCWDGuard cwd_guard(WorkDir.path());
    path_t result_path;
    auto command = CompileHandler(file) + " &> /dev/null";
    if (!system(command.c_str())) {
        result_path = fs::current_path() / Result_file_name;
        cout << "Success" << endl;
    } else {
        cout << "Failed\n" << endl;
    }

    return result_path;
}

THaskellCompiler::THaskellCompiler(path_t const & path) 
    : TCompilerBase(path / "haskell")
{}

string THaskellCompiler::CompileHandler(path_t const & file) const {
    auto new_file = MakeSymlink(file, "hs");
    return "ghc -dynamic -O2 "s + new_file.filename().c_str() + " -o "s + Result_file_name;
}

TCCompiler::TCCompiler(path_t const & path) 
    : TCompilerBase(path / "C")
{}

string TCCompiler::CompileHandler(path_t const & file) const {
    auto new_file = MakeSymlink(file, "c");
    return "gcc -std=c11 -O3 "s + new_file.filename().c_str() + " -o "s + Result_file_name;
}

TCPPCompiler::TCPPCompiler(path_t const & path) 
    : TCompilerBase(path / "CPP")
{}

string TCPPCompiler::CompileHandler(path_t const & file) const {
    auto new_file = MakeSymlink(file, "cpp");
    return "g++ -std=c++11 -O3 "s + new_file.filename().c_str() + " -o "s + Result_file_name;
}

TPaskalCompiler::TPaskalCompiler(path_t const & path) 
    : TCompilerBase(path / "paskal")
{}

string TPaskalCompiler::CompileHandler(path_t const & file) const {
    auto new_file = MakeSymlink(file, "pas");
    return "fpc -O3 "s + new_file.filename().c_str() + " -o" + Result_file_name;
}

TFortranCompiler::TFortranCompiler(path_t const & path) 
    : TCompilerBase(path / "fortran")
{}

string TFortranCompiler::CompileHandler(path_t const & file) const {
    auto new_file = MakeSymlink(file, "f90");
    return "gfortran -O3 "s + new_file.filename().c_str() + " -o "s + Result_file_name;
}