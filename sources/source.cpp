#include <iostream>
#include <fstream>
#include <algorithm>
#include "Utils.hpp"
#include "Compilers.hpp"

using namespace std;
namespace fs = filesystem;

enum class ELangs {
    undef, c, cpp, haskell, pascal, fortran, d
};

ELangs LangPrediction(fs::path const & file) {
    system(("file -b " + file.string() + " >predicted_language").c_str());
    ifstream inp("predicted_language");
    string result;
    getline(inp, result);

    if (result.find("C source") != string::npos)
        return ELangs::c;

    if (result.find("C++ source") != string::npos)
        return ELangs::cpp;

    return ELangs::undef;
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        cerr << "using: ucc file_name\n";
        return 1; 
    }

    fs::path file_name = argv[1];
    if (!fs::exists(file_name)) {
        cerr << "file " << file_name << " does not exist\n";
        return 2;
    }

    if (!fs::is_regular_file(file_name)) {
        cerr << file_name << " is not a file\n";
        return 3;
    }

    try {
        file_name = fs::canonical(file_name);

        TUniqueDir tmp_dir;
        unique_ptr<TCompilerBase> compiler;
        {
            TCWDGuard cwd_guard(tmp_dir.path());
            switch (LangPrediction(file_name)) {
                case ELangs::c      : compiler = make_unique<TCompilerComposer<TCCompiler, TCPPCompiler, TDCompiler>>(tmp_dir.path()); break;
                case ELangs::cpp    : compiler = make_unique<TCompilerComposer<TCPPCompiler, TCCompiler, TDCompiler>>(tmp_dir.path()); break;
                default: compiler = make_unique<TCompilerComposer<THaskellCompiler, TPascalCompiler, TFortranCompiler, TAdaCompiler, TDCompiler, TCPPCompiler, TCCompiler>>(tmp_dir.path()); break;
            }
        }

        auto result = compiler->Compile(file_name);

        if (!result.empty()) {
            auto dist = fs::current_path() / result.filename();
            fs::remove(dist);
            fs::copy_file(result, dist);
        }
    } catch (exception const & e) {
        cerr << e.what() << '\n';
        return -1;
    } catch (...) {
        cerr << "unknown error\n";
    }
    return 0;
}
