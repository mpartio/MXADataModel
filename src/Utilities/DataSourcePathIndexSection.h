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
#ifndef DataSourcePathIndexSection_H_
#define DataSourcePathIndexSection_H_

#include <Common/MXATypes.h>
#include <Common/MXATypeDefs.h>
#include <Utilities/IStringSection.h>

#include <string>
#include <vector>

/**
* @class DataSourcePathIndexSection DataSourcePathIndexSection.h Utilities/DataSourcePathIndexSection.h
* @brief 
* @author Mike Jackson
* @date Nov 2007
* @version $Revision: 1.1 $
*/
class MXA_EXPORT DataSourcePathIndexSection : public IStringSection
{

public:
  DataSourcePathIndexSection( int32 dimIndex, int8 fillChar, int32 width, const std::string &numericType);
   
  virtual ~DataSourcePathIndexSection();
   
  void setPreText(const std::string &preText);
  void setPostText(const std::string &postText);
  
 //std::string toString( std::vector<uint32> &dims, int8 &ok);
  std::string toString (int value, int8 &ok);
   
 private:
   int32 _index;
   int8 _fillChar;
   int32 _width;
   std::string _numericType;
   std::string _preText;
   std::string _postText;
	
   DataSourcePathIndexSection(const DataSourcePathIndexSection&);    // Copy Constructor Not Implemented
  void operator=(const DataSourcePathIndexSection&);  // Operator '=' Not Implemented
};


#endif /*DataSourcePathIndexSection_H_*/
