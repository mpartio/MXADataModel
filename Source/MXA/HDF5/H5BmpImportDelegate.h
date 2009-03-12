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
#ifndef H5BMPIMPORTDELEGATE_H_
#define H5BMPIMPORTDELEGATE_H_


//-- MXA Includes
#include <MXA/Common/DLLExport.h>
#include <MXA/Common/MXATypes.h>
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Base/IImportDelegate.h>
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
  int32 importDataSource(IDataSourcePtr dataSource, IDataFilePtr dataFile);


  /*
  * @brief Sets the 'FileNotFoundIsError' flag.
  * @param bool If this is true then the import will abort if an input file can not be found
  * @param FileNotFoundIsError If the input file is NOT found, consider this on error
  * @param _fileNotFoundIsError Internal Storage variable
  */
  MXA_PROPERTY(bool, FileNotFoundIsError, _fileNotFoundIsError);

  /*
  * @brief Sets the 'ImportAsGrayScale' value
  * @param bool If TRUE, then the tiff file will be converted to Grayscale
  * @param ImportAsGrayScale Import the image as a grayscale image
  * @param _importAsGrayScale Internal Storage variable
  */
  MXA_PROPERTY(bool, ImportAsGrayScale, _importAsGrayScale);

  /**
  * @brief Sets a property of this class with the given value.
  * @param key The name of the property.
  * @param value  The value to set the property to.
  */
  virtual int32 setProperty(const std::string &key, const std::string &value);

  virtual int32 getProperty(const std::string &key, std::string &value)
  {
    if (key.compare(H5BmpImport::FileNotFoundIsError) == 0)
    {
      if (this->_fileNotFoundIsError == true) {  value = "1"; }
      else {  value = "0"; }
      return 0;
    }
    if (key.compare(H5BmpImport::ImportAsGrayScale) == 0)
    {
      if (this->_importAsGrayScale == true) {  value = "1"; }
      else {  value = "0"; }
      return 0;
    }
    return -1;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  template<typename T>
  int32 getProperty(const std::string &key, T &value)
  {
    GET_PROPERTY_BODY(H5BmpImport, T, FileNotFoundIsError, _fileNotFoundIsError, key, value);
    GET_PROPERTY_BODY(H5BmpImport, T, ImportAsGrayScale, _importAsGrayScale, key, value);
    return 0;
  }

private:
  IDataModelPtr      _modelPtr;
  bool _fileNotFoundIsError;
  bool _importAsGrayScale;

  H5BmpImportDelegate(const H5BmpImportDelegate&);   //Copy Constructor Not Implemented
  void operator=(const H5BmpImportDelegate&); //Copy Assignment Not Implemented
};



#endif /*H5BMPIMPORTDELEGATE_H_*/
