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
#ifndef OFIMPORTDELEGATE_H_
#define OFIMPORTDELEGATE_H_


//-- MXA Includes
#include <MXA/MXATypes.h>
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Base/IDataSource.h>
#include <MXA/Base/IDataFile.h>
#include <MXA/Base/IDataModel.h>
#include <MXA/Base/IImportDelegate.h>
//#include <MXA/DataImport/AbstractImportDelegateFactory.h>

// C++ STL headers
#include <string>
#include <iostream>


//-- Forward Declare the MXADataModel Class
class MXADataModel;

namespace H5TiffImport
{
    MXA_PROPERTY_CONSTANT(FileNotFoundIsError);
    MXA_PROPERTY_CONSTANT(ImportAsGrayScale);
}

/**
 * @brief A generic Import Delegate that imports TIFF images into an HDF5 data file.
 * @author Mike Jackson
 * @date April 2007
 * @class H5TiffImportDelegate HDF5/H5TiffImportDelegate.h HDF5/H5TiffImportDelegate.h
 * @version $Revision: 1.2 $
 */
class MXA_EXPORT H5TiffImportDelegate : public IImportDelegate
{
public:
	H5TiffImportDelegate();
	virtual ~H5TiffImportDelegate();

  /**
  * @brief Implementation from the IDataImportDelegate class
  */
  int32_t importDataSource(IDataSource::Pointer dataSource, IDataFile::Pointer dataFile);

  /**
   * @brief Sets the pointer to the DataModel
   * @param model A Pointer to the DataModel
   */
  void setDataModel(IDataModel::Pointer model);

  /*
  * @brief Sets the 'FileNotFoundIsError' flag.
  * @param value If this is true then the import will abort if an input file can not be found
  */
  MXA_INSTANCE_PROPERTY(bool, FileNotFoundIsError);

  /*
  * @brief Sets the 'ImportAsGrayScale' value
  * @param value If TRUE, then the tiff file will be converted to Grayscale
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
    if (key.compare(H5TiffImport::FileNotFoundIsError) == 0)
    {
      if (this->m_FileNotFoundIsError == true) {  value = "1"; }
      else {  value = "0"; }
      return 0;
    }
    if (key.compare(H5TiffImport::ImportAsGrayScale) == 0)
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
    GET_PROPERTY_BODY(H5TiffImport, T, FileNotFoundIsError, key, value);
    GET_PROPERTY_BODY(H5TiffImport, T, ImportAsGrayScale, key, value);
    return 0;
  }

private:
  H5TiffImportDelegate(const H5TiffImportDelegate&);   //Copy Constructor Not Implemented
  void operator=(const H5TiffImportDelegate&); //Copy Assignment Not Implemented
};


#endif /*OFIMPORTDELEGATE_H_*/
