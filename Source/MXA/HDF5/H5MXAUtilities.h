///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef H5MXAUTILITIES_H_
#define H5MXAUTILITIES_H_

#include <MXA/Base/IDataModel.h>
#include <MXA/Base/IDataFile.h>

/**
* @class H5MXAUtilities H5MXAUtilities.h HDF5/H5MXAUtilities.h
* @brief Utility methods that combine hdf5 functions with MXA objects
* @author Mike Jackson
* @date July 2008
* @version $Revision: 1.2 $
*/
class H5MXAUtilities
{
  public:

    virtual ~H5MXAUtilities();



    /**
     * @brief Creates an absolute path suitable for create an HDF5 data set.
     * @param model The data model to use
     * @param indices The indices to use for the Data dimensions
     * @param record The MXADataRecord to generate the path to
     * @return The hdf5 path
     */
    static MXA_EXPORT std::string generateH5PathToDataset (  IDataModel::Pointer model,
                                                  std::vector<int32_t> &indices,
                                                  IDataRecord::Pointer record);

   /**
   * @brief Reads a dataset from an HDF5 data file into a newly allocated MXAAbstractData
   * derived object.
   * @param dataFile The IDataFile object to read from
   * @param datasetPath The internal HDF5 path to the data, relative to the root of the data file
   * @return
   */
    static MXA_EXPORT IMXAArray::Pointer readData(IDataFile::Pointer dataFile, const std::string &datasetPath);

    /**
    * @brief Reads an attribute from an HDF5 data file into a newly allocated MXAAbstractAttribute
    * derived object.
    * @param dataFile The IDataFile object to read from
    * @param datasetPath The internal HDF5 path to the data, relative to the root of the data file
    * @param attributeKey The name of the attribute to read
    * @return
    */
    static MXA_EXPORT IMXAArray::Pointer readAttribute(IDataFile::Pointer dataFile, const std::string &datasetPath, const std::string &attributeKey);





  protected:
    H5MXAUtilities() {};

  private:
    H5MXAUtilities(const H5MXAUtilities&);    // Copy Constructor Not Implemented
      void operator=(const H5MXAUtilities&);  // Operator '=' Not Implemented
};

#endif /* H5MXAUTILITIES_H_ */
