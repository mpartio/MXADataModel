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
#ifndef DataSourcePathIndexSection_H_
#define DataSourcePathIndexSection_H_

#include "MXA/MXA.h"
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Utilities/IStringSection.h>

#include <string>
#include <vector>

/**
* @class DataSourcePathIndexSection DataSourcePathIndexSection.h Utilities/DataSourcePathIndexSection.h
* @brief Represents the index value (a number) of a data source path.
* @author Mike Jackson
* @date Nov 2007
* @version $Revision: 1.2 $
*/
class MXA_EXPORT DataSourcePathIndexSection : public IStringSection
{

public:

  /**
   * @brief Constructor
   * @param dimIndex The index into the dimensions array that this represents
   * @param fillChar The characters to use as fill values in the generated string
   * @param width The number of characters to use when creating the vnumber
   * @param numericType The type of number to create.
   */
  DataSourcePathIndexSection( int32_t dimIndex, const std::string  &fillChar,
                              int32_t width, const std::string &numericType);

  virtual ~DataSourcePathIndexSection();

  /**
   * @brief Sets the prefix text
   * @param preText The prefix
   */
  void setPreText(const std::string &preText);

  /**
   * @brief Sets the siffix text
   * @param postText The suffix text
   */
  void setPostText(const std::string &postText);

  /**
   * @brief Creates a std::string from all the information given
   * @param value The value to use for the index
   * @param ok Was the string created properly
   * @return The created string
   */
  std::string toString (int value, int8_t &ok);

 private:
   int32_t _index;
   std::string _fillChar;
   int32_t _width;
   std::string _numericType;
   std::string _preText;
   std::string _postText;

   DataSourcePathIndexSection(const DataSourcePathIndexSection&);    // Copy Constructor Not Implemented
  void operator=(const DataSourcePathIndexSection&);  // Operator '=' Not Implemented
};


#endif /*DataSourcePathIndexSection_H_*/
