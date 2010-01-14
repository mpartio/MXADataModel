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
#ifndef _MXADATAMODELNODE_H_
#define _MXADATAMODELNODE_H_

#include <MXA/MXATypes.h>
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Base/IDataDimension.h>
#include <MXA/Core/MXAConstants.h>
#include <MXA/Utilities/StringUtils.h>

//#include <boost/shared_ptr.hpp>

#include <limits>


/**
 * @brief A concrete implementation of the IDataDimension Abstract Class.
 *
 * @author Mike Jackson @ BlueQuartz Software.us
 * @date March 2007
 * @version $Revision: 1.2 $
 *
 */
class  MXA_EXPORT MXADataDimension : public IDataDimension
{

public:
  MXA_SHARED_POINTERS(MXADataDimension);
  MXA_TYPE_MACRO(MXADataDimension)
  typedef std::vector<Pointer>            Container;

    /**
  * @brief Used to create a new MXANode
   * @param name The name for the Data Dimensions
   * @param altName An Alternate Name for the Data Dimension
   * @param index The index of this dimension
   * @param count The total number of
   * @param startValue The value this dimension starts at
   * @param endValue The ending value (inclusive) this dimension ends at
   * @param increment The value to increment the dimension when iterating
   * @param uniform Are the values uniform across the start to end values
  * @return A Boost SharedPointer to the MXANode Object
  */
  static MXADataDimension::Pointer New(std::string name, std::string altName,
        int32_t index = std::numeric_limits<int32_t>::min(),
        int32_t count = std::numeric_limits<int32_t>::min(),
        int32_t startValue  = std::numeric_limits<int32_t>::max(),
        int32_t endValue = std::numeric_limits<int32_t>::max(),
        int32_t increment = std::numeric_limits<int32_t>::max(),
        int32_t uniform = std::numeric_limits<int32_t>::min()
  );

  /**
  * @brief Performs a Deep copy of passed in MXADataDimension object
  * @param dim The MXADataDimension object to copy
  */
  static IDataDimension::Pointer New(IDataDimension::Pointer dim)
  {
    return MXADataDimension::New( dim->getDimensionName(), dim->getAltName(), dim->getIndex(),
                          dim->getCount(), dim->getStartValue(), dim->getEndValue(),
                          dim->getIncrement(), dim->getUniform() );
  }

//  #warning Add another static constructor that takes no args.

  virtual ~MXADataDimension();



  /**
  * @brief Setter for property Index
  * @param aValue The new value to set for property Index
  */
  void setIndex(int32_t aValue);

  /**
  * @brief Getter for property Index
  * @return The value of Index
  */
  int32_t getIndex();

  /**
  * @brief Setter for property Count
  * @param aValue The new value to set for property Count
  */
  void setCount(int32_t aValue) { _count = aValue; }

  /**
  * @brief Getter for property Count
  * @return The value of Count
  */
  int32_t getCount() { return _count; }

  /**
  * @brief Setter for property startValue
  * @param aValue The new value to set for property startValue
  */
  void setStartValue(int32_t aValue) { _startValue = aValue; }

  /**
  * @brief Getter for property startValue
  * @return The value of startValue
  */
  int32_t getStartValue() { return _startValue; }

  /**
  * @brief Setter for property increment
  * @param aValue The new value to set for property increment
  */
  void setIncrement(int32_t aValue) { _increment = aValue; }

  /**
  * @brief Getter for property increment
  * @return The value of increment
  */
  int32_t getIncrement() { return _increment; }

  /**
  * @brief Setter for property endValue
  * @param aValue The new value to set for property endValue
  */
  void setEndValue(int32_t aValue) { _endValue = aValue; }

  /**
  * @brief Getter for property endValue
  * @return The value of endValue
  */
  int32_t getEndValue() { return _endValue; }

  /**
  * @brief Setter for property name
  * @param aValue The new value to set for property name
  */
  void setDimensionName(std::string aValue) { _dimensionName = aValue; }

  /**
  * @brief Getter for property name
  * @return The value of name
  */
  std::string getDimensionName() { return _dimensionName; }

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
  * @brief Setter for property uniform
  * @param aValue The new value to set for property uniform
  */
  void setUniform(int32_t aValue) { _uniform = aValue; }

  /**
  * @brief Getter for property uniform
  * @return The value of uniform
  */
  int32_t getUniform() { return _uniform; }

  int32_t writeDimension(IDataDimensionWriter* writer);

 // Over ride from Superclass
  void setNodeName(std::string nodeName);
  // Print out the node information to the console
  void printNode(std::ostream& os, int32_t indent);

  /**
   * @brief Returns the maximum value that the Start Value can have */
  IDataDimension::size_type maxStartValue() { return std::numeric_limits<int32_t>::max(); }

  /** @brief Returns the maximum value that the End Value can have  */
  IDataDimension::size_type  maxEndValue() { return std::numeric_limits<int32_t>::max(); }

  /** @brief Returns the Maximun Increment value */
  IDataDimension::size_type  maxIncrement() { return std::numeric_limits<int32_t>::max(); }

  /** @brief returns the maximum value that the Count can be */
  IDataDimension::size_type  maxCount() { return std::numeric_limits<int32_t>::max(); }

  /** @brief   */
  bool isPropertyInitialized(int32_t value)
  {
    if (value == std::numeric_limits<int32_t>::max() ||
        value == std::numeric_limits<int32_t>::min() )
      { return false; }

    return true;
  }

  bool isValid(std::string &message);

  int32_t calculateCount();


protected:
  MXADataDimension();

  int32_t _index;
  int32_t _count;
  int32_t _startValue;
  int32_t _endValue;
  int32_t _increment;
  int32_t _uniform;
  std::string _dimensionName;
  std::string _altName;
  std::string _nodeName;
  std::string _nodeType;

private:
    MXADataDimension(const MXADataDimension&);   //Copy Constructor Not Implemented
    void operator=(const MXADataDimension&); //Copy Assignment Not Implemented
};



#endif /* _MXADATAMODELNODE_H_ */

