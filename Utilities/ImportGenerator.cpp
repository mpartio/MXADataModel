///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////

// Wheret to find our code templates
#include <UtilityFileLocations.h>
#include <MXA/Utilities/MXAFileSystemPath.h>

// STL includes
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/iostreams/device/file.hpp>

////-- Boost Filesystem Headers
//#include <boost/filesystem/operations.hpp>
//#include <boost/filesystem/convenience.hpp>
//#include <boost/filesystem/path.hpp>
//#include <boost/filesystem/fstream.hpp>


// Boost program options
#include <boost/program_options.hpp>
namespace ProgramOptions = boost::program_options;


/**
 * @namespace ImportGenerator
 */
namespace ImportGenerator
{

  const std::string ImportDelegateOutputHeader("Delegate.h");
  const std::string ImportDelegateOutputSource("Delegate.cpp");
  const std::string ImportDelegateFactoryOutputHeader("DelegateFactory.h");
  const std::string ImportDelegateFactoryOutputSource("DelegateFactory.cpp");
  const std::string CMakeListsOutput("CMakeLists.txt");
  const std::string MainOutput("Main.cpp");
  const std::string FindMXADataModelOutput("FindMXADataModel.cmake");
  const std::string OutputDir("outputdir");
  const std::string ProjectName("projectname");
}



#define GET_OPTION( option)\
  if (vm.count(option.toStdString() ) ) {\
    options.insert(option, QString::fromStdString( vm[option.toStdString() ].as<std::string>() )   ); \
    std::cout << "Adding Option: " << option.toStdString() << " --> " <<  vm[option.toStdString() ].as<std::string>() << std::endl;\
    } \



/**
 * @brief This method will parse the template code file, perform the necessary replacements
 * and then write out the new file to the proper location based on the output directory
 * and the project name.
 * @param inputFile The input template file
 * @param outputFile The output source code file to generate
 * @param projectName The name of the project.
 * @return Error condition. Zero is NO error.
 */
int parseTemplate(const std::string &inputFile,
                  const std::string &outputFile,
                  const std::string &projectName)
{
  //std::cout << "inputFile: " << inputFile << std::endl;
  //std::cout << "Generating: " << outputFile << std::endl;
  //std::cout << "-----------------------------------------------" << std::endl;
  //std::cout << "projectName: " << projectName << std::endl;


  int err = 0;
  boost::iostreams::file_source source(inputFile, std::ios_base::in);
  if (source.is_open() == false)
  {
    std::cout << "the source file could not be opened for reading.'" << inputFile << "'" << std::endl;
    return -1;
  }


  std::ifstream configFile(inputFile.c_str(), std::ios::binary);
  if (configFile.fail() == true)
  {
    std::cout << "Could not open file '" << inputFile << "'" << std::endl;
    return -1;
  }
  // Read the whole file into a string
  std::stringstream ss;
  ss << configFile.rdbuf();
  std::string str(ss.str());


  boost::algorithm::replace_all(str, "@PROJECT_NAME@", projectName);

  std::ofstream out;
  out.open(outputFile.c_str(), std::ios::out | std::ios::trunc);
  if (out.is_open()==false)
  {
    std::cout << "Output file could not be opened.'" << inputFile << "'" << std::endl;
    return -1;
  }

  out.write(str.c_str(), str.size() );
  out.flush();
  std::cout << "Generated: " << outputFile << std::endl;
  return err;
}




