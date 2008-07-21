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
#ifndef _H5MXADATASET_H_
#define _H5MXADATASET_H_

#include <Common/DLLExport.h>
#include <Common/MXATypeDefs.h>
#include <Base/IDataset.h>
/**
 * @brief
* @class H5Dataset H5Dataset.h HDF5/H5Dataset.h
 * @author Mike Jackson
 * @date Jan 3, 2008
 * @version $Revision: 1.1 $
 */
class MXA_EXPORT H5Dataset : public IDataset
{

public:

  static IDatasetPtr LoadFromFile(IDataFilePtr dataFile, const std::string &datasetPath);
  static IDatasetPtr CreateDatasetPtr(const std::string &datasetPath, IMXAArrayPtr data);

  H5Dataset(const std::string &datasetPath, IMXAArrayPtr data);
  virtual ~H5Dataset();

  // -----------------------------------------------------------------------------
  //  IDataset Methods
  // -----------------------------------------------------------------------------
  void setDatasetPath(const std::string &path);
  std::string getDatasetPath();

  void setData(IMXAArrayPtr data);
  IMXAArrayPtr getData();

  void addAttribute (const std::string &attributeKey, IMXAArrayPtr attribute);
  void removeAttribute (const std::string &attributeKey);
  IMXAArrayPtr getAttribute(const std::string &attributeKey);

  // -----------------------------------------------------------------------------
  // IDataFileIO Methods
  // -----------------------------------------------------------------------------
  virtual int32 writeToFile (IDataFilePtr dataFile);
  virtual int32 readFromFile (IDataFilePtr dataFile);

protected:
  H5Dataset() {}  // Protected constructor

private:
  IMXAArrayPtr _data;
  MXAAbstractAttributes _attributes;
  std::string _datasetPath;


  H5Dataset(const H5Dataset&);   //Copy Constructor Not Implemented
  void operator=(const H5Dataset&); //Copy Assignment Not Implemented
};



#endif /* _H5MXADATASET_H_ */

