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
#error NOTHING SHOULD BE INCLUDING THIS FILE
#ifndef _IMXANODE_H_
#define _IMXANODE_H_

//-- MXA includes
#include "MXA/MXADLLExport.h"
#include <MXA/Common/MXATypeDefs.h>
//-- STL Includes
#include <string>


/**
 * @brief Interface declaration for objects that want to implement a "Tree" type
 * data structure
 * @author Mike Jackson
 * @date March 2007
 * @version $Revision: 1.2 $
 *  
 */
class MXA_EXPORT INode 
{
  public:

    /**
     * @brief Types that an INode can take
     */
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
     
    INode(){} ;
    virtual ~INode(){};
    
    /// Accessor for NodeType iVar
    /**
    * @brief Sets the Node Type
    * @param nodeType The type of node, from the Type Enumeration
    */
    virtual void setNodeType(int32_t nodeType) = 0;

    /**
    * @brief Gets the node type.
    * @return The node type
    */
    virtual int32_t getNodeType() = 0;

    /**
    * @brief Returns the node type as a string
    * @return The Node type as a string
    */
    virtual std::string getNodeTypeString() = 0;
    
    /**
    * @brief Static function to return a human readable string of the Node Type
    * @param nodeType One of the Type Enumerations
    * @return String description of the node type
    */
    static std::string getNodeTypeAsString(int32_t nodeType)
    {
      switch(nodeType) {
        case Root:
          return "Root";
        case FileVersion:
          return "File Version";
        case FileType:
          return "File Type";
        case DataModel:
          return "Data Model";
        case DataRoot:
          return "Data Root";
        case DataDimensions:
          return "Data Dimensions";
        case Dimension:
          return "Dimension";
        case DataRecords:
          return "Data Records";
        case Record:
          return "Record";
        case MetaData:
          return "Meta Data";
        case Required:
          return "Required Meta Data";
        case UserDefined:
          return "User Defined Meta Data";
        case DataPath:
          return "Data Path";
        case Data:
          return "Data";
        case Dataset:
          return "Unknown Dataset";
        default:
          return "Unknown";
        }
    }
 

    /**
    * @brief Sets the node name 
    * @param nodeName The name to give to this node
    */
    virtual void setNodeName(std::string nodeName) = 0;

    /**
    * @brief Returns the Name of this node
    */
    virtual std::string getNodeName() = 0;

    /// Accessor for Parent iVar
    /**
    * @brief Sets the Parent node
    * @param parent The Parent node for this node
    */
    virtual void setParent(INodeWeakPtr parent) = 0;


    /**
    * @brief Returns the Parent Node
    */
    virtual INodeWeakPtr getParent() = 0;
    
    /**
    * @brief Setter for property guid
    * @param aValue The new value to set for property guid
    */
    virtual void setUniqueId(int32_t aValue) = 0;
    
    /**
    * @brief Getter for property guid
    * @return The value of guid
    */
    virtual int32_t getUniqueId() = 0;
    
    /**
     * @brief Resets the GUID value for this node
     */
    virtual void resetGUIDValue() = 0;

    // Children Methods
    /**
    * @brief Returns the number of Children of this node
    */
    virtual int32_t getNumChildren() const = 0;


    /**
    * @brief Returns TRUE if this node has children
    */
    virtual bool hasChildren() const = 0;  

    /**
    * @brief Adds a child to this node
    * @param child The child to add to this node
    */
    virtual void addChild(INodePtr child) = 0;

    /**
    * @brief Removes a child from this node
    * @param index The index of the child to remove
    */
    virtual void removeChild(int index) = 0;

    /**
    * @brief Removes the given child from this node
    * @param child The child to remove
    */
    virtual void removeChild(INode* child) = 0;

  
    /**
    * @brief Returns the index of a specific child
    * @param child The child object whose index we are interested in
    */
    virtual int32_t indexOfChild(INode* child) = 0;

    /**
    * @brief Returns a specific child of this node based on a given index
    * @param index The index of the child to return
    * @return The index of the child
    */
    virtual INodePtr getChildAt(int32_t index) = 0;
    
    /**
    * @brief Returns a reference to the children of this node
    */
    virtual INodes& getChildren() = 0;


    /**
    * @brief Removes an Attribute from this node
    * @param attrName The name of the attribute to remove
    */
    virtual void removeAttribute(std::string attrName) = 0;
    

    /**
    * @brief prints the node and the children of this node
    * @param depth The amount of indentation space
    */
    virtual void printNodeTree(int32_t depth=0) = 0;
    
    /**
    * @brief prints the node and the children of this node
    * @param os An ostream to print the node to
    * @param indentSize The amount of indentation space
    */
    virtual void printNode(std::ostream& os, int32_t indentSize=0) = 0;
  
};

#endif /*_IMXANODE_H_*/
