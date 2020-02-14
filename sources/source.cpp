#include <iostream>
#include "UniqueDir.hpp"
#include "Compilers.hpp"

using namespace std;
namespace fs = filesystem;

namespace NCompilers {

int Haskell (string const & name) {
    using namespace string_literals;
    filesystem::path file(name);
    
    string command = "ghc -dynamic "s + name + " && rm hs.hi hs.o"; 
    return system(command.c_str());
}

}

enum class ELangs {
    undef, c, cpp, haskell, bash, python
};

ELangs LangDefiner(string const & path_name) {
    filesystem::path path(path_name);
    if (path.has_extension() && path.extension() == "hs") {
        return ELangs::haskell;
    }
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

        THaskellCompiler x(tmp_dir.path()); 
        auto result = x.Compile(file_name);

        if (!result.empty()) {
            auto dist = fs::current_path() / result.filename();
            fs::remove(dist);
            fs::copy_file(result, dist);
        }
    } catch (exception const & e) {
        cerr << e.what() << '\n';
        return -1;
    }
    return 0;
}
