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
#include <MXA/Utilities/MXADir.h>
#include <MXA/MXAVersion.h>
#include <MXA/Common/LogTime.h>

// STL includes
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/algorithm/string.hpp>

//-- tclap headers
#include "tclap/CmdLine.h"
#include "tclap/ValueArg.h"

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
  std::ifstream source(inputFile.c_str(), std::ios_base::in | std::ios::binary);
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
    //CmdLine Parser generator
    TCLAP::CmdLine cmd("M3C Program", ' ', MXA::Version::Complete());
    TCLAP::ValueArg<std::string > projectNameArg("n", "name", "Name of the Project", true, "", "Name of the Project");
    cmd.add(projectNameArg);
    TCLAP::ValueArg<std::string > outputDirArg("o", "output", "Output Directory for Project files", true, "", "Output Directory for Project files");
    cmd.add(outputDirArg);

    // Parse the argv array.
    cmd.parse(argc, argv);

    outputDir = outputDirArg.getValue();
    projectName = projectNameArg.getValue();
  }
  catch (TCLAP::ArgException &e)
  {
    std::cerr << logTime() << " error: " << e.error() << " for arg " << e.argId() << std::endl;
    return EXIT_FAILURE;
  }


  // Create the output directory
  std::string outputPath = outputDir + MXADir::Separator + projectName;
  outputPath = MXADir::toNativeSeparators(outputPath);
  std::cout << "Creating directory: " << outputPath << std::endl;

  bool success = MXADir::mkdir(outputDir, true);
  std::cout << "Creating directory: " << outputPath << "/src" << std::endl;
  success = MXADir::mkdir(outputPath + MXADir::Separator + "src", true);


  int err = 0;
  {
    // Create the new CMakeLists.txt file
    std::string inputFile (ImportGenerator::CMakeListsInput);
    std::string outputFile (outputPath + MXADir::Separator + ImportGenerator::CMakeListsOutput);
    err = parseTemplate(MXADir::toNativeSeparators(inputFile),
                        MXADir::toNativeSeparators(outputFile), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }

  {
    // Create the new FindMXADataModel.cmake file
    std::string inputFile (ImportGenerator::FindMXADataModelInput);
    std::string outputFile (outputPath + MXADir::Separator + ImportGenerator::FindMXADataModelOutput);
    err = parseTemplate(MXADir::toNativeSeparators(inputFile),
                        MXADir::toNativeSeparators(outputFile), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }

  {
    // Create the new ImportDelegate.h Header file
    std::string inputFile (ImportGenerator::ImportDelegateInputHeader);
    std::string outputFile = MXADir::toNativeSeparators(outputPath) + MXADir::Separator + "src"
          + MXADir::Separator + projectName +  ImportGenerator::ImportDelegateOutputHeader;

    err = parseTemplate(MXADir::toNativeSeparators(inputFile),
                        MXADir::toNativeSeparators(outputFile), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }

  {
    // Create the new  ImportDelegate.cpp Source file
    std::string inputFile (ImportGenerator::ImportDelegateInputSource);
    std::string outputFile = MXADir::toNativeSeparators(outputPath) + MXADir::Separator + "src"
        + MXADir::Separator + projectName + ImportGenerator::ImportDelegateOutputSource;
    err = parseTemplate(MXADir::toNativeSeparators(inputFile),
                        MXADir::toNativeSeparators(outputFile), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }



  {
    // Create the new ImportDelegateFactory.h Header file
    std::string inputFile (ImportGenerator::ImportDelegateFactoryInputHeader);
    std::string outputFile = MXADir::toNativeSeparators(outputPath) + MXADir::Separator + "src"
        + MXADir::Separator + projectName + ImportGenerator::ImportDelegateFactoryOutputHeader;
    err = parseTemplate(MXADir::toNativeSeparators(inputFile),
                        MXADir::toNativeSeparators(outputFile), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }

  {
    // Create the new  ImportDelegateFactory.cpp Source file
    std::string inputFile (ImportGenerator::ImportDelegateFactoryInputSource);
    std::string outputFile = MXADir::toNativeSeparators(outputPath) + MXADir::Separator + "src"
        + MXADir::Separator + projectName  + ImportGenerator::ImportDelegateFactoryOutputSource;
    err = parseTemplate(MXADir::toNativeSeparators(inputFile),
                        MXADir::toNativeSeparators(outputFile), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }

  {
    // Create the new  main.cpp Source file
    std::string inputFile (ImportGenerator::MainInputSource);
    std::string outputFile = MXADir::toNativeSeparators(outputPath) + MXADir::Separator + "src"
        + MXADir::Separator + ImportGenerator::MainOutput;
    err = parseTemplate(MXADir::toNativeSeparators(inputFile),
                        MXADir::toNativeSeparators(outputFile), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }

  std::cout << "ImportGenerator is now complete. You can find your new project at " << MXADir::toNativeSeparators(outputPath) << std::endl;
  std::cout << "The project is automatically setup to use CMake." << std::endl;
  std::cout << "Setting the environment variable 'MXADataModel_INSTALL' will help this new project configure more quickly." << std::endl;

  return EXIT_SUCCESS;
}
