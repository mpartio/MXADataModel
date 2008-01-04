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
* @class H5MXADataset H5MXADataset.h HDF5/H5MXADataset.h
 * @author Mike Jackson
 * @date Jan 3, 2008
 * @version $Revision: 1.1 $
 */
class MXA_EXPORT H5MXADataset : public IDataset
{

public:
  
  static IDatasetPtr LoadFromFile(IDataFilePtr dataFile, const std::string &datasetPath);
  static IDatasetPtr CreateDatasetPtr(MXAAbstractDataPtr data);
  
  explicit H5MXADataset(MXAAbstractDataPtr data);
  virtual ~H5MXADataset();

  // -----------------------------------------------------------------------------
  //  IDataset Methods
  // -----------------------------------------------------------------------------
  std::string getDatasetPath();
  void setData(MXAAbstractDataPtr data);
  MXAAbstractDataPtr getData();
  
  void addAttribute (MXAAbstractAttributePtr attribute);
  void removeAttribute (const std::string &attributeKey);
  MXAAbstractAttributePtr getAttribute(const std::string &attributeKey);
  
  // -----------------------------------------------------------------------------
  // IDataFileIO Methods 
  // -----------------------------------------------------------------------------
  virtual int32 writeToFile (IDataFilePtr dataFile);
  virtual int32 readFromFile (IDataFilePtr dataFile);
  
protected:
  H5MXADataset() {}  // Protected constructor
  
private:
  MXAAbstractDataPtr _data;
  MXAAbstractAttributes _attributes;
  
  H5MXADataset(const H5MXADataset&);   //Copy Constructor Not Implemented
  void operator=(const H5MXADataset&); //Copy Assignment Not Implemented
};



#endif /* _H5MXADATASET_H_ */

