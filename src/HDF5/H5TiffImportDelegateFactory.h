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
#ifndef H5TIFFIMPORTDELEGATEFACTORY_H_
#define H5TIFFIMPORTDELEGATEFACTORY_H_

//-- MXA Includes
#include <DataImport/AbstractImportDelegateFactory.h>

//-- C++ includes
#include <string>


/**
 * @brief Constants for dealing with Tif files
 */
namespace H5TiffImportDelegateFactory_Detail
{
  const std::string ClassName("H5TiffImport");
}

// -----------------------------------------------------------------------------
//  Factory Class to generate H5TiffImportDelegate Objects
// -----------------------------------------------------------------------------
/**
* @class H5TiffImportDelegateFactory H5TiffImportDelegateFactory.h HDF5/H5TiffImportDelegateFactory.h
* @brief Factory Class to generate H5TiffImportDelegate Objects
* @author Mike Jackson
* @date Sept 2007
* @version $Revision: 1.3 $
*/
class MXA_EXPORT H5TiffImportDelegateFactory : public AbstractImportDelegateFactory
{
  public:
    H5TiffImportDelegateFactory();

    virtual ~H5TiffImportDelegateFactory();


  /**
   * @brief This method will return a new instance of H5TiffImportDelegate provided
   * the className matches.
   * @param className The name of the Delegate class that will be returned
   * @return A new boost shared pointer to H5TiffImportDelegate
   */
  IImportDelegatePtr newDataImportDelegate (const std::string &className );

  /**
   * @brief Returns the Classname of the delegate that this factory can create.
   */
  std::string delegateClassName();

  /**
  * @brief Sets the 'FileNotFoundIsError' flag.
  * @param value If this is true then the import will abort if an input file can not be found
  */
  void setFileNotFoundIsError(bool value);

  /**
  * @brief Sets the 'ImportAsGrayScale' value
  * @param value If TRUE, then the tiff file will be converted to Grayscale
  */
  void setImportAsGrayScale(bool value);

  private:

    bool _fileNotFoundIsError;
    bool _importAsGrayScale;

    H5TiffImportDelegateFactory(const H5TiffImportDelegateFactory&);    //Not Implemented
    void operator=(const H5TiffImportDelegateFactory&);  //Not Implemented
};



#endif /*H5TIFFIMPORTDELEGATEFACTORY_H_*/
