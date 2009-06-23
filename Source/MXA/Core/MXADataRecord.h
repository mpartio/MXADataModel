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

#include <MXA/Common/MXATypes.h>
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Base/IDataRecord.h>
#include <MXA/Base/IDataRecordWriter.h>
#include <MXA/Core/MXAConstants.h>
#include <MXA/Utilities/StringUtils.h>
// #include <MXA/Core/MXAAttribute.h>

//Include Boost Headers
// #include <boost/shared_ptr.hpp>
// #include <boost/weak_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <vector>
/**
 * @brief This class holds the required and any extended information about a
 * data record. Data records can have any number of children but only one parent.
 * @author  Mike Jackson
 * @date March 2007
 * @version 1.2
 *  
 */
class MXA_EXPORT MXADataRecord :  public IDataRecord
{

  static int32 _uniqueGUIDValue;
  
public:

  MXA_SHARED_POINTERS(MXADataRecord);
  MXA_TYPE_MACRO(MXADataRecord);
  typedef std::vector<MXADataRecord::Pointer>       MXADataRecords;
  
  /**
  * @brief Creates a flat look up table using the GUID of the data record as the
   * key and the boost::Shared_ptr as the value
   * @param lut The look up table to be populated.
   * @param nodes The IDataRecord::Container to use as the tree
   */
   static void generateLUT(IDataRecord::LookupTable &lut, IDataRecord::Container &nodes);
  
  
  /**
  * @brief Used to create a new MXANode
  * @param luid The local unique Id for this record
  * @param name The Name of this node
  * @param altName The Alternate name for the Data Record.
  * @return A Boost SharedPointer to the MXANode Object
  */
  static Pointer New(int luid, std::string name, std::string altName);

  /**
  * @brief Creates a copy of the IDataRecord object which will NOT share 
  * any data with the original object.
  * @param rec IDataRecord to copy
  */
  static Pointer New(IDataRecord::Pointer rec)
  {
    return MXADataRecord::New(rec->getLuid(), rec->getRecordName(), rec->getAltName() );
  }
  
/**
* @brief Destructor
*/
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
   * @brief Generates the path to this data record in terms of an internal HDF path
   */
  std::string generatePath();
  
  /**
   * @brief Generates the internal HDF5 path only to this records parent
   */
  std::string generateParentPath();
  
  /**
   * @brief Writes this data record using the provided IDataRecordWriter implementation
   * @param writer The IDataRecordWriter object to use to write this data record
   */
  int32 writeRecord(IDataRecordWriter* writer);

  /**
   * @brief Checks some basic properties of the model to make sure they are
   * set correctly.
   * @param message String to hold messages concerning errors/omissions from
   * the model
   * @return True if the model passes the basic checks
   */
  bool isValid(std::string &message);

  /**
    * @brief Setter for property guid
   * @param aValue The new value to set for property guid
   */
  void setUniqueId(int32 aValue) { _uniqueId = aValue; }
  
  /**
    * @brief Getter for property guid
   * @return The value of guid
   */
  int32 getUniqueId() { return _uniqueId; }
  
  /**
    * @brief
   */
  void resetGUIDValue();
  
  /**
    * @brief
   */
  static int32 nextGUIDValue();
  
  /// Accessor for Parent iVar
  void setParent(IDataRecord::WeakPointer parent);
  IDataRecord::WeakPointer getParent();
  
  // Children Methods
  int32 getNumChildren() const;
  bool hasChildren() const;
  void addChild(IDataRecord::Pointer child);
  void removeChild(int index);
  void removeChild(IDataRecord* child);
  void removeAllChildren();
  int32 indexOfChild(IDataRecord* child);

  IDataRecord::Pointer getChildAt(int32 index);
  IDataRecord::Container& getChildren();
  
  // Utilities
  void printDataRecordTree(int32 depth=0);
  virtual void printDataRecord(std::ostream& os, int32 indentSize=0);
  
  
  // -------- Needed for Parent Child relationship ------------
  void _setWeakPtr(IDataRecord::WeakPointer weakPtr);
  
protected:
  MXADataRecord();

  int32 _luid;
  std::string _recordName;
  std::string _altName;

  // --------------- IDataRecord Related Variables
  // Node Name
  std::string _nodeName;
  int32 _uniqueId;
  IDataRecord::WeakPointer _selfPtr;
  IDataRecord::WeakPointer _parent;
  IDataRecord::Container _children;
 // MXAAttributeMap _nodeAttributes;
  
private:
    MXADataRecord(const MXADataRecord&);   //Copy Constructor Not Implemented
    void operator=(const MXADataRecord&); //Copy Assignment Not Implemented
};



#endif /* _MXADATARECORDNODE_H_ */

