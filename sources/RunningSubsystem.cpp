#include "RunningSubsystem.hpp"

using namespace std;
namespace fs = filesystem;

TRunnerBase::TRunnerBase(path_t const & file) 
    : File(WorkDir.path() / file.filename())
{
    fs::rename(file, File);
}

string TRunnerBase::RunHandler(path_t const & file) const {
    return file.string();
}

void TRunnerBase::Run(vector<string_view> const & args) const {
    fs::permissions(File, fs::perms::owner_exec, fs::perm_options::add);
    auto command = RunHandler(File) + ConvertToString(args);
    system(command.c_str());
}

TLispRunner::TLispRunner(path_t const & file) 
    : TRunnerBase(file)
{}

std::string TLispRunner::RunHandler(path_t const & file) const {
    return "clisp " + file.string();
}
