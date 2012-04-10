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
#ifndef IDATARECORD_H_
#define IDATARECORD_H_

#include "MXA/MXADLLExport.h"
#include <MXA/Common/MXASetGetMacros.h>

#include <vector>
#include <map>

class IDataRecordWriter;

/**
 * @brief Abstract class defining the DataRecord public interface
 * @author Mike Jackson
 * @date March 2007
 * @version $Revision: 1.2 $
 *
 */
class MXA_EXPORT IDataRecord
{
  public:
    IDataRecord() {};
    virtual ~IDataRecord() {};

    MXA_SHARED_POINTERS(IDataRecord);
    MXA_TYPE_MACRO(IDataRecord);
    typedef std::vector<Pointer>          Container;
    typedef std::map<int32_t, Pointer>      LookupTable;
  //  typedef boost::weak_ptr<IDataRecord>              WeakPointer;

    /**
     * @brief Setter for property name
     * @param aValue The new value to set for property name
     */
     virtual void setRecordName(std::string aValue) = 0;

     /**
     * @brief Getter for property name
     * @return The value of name
     */
     virtual std::string getRecordName() = 0;

     /**
     * @brief Setter for property altName
     * @param aValue The new value to set for property altName
     */
     virtual void setAltName(std::string aValue) = 0;

     /**
     * @brief Getter for property altName
     * @return The value of altName
     */
     virtual std::string getAltName() = 0;


     /**
     * @brief Setter for property luid
     * @param aValue The new value to set for property luid
     */
     virtual void setLuid(int32_t aValue) = 0;

     /**
     * @brief Getter for property luid
     * @return The value of luid
     */
     virtual int32_t getLuid() = 0;

     /**
     * @brief Setter for property guid
     * @param aValue The new value to set for property guid
     */
     virtual void setGuid(int32_t aValue) = 0;

     /**
     * @brief Getter for property guid
     * @return The value of guid
     */
     virtual int32_t getGuid() = 0;


     virtual std::string generatePath()  = 0;
     virtual std::string generateParentPath()  = 0;

     virtual int32_t writeRecord(IDataRecordWriter* writer) = 0;

     /**
      * @brief Checks some basic properties of the model to make sure they are
      * set correctly.
      * @param message String to hold messages concerning errors/omissions from
      * the model
      * @return True if the model passes the basic checks
      */
     virtual bool isValid(std::string &message) = 0;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
    /// Accessor for Parent iVar
    /**
    * @brief Sets the Parent node
    * @param parent The Parent node for this node
    */
    virtual void setParent(IDataRecord::WeakPointer parent) = 0;


    /**
    * @brief Returns the Parent Node
    */
    virtual IDataRecord::WeakPointer getParent() = 0;

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
    virtual void addChild(IDataRecord::Pointer child) = 0;

    /**
    * @brief Removes a child from this node
    * @param index The index of the child to remove
    */
    virtual void removeChild(int index) = 0;

    /**
    * @brief Removes the given child from this node
    * @param child The child to remove
    */
    virtual void removeChild(IDataRecord* child) = 0;

    /**
    * @brief Removes all the children of this Data Record
    */
    virtual void removeAllChildren() = 0;

    /**
    * @brief Returns the index of a specific child
    * @param child The child object whose index we are interested in
    */
    virtual int32_t indexOfChild(IDataRecord* child) = 0;

    /**
    * @brief Returns a specific child of this node based on a given index
    * @param index The index of the child to return
    * @return The index of the child
    */
    virtual IDataRecord::Pointer getChildAt(int32_t index) = 0;

    /**
    * @brief Returns a reference to the children of this node
    */
    virtual IDataRecord::Container& getChildren() = 0;

#if 0
    /**
    * @brief Removes an Attribute from this node
    * @param attrName The name of the attribute to remove
    */
    virtual void removeAttribute(std::string attrName) = 0;
#endif

    /**
    * @brief prints the node and the children of this node
    * @param depth The amount of indentation space
    */
    virtual void printDataRecordTree(int32_t depth=0) = 0;

    /**
    * @brief prints the node and the children of this node
    * @param os An ostream to print the node to
    * @param indentSize The amount of indentation space
    */
    virtual void printDataRecord(std::ostream& os, int32_t indentSize=0) = 0;




  protected:


  private:
    IDataRecord(const IDataRecord&);    //Not Implemented
    void operator=(const IDataRecord&); //Not Implemented
};

#endif /*IDATARECORD_H_*/
