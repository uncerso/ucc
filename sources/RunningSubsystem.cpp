#include "RunningSubsystem.hpp"

using namespace std;
namespace fs = filesystem;

std::string TRunnerBase::RunHandler(path_t const & file) const {
    return file.string();
}

TRunnerBase::TRunnerBase(path_t const & file) 
    : File(WorkDir.path() / file.filename())
{
    fs::rename(file, File);
}

void TRunnerBase::Run(std::vector<std::string_view> const & args) const {
    auto command = RunHandler(File) + ConvertToString(args);
    system(command.c_str());
}
