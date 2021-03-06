///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, 2010 Michael A. Jackson for BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef H5BMPIMPORTDELEGATE_H_
#define H5BMPIMPORTDELEGATE_H_


//-- MXA Includes
#include "MXA/MXADLLExport.h"
#include "MXA/MXA.h"
//#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Base/IImportDelegate.h>
#include <MXA/Base/IDataSource.h>
#include <MXA/DataImport/AbstractImportDelegateFactory.h>

// C++ STL headers
#include <string>
#include <iostream>


//-- Forward Declare the MXADataModel Class
class MXADataModel;

namespace H5BmpImport
{
MXA_PROPERTY_CONSTANT(FileNotFoundIsError);
MXA_PROPERTY_CONSTANT(ImportAsGrayScale);
}

/**
 * @brief A generic Import Delegate that imports TIFF images into an HDF5 data file.
 * @author Mike Jackson
 * @date April 2007
 * @class H5BmpImportDelegate HDF5/H5BmpImportDelegate.h HDF5/H5BmpImportDelegate.h
 * @version $Revision: 1.2 $
 */
class MXA_EXPORT H5BmpImportDelegate : public IImportDelegate
{
public:
  H5BmpImportDelegate();
  virtual ~H5BmpImportDelegate();

  /**
  * @brief Implementation from the IDataImportDelegate class
  */
  int32_t importDataSource(IDataSource::Pointer dataSource, IDataFile::Pointer dataFile);


  /*
  * @brief Sets the 'FileNotFoundIsError' flag.
  * @param bool If this is true then the import will abort if an input file can not be found
  * @param FileNotFoundIsError If the input file is NOT found, consider this on error
  * @param m_FileNotFoundIsError Internal Storage variable
  */
  MXA_INSTANCE_PROPERTY(bool, FileNotFoundIsError);

  /*
  * @brief Sets the 'ImportAsGrayScale' value
  * @param bool If TRUE, then the tiff file will be converted to Grayscale
  * @param ImportAsGrayScale Import the image as a grayscale image
  * @param m_ImportAsGrayScale Internal Storage variable
  */
  MXA_INSTANCE_PROPERTY(bool, ImportAsGrayScale);

  /**
  * @brief Sets a property of this class with the given value.
  * @param key The name of the property.
  * @param value  The value to set the property to.
  */
  virtual int32_t setProperty(const std::string &key, const std::string &value);

  virtual int32_t getProperty(const std::string &key, std::string &value)
  {
    if (key.compare(H5BmpImport::FileNotFoundIsError) == 0)
    {
      if (this->m_FileNotFoundIsError == true) {  value = "1"; }
      else {  value = "0"; }
      return 0;
    }
    if (key.compare(H5BmpImport::ImportAsGrayScale) == 0)
    {
      if (this->m_ImportAsGrayScale == true) {  value = "1"; }
      else {  value = "0"; }
      return 0;
    }
    return -1;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  template<typename T>
  int32_t getProperty(const std::string &key, T &value)
  {
    GET_PROPERTY_BODY(H5BmpImport, T, FileNotFoundIsError, key, value);
    GET_PROPERTY_BODY(H5BmpImport, T, ImportAsGrayScale, key, value);
    return 0;
  }

private:
  IDataModel::Pointer      m_modelPtr;

  H5BmpImportDelegate(const H5BmpImportDelegate&);   //Copy Constructor Not Implemented
  void operator=(const H5BmpImportDelegate&); //Copy Assignment Not Implemented
};



#endif /*H5BMPIMPORTDELEGATE_H_*/
