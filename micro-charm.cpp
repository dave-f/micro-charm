// micro-charm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include "boost/program_options.hpp"
#include "Compiler.h"
#include "BBCMicroWriter.h"

#define PROGRAM_TITLE       "Micro charm"
#define VERSION_NUMBER      "0.1"
#define PROGRAM_USAGE       "Usage: charm [options] <input file> <output file>"
#define PROGRAM_DESCRIPTION "A compiler for building simple text adventures"

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
    string  infile;
    string  outfile;
    BackEnd backendType;
    bool    verbose;
};

void displayUsage(po::options_description& d)
{
    cout << PROGRAM_TITLE << " " << VERSION_NUMBER << endl;
    cout << PROGRAM_DESCRIPTION << endl;
    cout << PROGRAM_USAGE << endl << endl;
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
    string destFile;
    string backend;

    desc.add_options()
        ("help,h",                                      "produce this help message")
        ("verbose,v",                                   "enable verbose mode")
        ("backend,b",   po::value<string>(&backend),    "set backend writer");

    hidden.add_options()
        ("input,i",     po::value<string>(&sourceFile), "set source file")
        ("output,o",    po::value<string>(&destFile),   "set destination file");

    pdesc.add("input",1);
    pdesc.add("output",2);

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

    if (vm.count("help") || vm.size()==0 || vm.count("input")==0 || vm.count("output")==0)
    {
        displayUsage(desc);

        return false;
    }
    else
    {
        options.infile      = sourceFile;
        options.outfile     = destFile;
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

        if (c.compileFile(options.infile))
        {
            unique_ptr<Writer> w;

            switch(options.backendType)
            {
                case BBCMicro:
                default:
                    // Default to BBC for now
                    w.reset(new BBCMicroWriter());
                    break;
            }
            
            w->writeFile(c, options.outfile);
        }

        return 1;
    }

    return 0;
}
