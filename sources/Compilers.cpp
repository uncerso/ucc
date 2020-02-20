#include "Compilers.hpp"
#include <utility>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
namespace fs = filesystem;

namespace {
    constexpr auto Result_file_name = "out";
}

auto TCompilerBase::MakeSymlinks(vector<path_t> const & files, string_view wishes_extension) const -> vector<path_t> {
    vector<fs::path> sym_paths;
    sym_paths.reserve(files.size());
    for (auto const & file : files) {
        if (file.has_extension() && file.extension().string() == wishes_extension) {
            sym_paths.push_back(file);
            continue;
        }

        auto sym_path = file;
        sym_path += wishes_extension;
        fs::create_symlink(file, sym_path);
        CreatedFiles.push_back(sym_path);
        sym_paths.push_back(move(sym_path));
    }
    return sym_paths;
}

TCompilerBase::TCompilerBase(path_t && path) 
    : WorkDir(move(path))
{}

TCompilerBase::~TCompilerBase() {
    for (auto const & file : CreatedFiles)
        fs::remove(file);
}

auto TCompilerBase::Compile(vector<path_t> const & files, bool verbose) const -> return_t {
    #define PRINT(text) if (verbose) cerr << text << endl

    PRINT("Trying to compile as " << WorkDir.path().filename().c_str());
    TCWDGuard cwd_guard(WorkDir.path());
    return_t result;
    auto command = CompileHandler(files) + (verbose ? "" : " &> /dev/null");
    if (!system(command.c_str())) {
        result = make_unique<TRunnerBase>(fs::current_path() / Result_file_name);
        PRINT("Success\n");
    } else {
        PRINT("Failed\n");
    }
    return result;

    #undef PRINT
}

THaskellCompiler::THaskellCompiler(path_t const & path) 
    : TCompilerBase(path / "haskell")
{}

string THaskellCompiler::CompileHandler(vector<path_t> const & files) const {
    auto new_files = MakeSymlinks(files, ".hs");
    return "ghc -dynamic -O2 "s + ConvertToString(new_files) + " -o "s + Result_file_name;
}

TCCompiler::TCCompiler(path_t const & path) 
    : TCompilerBase(path / "C")
{}

string TCCompiler::CompileHandler(vector<path_t> const & files) const {
    auto new_files = MakeSymlinks(files, ".c");
    return "gcc -std=c11 -O3 "s + ConvertToString(new_files) + " -o "s + Result_file_name;
}

TCPPCompiler::TCPPCompiler(path_t const & path) 
    : TCompilerBase(path / "CPP")
{}

string TCPPCompiler::CompileHandler(vector<path_t> const & files) const {
    auto new_files = MakeSymlinks(files, ".cpp");
    return "g++ -std=c++17 -O3 "s + ConvertToString(new_files) + " -o "s + Result_file_name;
}

TPascalCompiler::TPascalCompiler(path_t const & path) 
    : TCompilerBase(path / "pascal")
{}

string TPascalCompiler::CompileHandler(vector<path_t> const & files) const {
    auto new_files = MakeSymlinks(files, ".pas");
    return "fpc -O3 "s + ConvertToString(new_files) + " -o" + Result_file_name;
}

TFortranCompiler::TFortranCompiler(path_t const & path) 
    : TCompilerBase(path / "fortran")
{}

string TFortranCompiler::CompileHandler(vector<path_t> const & files) const {
    auto new_files = MakeSymlinks(files, ".f90");
    return "gfortran -O3 "s + ConvertToString(new_files) + " -o "s + Result_file_name;
}

TDCompiler::TDCompiler(path_t const & path) 
    : TCompilerBase(path / "D")
{}

string TDCompiler::CompileHandler(vector<path_t> const & files) const {
    auto new_files = MakeSymlinks(files, ".d");
    return "dmd -O "s + ConvertToString(new_files) + " -of="s + Result_file_name;
}

TAdaCompiler::TAdaCompiler(path_t const & path) 
    : TCompilerBase(path / "ada")
{}

string TAdaCompiler::CompileHandler(vector<path_t> const & files) const {
    auto new_files = MakeSymlinks(files, ".adb");
    return "gnat make "s + ConvertToString(new_files) + " -o "s + Result_file_name;
}

TOCamlCompiler::TOCamlCompiler(path_t const & path) 
    : TCompilerBase(path / "ocaml")
{}

string TOCamlCompiler::CompileHandler(vector<path_t> const & files) const {
    auto new_files = MakeSymlinks(files, ".ml");
    return "ocamlc "s + ConvertToString(new_files) + " -o "s + Result_file_name;
}

TOnlyCopyCompiler::TOnlyCopyCompiler(path_t const & path) 
    : TCompilerBase(path / "copy")
{}

string TOnlyCopyCompiler::CompileHandler(vector<path_t> const & files) const {
    if (files.size() != 1)
        return "false";

    return "cp \""s + files[0].c_str() + "\" "s + Result_file_name + " && chmod +x "s + Result_file_name ;
}