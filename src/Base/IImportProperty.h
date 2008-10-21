///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) Mike Jackson,
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _IIMPORT_PROPERTY_H_
#define _IIMPORT_PROPERTY_H_

///////////////////////////////////////////////////////////////////////////////

//C++ includes
#include <string>
#include <iostream>

//-- Boost includes
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>

//------------------------------------------------------------------------------
// Macros for Properties
/**
* @brief Creates a std::string constant for the Property so that the property
* can be retrieved by name.
*/
#define MXA_PROPERTY_CONSTANT(prpty) \
  const std::string prpty ( #prpty );

/**
* @brief Creates a "setter" method to set the property.
*/
#define MXA_SET_PROPERTY(type, prpty, varname) \
  void set##prpty(type value) { this->varname = value; }

/**
* @brief Creates a "getter" method to retrieve the value of the property.
*/
#define MXA_GET_PROPERTY(type, prpty, varname) \
  type get##prpty() { return varname; }

/**
* @brief Convenience macro to create both the setter and getter methods.
*/
#define MXA_PROPERTY(type, prpty, varname) \
  MXA_SET_PROPERTY(type, prpty, varname)\
  MXA_GET_PROPERTY(type, prpty, varname)


/**
* @brief Creates a "setter" method to set the property.
*/
#define MXA_SET_STRING_PROPERTY( prpty, varname) \
  void set##prpty(const std::string &value) { this->varname = value; }

/**
* @brief Creates a "getter" method to retrieve the value of the property.
*/
#define MXA_GET_STRING_PROPERTY( prpty, varname) \
  std::string get##prpty() { return varname; }


#define MXA_STRING_PROPERTY(prpty, varname)\
  MXA_SET_STRING_PROPERTY(prpty, varname)\
  MXA_GET_STRING_PROPERTY(prpty, varname)

/**
* @brief Creates an if conditional where the key is tested against the values constant
* and if a match found then the property value is set
* @param name_space
* @param type
* @param prpty
* @param key
* @param value
*/
#define SET_PROPERTY_BODY(name_space, type, prpty, key, value) \
  if (name_space::prpty.compare(key) == 0) { \
    try { \
      this->set##prpty(boost::lexical_cast<type>(value)); return 1; \
    }  catch(boost::bad_lexical_cast &excp) { \
      std::cout << excp.what() << std::endl; \
      std::cout << "Could not convert value '" << value << "' to type '" << #type << "' for property '" << #prpty << "'" << std::endl; \
    } \
  }

/**
* @brief Creates an if condition where the key is tested against the values constant
* and if a match found the the property is returned using the 'value' argument
* @param class
* @param type
* @param prpty
* @param varname
* @param key
* @param value
*/
#define GET_PROPERTY_BODY(name_space, type, prpty, varname, key, value)\
  if (name_space::prpty.compare(key) == 0) {  \
  try { value = boost::any_cast<type>(varname); return 1;} \
  catch(boost::bad_any_cast &) { std::cout << "Could not cast value '" << value << "' to type '" << #type << "' for property '" << #prpty << "'" << std::endl; } }

//
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief
 * @author Mike Jackson
 * @date Septermber 2008
 * @version $Revision: 1.3 $
 *
 */
class MXA_EXPORT IImportProperty
{

public:
  IImportProperty(){};
  virtual ~IImportProperty(){};

  virtual int32 setProperty(const std::string &key, const std::string &value) = 0;

  virtual int32 getProperty(const std::string &key, std::string &value) = 0;


};

#endif /*_IIMPORT_PROPERTY_H_*/
