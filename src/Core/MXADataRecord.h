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
#ifndef _MXADATARECORDNODE_H_
#define _MXADATARECORDNODE_H_

//TODO: Add ability to append any type of attribute to this Data Record

#include "Common/MXATypes.h"
#include "Common/MXATypeDefs.h"
#include "Base/IDataRecord.h"
#include "Base/IDataRecordWriter.h"
#include "Core/MXANode.h"
#include "Core/MXAConstants.h"
#include "Utilities/StringUtils.h"

//Include Boost Headers
#include "boost/shared_ptr.hpp"

/**
 * @brief This class holds the required and any extended information about a
 * data record. Data records can have any number of children but only one parent.
 * @author  Mike Jackson
 * @date March 2007
 * @version 1.0
 *  
 */
class MXA_EXPORT MXADataRecord :  public IDataRecord, public MXANode
{

public:  
 /**
  * @brief Used to create a new MXANode
  * @param type The Type of Node this is going to be
  * @param nodeName The Name of this node
  * @return A Boost SharedPointer to the MXANode Object
  */
  static MXADataRecordPtr New(int luid, std::string name, std::string altName);

  virtual ~MXADataRecord();
  
  /**
  * @brief Setter for property name
  * @param aValue The new value to set for property name
  */
  void setRecordName(std::string aValue) { _recordName = aValue; }

  /**
  * @brief Getter for property name
  * @return The value of name
  */
  std::string getRecordName() { return _recordName; }

  /**
  * @brief Setter for property altName
  * @param aValue The new value to set for property altName
  */
  void setAltName(std::string aValue) { _altName = aValue; }

  /**
  * @brief Getter for property altName
  * @return The value of altName
  */
  std::string getAltName() { return _altName; }
  
  /**
  * @brief Getter for property luid
  * @return The value of luid
  */
  int32 getLuid() { return _luid; }
  
  /**
  * @brief Setter for property guid
  * @param aValue The new value to set for property guid
  */
  void setGuid(int32 aValue) { setUniqueId(aValue); }

  /**
  * @brief Getter for property guid
  * @return The value of guid
  */
  int32 getGuid() { return getUniqueId(); }
  
  /**
  * @brief Setter for property luid
  * @param aValue The new value to set for property luid
  */
  void setLuid(int32 aValue) { 
    _luid = aValue;
    this->_nodeName = StringUtils::numToString(this->_luid);
  }


  /**
   * @brief Prints the Data Record to the provided stream
   */
  void printNode(std::ostream& os, int32 indentSize=0);
  
  /**
   * @brief Generates the path to this data record in terms of an internal HDF path
   */
  std::string generatePath();
  
  /**
   * @brief Generates the internal HDF5 path only to this dimensions parent
   */
  std::string generateParentPath();
  
  /**
   * @brief Writes this data record using the provided IDataRecordWriter implementation
   * @param writer The IDataRecordWriter object to use to write this data record
   */
  int32 writeRecord(IDataRecordWriter* writer);
  
  //--- Over Ride from the MXANode Base class
  void setNodeName(std::string nodeName);

  
protected:
  MXADataRecord();

  int32 _luid;
  std::string _recordName;
  std::string _altName;

private:
    MXADataRecord(const MXADataRecord&);   //Copy Constructor Not Implemented
    void operator=(const MXADataRecord&); //Copy Assignment Not Implemented
};



#endif /* _MXADATARECORDNODE_H_ */

