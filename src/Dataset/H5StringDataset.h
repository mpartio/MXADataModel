///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _H5STRINGDATASET_CPP_
#define _H5STRINGDATASET_CPP_

#include <Common/DLLExport.h>
#include <Common/MXATypes.h>
#include <Base/IFileWriter.h>
#include <Base/IFileReader.h>
//
//#include <string>

/**
 * @brief This class represents an HDF5 data set that is stored as a string
 * @author Mike Jackson
 * @date Dec 28, 2007
 * @version $Revision: 1.2 $
 * @header Dataset/H5StringDataset.h
 */
class MXA_EXPORT H5StringDataset : public IFileWriter, public IFileReader
{

public:
  H5StringDataset(const std::string &datasetPath, const std::string &value)
  {}
  virtual ~H5StringDataset()
  {}

    
// -----------------------------------------------------------------------------
//  IFileWriter Implementation is pure virtual. Subclasses need to implement.
// -----------------------------------------------------------------------------
  int32 writeToFile(IDataFilePtr dataFile);

// -----------------------------------------------------------------------------
//  IFileReader Implementation is pure virtual. Subclasses need to implement
// -----------------------------------------------------------------------------
  int32 readFromFile(IDataFilePtr dataFile);
    
    
private:
  std::string _value;
  
  
  H5StringDataset(const H5StringDataset&);   //Copy Constructor Not Implemented
  void operator=(const H5StringDataset&); //Copy Assignment Not Implemented
  
};



#endif /* _H5STRINGDATASET_CPP_ */

