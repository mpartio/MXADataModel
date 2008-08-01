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
#ifndef DataSourcePathTextSection_H_
#define DataSourcePathTextSection_H_

#include <Common/MXATypes.h>
#include <Common/MXATypeDefs.h>
#include <Utilities/IStringSection.h>

#include <string>
#include <vector>


/**
* @class DataSourcePathTextSection DataSourcePathTextSection.h Utilities/DataSourcePathTextSection.h
* @brief Represents the text section of a Data Source path.
* @author Mike Jackson
* @date Nov 2007
* @version $Revision: 1.2 $
*/
class MXA_EXPORT DataSourcePathTextSection : public IStringSection
{
public:
  /**
   * @brief Creates a text section from a string
   * @param text The text to use in this section
   */
  explicit DataSourcePathTextSection(const std::string &text);
	virtual ~DataSourcePathTextSection();


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
  std::string toString (int value, int8 &ok);

  private:
    std::string _text;
    std::string _preText;
    std::string _postText;

  DataSourcePathTextSection(const DataSourcePathTextSection&);    // Copy Constructor Not Implemented
  void operator=(const DataSourcePathTextSection&);  // Operator '=' Not Implemented

};

#endif /*DataSourcePathTextSection_H_*/

