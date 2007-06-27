
#include "XMLEvtHandler.h"


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
XMLEvtHandler::XMLEvtHandler() :
  _error(0)
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
XMLEvtHandler::~XMLEvtHandler()
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void XMLEvtHandler::OnStartElement(const XML_Char* name, const XML_Char** attrs)
{
    std::string currentTag(name);
    if ( currentTag.compare(MXA_XML::Data_Dimensions) == 0 )
    {
          onData_DimensionsStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Data_Model) == 0 )
    {
          onData_ModelStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Data_Records) == 0 )
    {
          onData_RecordsStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Data_Root) == 0 )
    {
          onData_RootStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Dimension) == 0 )
    {
          onDimensionStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::File_Root) == 0 )
    {
          onFile_RootStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Meta_Data) == 0 )
    {
          onMeta_DataStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Required_MD) == 0 )
    {
          onRequired_MDStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Signal) == 0 )
    {
          onSignalStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Signal_Group) == 0 )
    {
          onSignal_GroupStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::UserDefined_MD) == 0 )
    {
          onUserDefined_MDStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::UserMetaData) == 0 )
    {
          onUserMetaDataStartTag(name, attrs);
    }
} // End OnStartElement(...)


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void XMLEvtHandler::OnEndElement(const XML_Char* name)
{

    std::string currentTag(name);
    if ( currentTag.compare(MXA_XML::Data_Dimensions) == 0 )
    {
          onData_DimensionsEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Data_Model) == 0 )
    {
          onData_ModelEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Data_Records) == 0 )
    {
          onData_RecordsEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Data_Root) == 0 )
    {
          onData_RootEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Dimension) == 0 )
    {
          onDimensionEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::File_Root) == 0 )
    {
          onFile_RootEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Meta_Data) == 0 )
    {
          onMeta_DataEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Required_MD) == 0 )
    {
          onRequired_MDEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Signal) == 0 )
    {
          onSignalEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Signal_Group) == 0 )
    {
          onSignal_GroupEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::UserDefined_MD) == 0 )
    {
          onUserDefined_MDEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::UserMetaData) == 0 )
    {
          onUserMetaDataEndTag(name);
    }
} // End OnEndElement(...)


// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Data_Dimensions> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onData_DimensionsStartTag(const XML_Char* name, const XML_Char** attrs)
{

    printf("Starting %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Data_Model> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onData_ModelStartTag(const XML_Char* name, const XML_Char** attrs)
{

    printf("Starting %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Data_Records> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onData_RecordsStartTag(const XML_Char* name, const XML_Char** attrs)
{

    printf("Starting %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Data_Root> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onData_RootStartTag(const XML_Char* name, const XML_Char** attrs)
{

    printf("Starting %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Dimension> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onDimensionStartTag(const XML_Char* name, const XML_Char** attrs)
{

    printf("Starting %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <File_Root> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onFile_RootStartTag(const XML_Char* name, const XML_Char** attrs)
{

    printf("Starting %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Meta_Data> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onMeta_DataStartTag(const XML_Char* name, const XML_Char** attrs)
{

    printf("Starting %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Required_MD> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onRequired_MDStartTag(const XML_Char* name, const XML_Char** attrs)
{

    printf("Starting %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Signal> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onSignalStartTag(const XML_Char* name, const XML_Char** attrs)
{

    printf("Starting %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Signal_Group> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onSignal_GroupStartTag(const XML_Char* name, const XML_Char** attrs)
{

    printf("Starting %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <UserDefined_MD> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onUserDefined_MDStartTag(const XML_Char* name, const XML_Char** attrs)
{

    printf("Starting %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <UserMetaData> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onUserMetaDataStartTag(const XML_Char* name, const XML_Char** attrs)
{

    printf("Starting %s\n", name); 
}

//______________________________________________________________________________
//------------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Data_Dimensions> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onData_DimensionsEndTag(const XML_Char* name)
{

    printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Data_Model> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onData_ModelEndTag(const XML_Char* name)
{

    printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Data_Records> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onData_RecordsEndTag(const XML_Char* name)
{

    printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Data_Root> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onData_RootEndTag(const XML_Char* name)
{

    printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Dimension> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onDimensionEndTag(const XML_Char* name)
{

    printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <File_Root> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onFile_RootEndTag(const XML_Char* name)
{

    printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Meta_Data> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onMeta_DataEndTag(const XML_Char* name)
{

    printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Required_MD> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onRequired_MDEndTag(const XML_Char* name)
{

    printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Signal> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onSignalEndTag(const XML_Char* name)
{

    printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Signal_Group> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onSignal_GroupEndTag(const XML_Char* name)
{

    printf("Ending %s\n", name); 
}


// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <UserDefined_MD> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onUserDefined_MDEndTag(const XML_Char* name)
{

    printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <UserMetaData> 
// -----------------------------------------------------------------------------
void XMLEvtHandler::onUserMetaDataEndTag(const XML_Char* name)
{

    printf("Ending %s\n", name); 
}

