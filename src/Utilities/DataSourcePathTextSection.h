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
* @brief 
* @author Mike Jackson
* @date Nov 2007
* @version $Revision: 1.1 $
*/
class MXA_EXPORT DataSourcePathTextSection : public IStringSection
{
public:
  explicit DataSourcePathTextSection(const std::string &text);
	virtual ~DataSourcePathTextSection();
	
 //std::string toString( std::vector<uint32> &dims, int8 &ok);
 std::string toString (int value, int8 &ok);
 
 void setPreText(const std::string &preText);
 void setPostText(const std::string &postText);
 
  private:
    std::string _text;
    std::string _preText;
    std::string _postText;
    
  DataSourcePathTextSection(const DataSourcePathTextSection&);    // Copy Constructor Not Implemented
  void operator=(const DataSourcePathTextSection&);  // Operator '=' Not Implemented
  
};

#endif /*DataSourcePathTextSection_H_*/

