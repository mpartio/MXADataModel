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
#ifndef XMLCONSTANTS_H_
#define XMLCONSTANTS_H_

#include <string>

/**
 * @brief Constants for dealing with MXA XML files
 * @version $Revision: 1.6 $
 */


// Namespace for XML Constants
namespace MXA_XML {
    const std::string File_Root ("File_Root");
    const std::string Data_Model ("Data_Model");
    const std::string Data_Root ("Data_Root");
    const std::string Data_Dimensions ("Data_Dimensions");
    const std::string Dimension ("Dimension");
    const std::string Data_Records ("Data_Records");
    const std::string Signal ("Signal");
    const std::string Signal_Group ("Signal_Group");
    const std::string Meta_Data ("Meta_Data");
    const std::string Required_MD ("Required_MD");
    const std::string UserDefined_MD ("UserDefined_MD");
    const std::string Model_Type("Model_Type");
    const std::string Model_Version("Model_Version");
    
    
    namespace Attribute {
      const std::string Name("Name");
      const std::string AltName("Alt_Name");
    }
    
    namespace UserMetaData
    {
      const std::string Tag ("UserMetaData");
      const std::string Key ("key");
      const std::string Dims("dims");
      const std::string Type("type");
    }

    
} // End Namespace MXA_XML



#endif /*XMLCONSTANTS_H_*/
