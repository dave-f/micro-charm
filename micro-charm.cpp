// micro-charm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include "boost/program_options.hpp"
#include "Compiler.h"

#define PROGRAM_TITLE       "Micro charm"
#define VERSION_NUMBER      "0.1"
#define PROGRAM_DESCRIPTION "A compiler for building basic text adventures"

using namespace std;
namespace po = boost::program_options;

enum BackEnd : uint8_t
{
    Undefined = 0,
    Spectrum,
    BBCMicro
};

struct ProgramOptions
{
    string filename;
    BackEnd backendType;
    bool verbose;
};

void displayUsage(po::options_description& d)
{
    cout << PROGRAM_TITLE << " " << VERSION_NUMBER << endl;
    cout << PROGRAM_DESCRIPTION << endl;
    cout << endl << d << endl;
}

bool parseCommandLine(int argc, _TCHAR* argv[], ProgramOptions& options)
{
    po::options_description desc("Available options");
    po::options_description hidden("Hidden options");
    po::options_description all("Merged options");

    po::positional_options_description pdesc;
    po::variables_map vm;

    string sourceFile;
    string backend;

    desc.add_options()
        ("help,h",                                      "produce this help message")
        ("verbose,v",                                   "enable verbose mode")
        ("backend,b",   po::value<string>(&backend),    "set backend writer");

    hidden.add_options()
        ("input,i",     po::value<string>(&sourceFile), "set source file");

    pdesc.add("input",1);

    try
    {
        all.add(desc).add(hidden);
        po::store(po::basic_command_line_parser<TCHAR>(argc, argv).options(all).positional(pdesc).run(), vm);
        po::notify(vm);
    }
    catch (...)
    {
        displayUsage(desc);

        return false;
    }

    if (vm.count("help") || vm.size()==0 || vm.count("input")==0)
    {
        displayUsage(desc);

        return false;
    }
    else
    {
        options.filename    = sourceFile;
        options.verbose     = vm.count("verbose") > 0;
        options.backendType = Undefined;

        return true;
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    ProgramOptions options;

    if (parseCommandLine(argc,argv,options))
    {
        Compiler c;

        if (c.compileFile(options.filename))
        {
            // Writer w;
            // w.writeFile();
        }

        return 1;
    }

    return 0;
}
