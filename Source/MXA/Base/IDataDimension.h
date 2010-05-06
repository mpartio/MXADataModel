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
#ifndef IDATADIMENSION_H_
#define IDATADIMENSION_H_

#include <MXA/MXATypes.h>
#include <MXA/Common/DLLExport.h>
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Base/IDataDimensionWriter.h>

#include <vector>

/**
 * @brief The formal interface description for a DataDimension
 * @author Mike Jackson
 * @date March 2007
 * @version $Revision: 1.2 $
 *
 */
class MXA_EXPORT IDataDimension
{
  public:
    IDataDimension() {};
    virtual ~IDataDimension() {};

    MXA_SHARED_POINTERS(IDataDimension);
    typedef std::vector<Pointer>            Container;

    typedef int32_t size_type;

    // Print out the node information to the console
    virtual void printNode(std::ostream& os, int32_t indent) = 0;

    /**
     * @brief Setter for property Index
     * @param aValue The new value to set for property Index
     */
     virtual void setIndex(int32_t aValue) = 0;

     /**
     * @brief Getter for property Index
     * @return The value of Index
     */
     virtual int32_t getIndex() = 0;

     /**
     * @brief Setter for property Count
     * @param aValue The new value to set for property Count
     */
     virtual void setCount(int32_t aValue) = 0;

     /**
     * @brief Getter for property Count
     * @return The value of Count
     */
     virtual int32_t getCount() = 0;

     /**
     * @brief Setter for property startValue
     * @param aValue The new value to set for property startValue
     */
     virtual void setStartValue(int32_t aValue) = 0;

     /**
     * @brief Getter for property startValue
     * @return The value of startValue
     */
     virtual int32_t getStartValue() = 0;

     /**
     * @brief Setter for property increment
     * @param aValue The new value to set for property increment
     */
     virtual void setIncrement(int32_t aValue) = 0;

     /**
     * @brief Getter for property increment
     * @return The value of increment
     */
     virtual int32_t getIncrement() = 0;

     /**
     * @brief Setter for property endValue
     * @param aValue The new value to set for property endValue
     */
     virtual void setEndValue(int32_t aValue) = 0;

     /**
     * @brief Getter for property endValue
     * @return The value of endValue
     */
     virtual int32_t getEndValue() = 0;

     /**
     * @brief Setter for property name
     * @param aValue The new value to set for property name
     */
     virtual void setDimensionName(std::string aValue) = 0;

     /**
     * @brief Getter for property name
     * @return The value of name
     */
     virtual std::string getDimensionName() = 0;

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
     * @brief Setter for property uniform
     * @param aValue The new value to set for property uniform
     */
     virtual void setUniform(int32_t aValue) = 0;

     /**
     * @brief Getter for property uniform
     * @return The value of uniform
     */
     virtual int32_t getUniform() = 0;

     /**
      * @brief This method uses the given IDataDimensionWriter to serialize itself
      * to the underlying data storage system.
      * @param writer Valid (Non Null) IDataDimensioWriter object
      * @return Error condition.
      */
     virtual int32_t writeDimension(IDataDimensionWriter* writer) = 0;

     /**
      * @brief Returns the maximum value that the Start Value can have */
     virtual IDataDimension::size_type  maxStartValue() = 0;

     /** @brief Returns the maximum value that the End Value can have  */
     virtual IDataDimension::size_type  maxEndValue() = 0;

     /** @brief Returns the Maximun Increment value */
     virtual IDataDimension::size_type  maxIncrement() = 0;

     /** @brief returns the maximum value that the Count can be */
     virtual IDataDimension::size_type  maxCount() = 0;

     /**
      * @brief Returns is a property, like count, increment.. is equal to either the
      * maximum or minimum value for its type, indicating that the value is uninitialized.
      */
     virtual bool isPropertyInitialized(int32_t value) = 0;

     /**
      * @brief Checks some basic properties of the model to make sure they are
      * set correctly.
      * @param message String to hold messages concerning errors/omissions from
      * the model
      * @return True if the model passes the basic checks
      */
     virtual bool isValid(std::string &message) = 0;

     /**
      * @brief This will force the count value to be calculated based on the
      * current start, end and increment values
      */
     virtual int32_t calculateCount() = 0;


  private:
    IDataDimension(const IDataDimension&);    //Not Implemented
    void operator=(const IDataDimension&); //Not Implemented
};

#endif /*IDATADIMENSION_H_*/
