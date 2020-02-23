#include <iostream>
#include <fstream>
#include <algorithm>
#include "Utils.hpp"
#include "Compilers.hpp"

using namespace std;
namespace fs = filesystem;

namespace {

struct {
    bool Verbose = false;
    vector<string_view> RunArgs;
} opts;


enum class ELangs {
    undef, c, cpp, python, bash, csh, perl, tcl
};

ELangs LangPrediction(fs::path const & file, fs::path const & tmp_dir) {
    TCWDGuard cwd_guard(tmp_dir);
    system(("file -b " + file.string() + " >predicted_language").c_str());
    ifstream inp("predicted_language");
    string result;
    getline(inp, result);

    if (result.find("C source") != string::npos)
        return ELangs::c;

    if (result.find("C++ source") != string::npos)
        return ELangs::cpp;

    if (result.find("Python script") != string::npos)
        return ELangs::python;

    if (result.find("Bourne-Again shell script") != string::npos)
        return ELangs::bash;

    if (result.find("a /usr/bin/sh script") != string::npos)
        return ELangs::csh;

    if (result.find("Tcl script") != string::npos)
        return ELangs::tcl;

    if (result.find("Perl script") != string::npos)
        return ELangs::perl;

    return ELangs::undef;
}

unique_ptr<TCompilerBase> GetCompiler(ELangs lang, fs::path const & tmp_dir) {
    unique_ptr<TCompilerBase> compiler;
    {
        TCWDGuard cwd_guard(tmp_dir);
        switch (lang) {
            case ELangs::c      : compiler = make_unique<TCompilerComposer<TCCompiler, TCPPCompiler, TDCompiler>>(tmp_dir); break;
            case ELangs::cpp    : compiler = make_unique<TCompilerComposer<TCPPCompiler, TCCompiler, TDCompiler>>(tmp_dir); break;
            case ELangs::bash   : [[fallthrough]];
            case ELangs::csh    : [[fallthrough]];
            case ELangs::perl   : [[fallthrough]];
            case ELangs::tcl    : [[fallthrough]];
            case ELangs::python : compiler = make_unique<TOnlyCopyCompiler>(tmp_dir);                                       break;
            default: compiler = make_unique<TCompilerComposer<THaskellCompiler, TPascalCompiler, TFortranCompiler, TAdaCompiler, TOCamlCompiler, TDCompiler, TCPPCompiler, TCCompiler, TLispCompiler>>(tmp_dir); break;
        }
    }
    return compiler;
}

auto ParseOpts(int argc, char const *argv[]) {
    vector<fs::path> files;
    for (int i = 1; i < argc; ++i) {
        string_view s(argv[i]);
        if (s.empty())
            continue;

        if (s[0] == '-') {
            if (s == "-v" || s == "--verbose") {
                opts.Verbose = true;
                continue;
            }

            if (s == "--args") {
                for (++i; i < argc; ++i)
                    opts.RunArgs.push_back(argv[i]);

                break;
            }

            cerr << "error: unknown option: " << s << '\n';
            exit(1);
        }

        files.push_back(s);
    }

    return files;
}

bool CheckFilesCorrectens(vector<fs::path> const & files) {
    bool ok = true;
    for (auto const & file : files) {
        if (!fs::exists(file)) {
            cerr << "error: " << file << " does not exist\n";
            ok = false;
        }
        if (!fs::is_regular_file(file)) {
            cerr << file << " is not a file\n";
            ok = false;
        }
    }

    return ok;
}

void ConvertToCanonical(vector<fs::path> & files) {
    for (auto & file : files)
        file = fs::canonical(file);
}

} // namespace


#include <thread>

int main(int argc, char const *argv[]) {
    auto files = ParseOpts(argc, argv);
    if (files.empty()) {
        cerr << "using: ucc [-v] file1 file2 ... [--args arg1 arg2 ...]\n";
        return 1; 
    }

    if (!CheckFilesCorrectens(files))
        return 2;

    try {
        ConvertToCanonical(files);

        TUniqueDir tmp_dir;
        auto compiler = GetCompiler(LangPrediction(files[0], tmp_dir.path()), tmp_dir.path());

        auto runner = compiler->Compile(files, opts.Verbose);

        if (!runner)
            return -1;

        runner->Run(opts.RunArgs);

    } catch (exception const & e) {
        cerr << "oops, error, what:\n";
        cerr << e.what() << '\n';
        return -1;
    } catch (...) {
        cerr << "unknown error\n";
    }
    return 0;
}
