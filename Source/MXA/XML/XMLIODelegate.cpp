

#include <MXA/XML/XMLIODelegate.h>
#include <MXA/XML/XMLDataModelWriter.h>
#include <MXA/XML/XMLDataModelReader.h>


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
XMLIODelegate::XMLIODelegate() :
  _openFile(""),
  _fileId(-1)
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
XMLIODelegate::~XMLIODelegate()
{
}




// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXATypes::MXAError XMLIODelegate::writeModelToFile(const std::string &fileName, 
                                                    IDataModel::Pointer model, 
                                                    bool closeWhenFinished,
                                                    bool deleteExisting)
{
  int32 success = -1;
  XMLDataModelWriter writer(model, fileName );
  success = writer.writeModelToFile(_fileId);
  return success;
}

// -----------------------------------------------------------------------------
//  Opens the file and then reads the data model from the file. Then closes the
//  when reading is complete.
// -----------------------------------------------------------------------------
MXATypes::MXAError XMLIODelegate::readModelFromFile(const std::string &fileName, IDataModel::Pointer model, bool closeWhenFinished, bool openReadOnly)
{
  int32 success = -1;
  XMLDataModelReader reader(model, fileName);
  success = reader.readDataModel(-1);
  return success;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool XMLIODelegate::supportedMXAFileVersion(float version)
{
  if (version == MXA::MXACurrentFileVersion) {
    return true;
  } else {
    return false;
  }
}

// -----------------------------------------------------------------------------
// Returns true if the file referred to by fileId is an XML file that conforms
// to the DTD for the MXA project compliant file (has the correct file type and version)
// -----------------------------------------------------------------------------
//TODO: Implement XMLIODelegate::isMXAFile(hid_t fileId)
bool XMLIODelegate::isMXAFile(int32 fileId)
{
  MXATypes::MXAError err=0;
  // We can check wellformedness of the XML
  // We could read the model from the file then sanity check the Model for Dimensions/Records/Meta Data
  
  return (err >= 0);
}

// -----------------------------------------------------------------------------
// Returns true if the file referred to by fileId is an XML file that conforms
// to the DTD for the MXA project compliant file (has the correct file type and version)
// -----------------------------------------------------------------------------
//TODO: Implement XMLIODelegate::isMXAFile(std::string filename)
bool XMLIODelegate::isMXAFile(const std::string &filename)
{
  MXATypes::MXAError err=0;
  // We can check wellformedness of the XML
  // We could read the model from the file then sanity check the Model for Dimensions/Records/Meta Data
  return (err >= 0);
}


// -----------------------------------------------------------------------------
// Opens and returns an hdf file identifier
//TODO: Implement XMLIODelegate::openMXAFile(std::string filename, bool readOnly)
int32 XMLIODelegate::openMXAFile(std::string filename, bool readOnly)
{
  int32 err = -1;
  
  return err;
}

// -----------------------------------------------------------------------------
//  This is a no-op because we read the XML file and then always close the file
// ----------------------------------------------------------------------------- 
void XMLIODelegate::closeMXAFile() 
{
 
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 XMLIODelegate::getOpenFileId()
{
  return _fileId;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string XMLIODelegate::getOpenFileName()
{
  return this->_openFile;
}
