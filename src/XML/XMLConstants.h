#ifndef XMLCONSTANTS_H_
#define XMLCONSTANTS_H_

#include <string>


// Our namespace for XML Constants
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

    
} // End Namespace



#endif /*XMLCONSTANTS_H_*/
