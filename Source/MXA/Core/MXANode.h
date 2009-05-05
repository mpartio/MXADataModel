///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2006, Mike Jackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#error Nothing should be including this file

#ifndef _MHDNODE_H_
#define _MHDNODE_H_

//MXA Includes
#include <MXA/Common/MXATypes.h>
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Base/INode.h>
#include <MXAAttribute.h>

//-- C++ Headers
#include <string>
#include <vector>
#include <list>
#include <map>

//Include Boost Headers
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/tuple/tuple.hpp>


/**
* @class MXANode MXANode.h src/Core/MXANode.h
* @brief This class is a concrete implementation of the INode abstract class. It is meant to 
* be subclassed by other classes that need to maintain parent/child relationships
* @author Mike Jackson
* @date March 2007
* @version $Revision: 1.2 $
*/
class MXA_EXPORT MXANode : public INode
{
 public:
   
   MXA_SHARED_POINTERS(MXANode);
   MXA_TYPE_MACRO(MXANode);
   
   static int32 _uniqueGUIDValue;
   
   /**
    * @brief Creates a flat look up table using the GUID of the data record as the
    * key and the boost::Shared_ptr as the value
    * @param lut The look up table to be populated.
    * @param nodes The nodes to use in the lookup table
    */
   static void generateLUT(std::map<int32, MXANodePtr> &lut, MXANodes &nodes);
   
   enum Type {
     Root,
     FileVersion,
     FileType,
     DataModel, 
     DataRoot, 
     DataDimensions,
     Dimension,
     DataRecords,
     Record,
     MetaData,
     Required,
     UserDefined,
     DataPath,
     Data,     // Data Dataset
     Dataset  // Unknown datasets
   };
   

   
  /**
  * @brief Used to create a new MXANode
  * @param type The Type of Node this is going to be
  * @param nodeName The Name of this node
  * @return A Boost SharedPointer to the MXANode Object
  */
  static MXANodePtr New(int32 type, std::string nodeName);
  

  
  // Virtual Destructor in case we are subclassed.
  virtual ~MXANode();  //Needs to be virtual if someone wants to subclass
  
  /// Accessor for NodeType iVar
  void setNodeType(int32 nodeType);
  int32 getNodeType();
  std::string getNodeTypeString();
  
  /**
   * @brief Returns the node type as a string
   * @param node_type The type of node
   * @return The node type as a string
   */
  static std::string getNodeTypeAsString(int32 node_type);
  
  
  /// Accessors for NodeName iVar
  virtual void setNodeName(std::string nodeName);
  std::string getNodeName();
  

  
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
  void setParent(MXANodeWeakPtr parent);
  MXANodeWeakPtr getParent();
  
  // Children Methods
  int32 getNumChildren() const;
  bool hasChildren() const;
  void addChild(MXANodePtr child);
  void removeChild(int index);
  void removeChild(MXANodePtr child);
  int32 indexOfChild(MXANodePtr child);
  int32 indexOfChild(MXANode* child);
  MXANodePtr getChildAt(int32 index);
  MXANodeChildren& getChildren();
  
  // Template Method for native types
  template<typename T>
  void setAttribute(std::string key, T value)
  {
    boost::any v(value);
    MXAAttributePtr attr = MXAAttribute::createAttribute(key, v);
    _nodeAttributes[key] = attr;
  }

  // Removes the Attribute
  void removeAttribute(std::string);

  // Utilities
  void printNodeTree(int32 depth=0);
  virtual void printNode(std::ostream& os, int32 indentSize=0);
  
  
  // -------- Needed for Parent Child relationship ------------
  void _setWeakPtr(MXANodeWeakPtr weakPtr);
  
protected:
  
  MXANode(){}; //Protect this constructor
  MXANode(int32 nodeType, std::string nodeName); // This is the normal constructor
  
  // Node Name
  std::string _nodeName;
  int32 _nodeType;
  int32 _uniqueId;
  MXANodeWeakPtr _selfPtr;
  MXANodeWeakPtr _parent;
  MXANodeChildren _children;
  Attributes _nodeAttributes;
  
  std::string _indent(int);

  private:
      MXANode(const MXANode&);   //Copy Constructor Not Implemented
      void operator=(const MXANode&); //Copy Assignment Not Implemented

};

#endif /* _MHDNODE_H_ */
