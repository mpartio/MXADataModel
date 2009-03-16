///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////


// #define USE_BUILD_TREE 0
#include <UtilityFileLocations.h>

// STL includes
#include <string>
#include <iostream>
#include <sstream>
#include <MXA/Utilities/MXAFileSystemPath.h>

#include <boost/algorithm/string.hpp>

#include <boost/iostreams/device/file.hpp>


// Boost program options
#include <boost/program_options.hpp>
namespace ProgramOptions = boost::program_options;



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


#define GET_REQUIRED_OPTION(option)\
  if (vm.count(option ) ) {\
    options.insert(option, vm[option.toStdString() ].as<std::string>()  ); \
    std::cout << "Adding Option: " << option << " --> " <<  vm[option].as<std::string>() << std::endl;\
    } \
else {\
  std::cout << "Required Option " << option << " was not found. Exiting" << std::endl;\
  exit(-1);\
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int parseTemplate(const std::string &inputFile,
                  const std::string &outputFile,
                  const std::string &projectName)
{
  //std::cout << "inputFile: " << inputFile << std::endl;
  std::cout << "Generating: " << outputFile << std::endl;
  std::cout << "-----------------------------------------------" << std::endl;
  //std::cout << "projectName: " << projectName << std::endl;


  int err = 0;
  boost::iostreams::file_source source(inputFile, std::ios_base::in);
  if (source.is_open() == false)
  {
    std::cout << "the source file could not be opened for reading.'" << inputFile << "'" << std::endl;
    return -1;
  }

  std::string _configFile = inputFile;

  std::ifstream configFile;
  configFile.open(_configFile);
  if (configFile.is_open() == false)
  {
    std::cout << "Could not open file '" << inputFile << "'" << std::endl;
    return -1;
  }
  // Read the whole file into a string
  std::stringstream ss;
  ss << configFile.rdbuf();
  std::string str(ss.str());


  boost::algorithm::replace_all(str, "@PROJECT_NAME@", projectName);

  boost::filesystem::ofstream out;
  out.open(outputFile, BOOST_IOS::out | BOOST_IOS::trunc);
  if (out.is_open()==false)
  {
    std::cout << "Output file could not be opened.'" << inputFile << "'" << std::endl;
    return -1;
  }

  out.write(str.c_str(), str.size() );

  return err;
}




/**
 *
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
   // int opt;
    //int ProgramOptionsrtnum;
    ProgramOptions::options_description desc("Allowed options");
    desc.add_options()("help", "produce help message")
    (ImportGenerator::ProjectName.c_str(), ProgramOptions::value<std::string>(), "Name of the Project.")
    (ImportGenerator::OutputDir.c_str(), ProgramOptions::value<std::string>(), "Output Directory for Project files.");

    ProgramOptions::variables_map vm;
    ProgramOptions::store(ProgramOptions::command_line_parser(argc, argv).options(desc).run(), vm);
    ProgramOptions::notify(vm);

    if (vm.count("help"))
    {
      std::cout << "Usage: options_description [options]\n";
      std::cout << desc;
      return 0;
    }

    // Get the output directory
    if (vm.count(ImportGenerator::OutputDir))
    {
      outputDir =  vm[ImportGenerator::OutputDir].as<std::string> ();
      std::cout << "Adding Option: " << ImportGenerator::OutputDir << " --> " << vm[ImportGenerator::OutputDir].as<std::string> () << std::endl;
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
      std::cout << "Adding Option: " << ImportGenerator::ProjectName << " --> " << vm[ImportGenerator::ProjectName].as<std::string> () << std::endl;
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
  FileSystem::path outputPath(outputDir);
  outputPath = outputPath / projectName;
  boost::filesystem::create_directories(outputPath);
  boost::filesystem::create_directories(outputPath/"src");
  int err = 0;
  {
    // Create the new CMakeLists.txt file
    FileSystem::path inputFile (ImportGenerator::CMakeListsInput);
    FileSystem::path outputFile (outputPath / ImportGenerator::CMakeListsOutput);
    err = parseTemplate(inputFile.native_file_string(), outputFile.native_file_string(), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }

  {
    // Create the new FindMXADataModel.cmake file
    FileSystem::path inputFile (ImportGenerator::FindMXADataModelInput);
    FileSystem::path outputFile (outputPath / ImportGenerator::FindMXADataModelOutput);
    err = parseTemplate(inputFile.native_file_string(), outputFile.native_file_string(), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }

  {
    // Create the new Delegate Header file
    FileSystem::path inputFile (ImportGenerator::ImportDelegateInputHeader);
    FileSystem::path outputFile = outputPath.native_file_string() + "/src/" + projectName +  ImportGenerator::ImportDelegateOutputHeader;
    err = parseTemplate(inputFile.native_file_string(), outputFile.native_file_string(), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }

  {
    // Create the new  Delegate Source file
    FileSystem::path inputFile (ImportGenerator::ImportDelegateInputSource);
    FileSystem::path outputFile = outputPath.native_file_string() + "/src/" + projectName +  ImportGenerator::ImportDelegateOutputSource;
    err = parseTemplate(inputFile.native_file_string(), outputFile.native_file_string(), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }



  {
    // Create the new DelegateFactory Header file
    FileSystem::path inputFile (ImportGenerator::ImportDelegateFactoryInputHeader);
    FileSystem::path outputFile = outputPath.native_file_string() + "/src/" + projectName +  ImportGenerator::ImportDelegateFactoryOutputHeader;
    err = parseTemplate(inputFile.native_file_string(), outputFile.native_file_string(), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }

  {
    // Create the new  DelegateFactory Source file
    FileSystem::path inputFile (ImportGenerator::ImportDelegateFactoryInputSource);
    FileSystem::path outputFile = outputPath.native_file_string() + "/src/" + projectName +  ImportGenerator::ImportDelegateFactoryOutputSource;
    err = parseTemplate(inputFile.native_file_string(), outputFile.native_file_string(), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }

  {
    // Create the new  DelegateFactory Source file
    FileSystem::path inputFile (ImportGenerator::MainInputSource);
    FileSystem::path outputFile = outputPath.native_file_string() + "/src/" + ImportGenerator::MainOutput;
    err = parseTemplate(inputFile.native_file_string(), outputFile.native_file_string(), projectName);
    if (err < 0)
    {
      exit(EXIT_FAILURE);
    }
  }


  return EXIT_SUCCESS;
}
