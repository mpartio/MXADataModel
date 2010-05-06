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
#ifndef H5BMPIMPORTDELEGATEFACTORY_H_
#define H5BMPIMPORTDELEGATEFACTORY_H_


//-- MXA Includes
#include <MXA/DataImport/AbstractImportDelegateFactory.h>

//-- C++ includes
#include <string>


DEFINE_IMPORT_DELEGATE_NAMESPACE(H5BmpImport);

// -----------------------------------------------------------------------------
//  Factory Class to generate H5BmpImportDelegate Objects
// -----------------------------------------------------------------------------
/**
* @class H5BmpImportDelegateFactory H5BmpImportDelegateFactory.h HDF5/H5BmpImportDelegateFactory.h
* @brief Factory Class to generate H5BmpImportDelegate Objects
* @author Mike Jackson
* @date Oct 2007
* @version $Revision: 1.2 $
*/
class MXA_EXPORT H5BmpImportDelegateFactory : public AbstractImportDelegateFactory
{
  public:
    H5BmpImportDelegateFactory();

    virtual ~H5BmpImportDelegateFactory();


  /**
   * @brief This method will return a new instance of H5BmpImportDelegate provided
   * the className matches.
   * @param className The name of the Delegate class that will be returned
   * @return A new boost shared pointer to H5BmpImportDelegate
   */
  IImportDelegate::Pointer newDataImportDelegate (const std::string &className );

  /**
   * @brief Returns the Classname of the delegate that this factory can create.
   */
  std::string delegateClassName();

  int32_t setProperty(const std::string &key, const std::string &value);
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

    H5BmpImportDelegateFactory(const H5BmpImportDelegateFactory&);    //Not Implemented
    void operator=(const H5BmpImportDelegateFactory&);  //Not Implemented
};


#endif /*H5BMPIMPORTDELEGATEFACTORY_H_*/