/**
 * @brief This is the main entry point for this program. Run the program and "ImportGenerator --outputdir [arg] --projectname [arg]"
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv)
{

  std::string outputDir;
  std::string projectName;
  try
  {
    ProgramOptions::options_description desc("Allowed options");
    desc.add_options()("help", "produce help message")
    (ImportGenerator::ProjectName.c_str(), ProgramOptions::value<std::string>(), "Name of the Project. Something like RawImporter or BinaryImporter")
    (ImportGenerator::OutputDir.c_str(), ProgramOptions::value<std::string>(), "Output Directory for Project files. Something like /tmp or C:/Workspace/");

    ProgramOptions::variables_map vm;
    ProgramOptions::store(ProgramOptions::command_line_parser(argc, argv).options(desc).run(), vm);
    ProgramOptions::notify(vm);

    if (vm.size() == 0 || vm.count("help"))
    {
      std::cout << "Usage: options_description [options]\n";
      std::cout << desc;
      return 0;
    }

    // Get the output directory
    if (vm.count(ImportGenerator::OutputDir))
    {
      outputDir =  vm[ImportGenerator::OutputDir].as<std::string> ();
      //std::cout << "Adding Option: " << ImportGenerator::OutputDir << " --> " << vm[ImportGenerator::OutputDir].as<std::string> () << std::endl;
    }
    else
    {
      std::cout << "Required Option " << ImportGenerator::OutputDir << " was not found. Exiting" << std::endl;
      exit(-1);
    }

    // Get the Project Name
    if (vm.count(ImportGenerator::ProjectName))
    {
      projectName =  vm[ImportGenerator::ProjectName].as<std::string> ();
      //std::cout << "Adding Option: " << ImportGenerator::ProjectName << " --> " << vm[ImportGenerator::ProjectName].as<std::string> () << std::endl;
    }
    else
    {
      std::cout << "Required Option " << ImportGenerator::ProjectName << " was not found. Exiting" << std::endl;
      exit(-1);
    }


  }
  catch (std::exception& e)
  {
    std::cout << e.what() << "\n";
    return 1;
  }


  // Create the output directory
  std::string outputPath = outputDir + MXAFileSystemPath::Separator + projectName;
  outputPath = MXAFileSystemPath::toNativeSeparators(outputPath);
  std::cout << "Creating directory: " << outputPath << std::endl;

  bool success = MXAFileSystemPath::mkdir(outputDir, true);
  std::cout << "Creating directory: " << outputPath << "/src" << std::endl;
  success = MXAFileSystemPath::mkdir(outputPath + MXAFileSystemPath::Separator + "src", true);


  int err = 0;
  {
    // Create the new CMakeLists.txt file
    std::string inputFile (ImportGenerator::CMakeListsInput);
    std::string outputFile (outputPath + MXAFileSystemPath::Separator + ImportGenerator::CMakeListsOutput);
    err = parseTemplate(MXAFileSystemPath::toNativeSeparators(inputFile),
                        MXAFileSystemPath::toNativeSeparators(outputFile), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }

  {
    // Create the new FindMXADataModel.cmake file
    std::string inputFile (ImportGenerator::FindMXADataModelInput);
    std::string outputFile (outputPath + MXAFileSystemPath::Separator + ImportGenerator::FindMXADataModelOutput);
    err = parseTemplate(MXAFileSystemPath::toNativeSeparators(inputFile),
                        MXAFileSystemPath::toNativeSeparators(outputFile), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }

  {
    // Create the new ImportDelegate.h Header file
    std::string inputFile (ImportGenerator::ImportDelegateInputHeader);
    std::string outputFile = MXAFileSystemPath::toNativeSeparators(outputPath) + MXAFileSystemPath::Separator + "src" + MXAFileSystemPath::Separator + projectName +  ImportGenerator::ImportDelegateOutputHeader;
    err = parseTemplate(MXAFileSystemPath::toNativeSeparators(inputFile),
                        MXAFileSystemPath::toNativeSeparators(outputFile), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }

  {
    // Create the new  ImportDelegate.cpp Source file
    std::string inputFile (ImportGenerator::ImportDelegateInputSource);
    std::string outputFile = MXAFileSystemPath::toNativeSeparators(outputPath) + MXAFileSystemPath::Separator + "src" + MXAFileSystemPath::Separator + projectName + ImportGenerator::ImportDelegateOutputSource;
    err = parseTemplate(MXAFileSystemPath::toNativeSeparators(inputFile),
                        MXAFileSystemPath::toNativeSeparators(outputFile), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }



  {
    // Create the new ImportDelegateFactory.h Header file
    std::string inputFile (ImportGenerator::ImportDelegateFactoryInputHeader);
    std::string outputFile = MXAFileSystemPath::toNativeSeparators(outputPath) + MXAFileSystemPath::Separator + "src" + MXAFileSystemPath::Separator + projectName + ImportGenerator::ImportDelegateFactoryOutputHeader;
    err = parseTemplate(MXAFileSystemPath::toNativeSeparators(inputFile),
                        MXAFileSystemPath::toNativeSeparators(outputFile), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }

  {
    // Create the new  ImportDelegateFactory.cpp Source file
    std::string inputFile (ImportGenerator::ImportDelegateFactoryInputSource);
    std::string outputFile = MXAFileSystemPath::toNativeSeparators(outputPath) + MXAFileSystemPath::Separator + "src" + MXAFileSystemPath::Separator + projectName  + ImportGenerator::ImportDelegateFactoryOutputSource;
    err = parseTemplate(MXAFileSystemPath::toNativeSeparators(inputFile),
                        MXAFileSystemPath::toNativeSeparators(outputFile), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }

  {
    // Create the new  main.cpp Source file
    std::string inputFile (ImportGenerator::MainInputSource);
    std::string outputFile = MXAFileSystemPath::toNativeSeparators(outputPath) + MXAFileSystemPath::Separator + "src" + MXAFileSystemPath::Separator + ImportGenerator::MainOutput;
    err = parseTemplate(MXAFileSystemPath::toNativeSeparators(inputFile),
                        MXAFileSystemPath::toNativeSeparators(outputFile), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }

  std::cout << "ImportGenerator is now complete. You can find your new project at " << MXAFileSystemPath::toNativeSeparators(outputPath) << std::endl;
  std::cout << "The project is automatically setup to use CMake." << std::endl;
  std::cout << "Setting the environment variable 'MXADataModel_INSTALL' will help this new project configure more quickly." << std::endl;

  return EXIT_SUCCESS;
}
