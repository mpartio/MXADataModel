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
#ifndef _IMXANODE_H_
#define _IMXANODE_H_

//-- MXA includes
#include "Common/DLLExport.h"
#include "Common/MXATypeDefs.h"
//-- STL Includes
#include <string>


/**
 * @brief Interface declaration for objects that want to implement a "Tree" type
 * data structure
 * @author Mike Jackson
 * @date March 2007
 * @version 1.0
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
    * @brief 
    * @param nodeType
    */
    virtual void setNodeType(int32 nodeType) = 0;

    /**
    * @brief 
    * @param getNodeType(
    * @return 
    */
    virtual int32 getNodeType() = 0;

    /**
    * @brief 
    * @param getNodeTypeString(
    * @return 
    */
    virtual std::string getNodeTypeString() = 0;
    
    /**
    * @brief 
    * @param nodeType
    * @return 
    */
    static std::string getNodeTypeAsString(int32 nodeType)
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
    * @brief 
    * @param nodeName
    */
    virtual void setNodeName(std::string nodeName) = 0;

    /**
    * @brief 
    * @return 
    */
    virtual std::string getNodeName() = 0;

    /// Accessor for Parent iVar
    /**
    * @brief 
    * @param parent
    */
    virtual void setParent(MXANodeWeakPtr parent) = 0;


    /**
    * @brief 
    * @return 
    */
    virtual MXANodeWeakPtr getParent() = 0;
    
    /**
    * @brief Setter for property guid
    * @param aValue The new value to set for property guid
    */
    virtual void setUniqueId(int32 aValue) = 0;
    
    /**
    * @brief Getter for property guid
    * @return The value of guid
    */
    virtual int32 getUniqueId() = 0;
    
    /**
     * @brief
     */
    virtual void resetGUIDValue() = 0;

    // Children Methods
    /**
    * @brief 
    * @return 
    */
    virtual int getNumChildren() const = 0;


    /**
    * @brief 
    * @return 
    */
    virtual bool hasChildren() const = 0;  

    /**
    * @brief 
    * @param child
    */
    virtual void addChild(MXANodePtr child) = 0;

    /**
    * @brief 
    * @param index
    */
    virtual void removeChild(int index) = 0;

    /**
    * @brief 
    * @param child
    */
    virtual void removeChild(MXANodePtr child) = 0;

    /**
    * @brief 
    * @param child
    * @return 
    */
    virtual int indexOfChild(MXANodePtr child) = 0;
    
    /**
    * @brief 
    * @param child
    * @return 
    */
    virtual int32 indexOfChild(MXANode* child) = 0;

    /**
    * @brief
    * @param index
    * @return
    */
    virtual MXANodePtr getChildAt(int32 index) = 0;
    
    /**
    * @brief 
    * @return 
    */
    virtual MXANodeChildren& getChildren() = 0;


    /**
    * @brief 
    * @param attrName
    */
    virtual void removeAttribute(std::string attrName) = 0;
    
    
  protected:
    
    
  private:
    //INode(const INode&);    //Not Implemented
    //void operator=(const INode&); //Not Implemented
};

#endif /*_IMXANODE_H_*/
