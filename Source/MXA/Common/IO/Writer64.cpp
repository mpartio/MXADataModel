#include "Writer64.h"




// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
Writer64::Writer64(const std::string &filename) :
  _filename(filename)
{
  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
Writer64::~Writer64()
{
#if defined (WINDOWS_LARGE_FILE_SUPPORT)
  int error = CloseHandle(_outStream); //Close the file
  if (0 == error)
  {
    std::cout << "Writer64:: Error Closing File " << _filename << std::endl;
  }
#endif
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool Writer64::initWriter()
{
#if defined (WINDOWS_LARGE_FILE_SUPPORT)
   
	_outStream = CreateFile(TEXT(this->_filename.c_str()),    // file to open
                   GENERIC_WRITE,          // open for writing
                   FILE_SHARE_WRITE,       // share for writing
                   NULL,                  // default security
                   CREATE_ALWAYS,         // Always create a new file, regardless if the file exists or not
                   FILE_ATTRIBUTE_NORMAL, // normal file
                   NULL);                 // no attr. template
  if (_outStream == INVALID_HANDLE_VALUE) 
  { 
    std::cout << "Error: Failed to open file: " + std::string(this->_filename) << " Error code:(" << GetLastError() << ")" << std::endl;
    return false;
  }
#else
 
  _outStream.open(this->_filename.c_str(), std::ios::out | std::ios::binary);
  if ( _outStream.fail() ) {
    std::cout << "Output File " << this->_filename << "\n  could not be opened for writing. Please" <<
     " check the path is correct and permissions are set correctly on the parent folder." << std::endl;
     return false;
  }
#endif
  return true;
}

