#include <MXA/Common/MXAEndian.h>
#include <MXA/Common/IO/Reader64.h>




// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Reader64::Reader64(const std::string &filename) :
  _filename(filename)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Reader64::~Reader64()
{
#if defined (WINDOWS_LARGE_FILE_SUPPORT)
  int error = CloseHandle(_instream); //Close the file
  if (0 == error)
  {
    std::cout << "Reader64:: Error Closing File " << _filename << std::endl;
  }
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool Reader64::initReader()
{
#if defined (WINDOWS_LARGE_FILE_SUPPORT)

	_instream = CreateFile(TEXT(this->_filename.c_str()),    // file to open
                   GENERIC_READ,          // open for reading
                   FILE_SHARE_READ,       // share for reading
                   NULL,                  // default security
                   OPEN_EXISTING,         // existing file only
                   FILE_FLAG_SEQUENTIAL_SCAN, // normal file
                   NULL);                 // no attr. template
  if (_instream == INVALID_HANDLE_VALUE)
  {
    std::cout << "Error: Failed to open file: " + std::string(this->_filename) << " Error code:(" << GetLastError() << ")" << std::endl;
    return false;
  }
#else
  // Open a stream with a large buffer.

  _buffer.resize(BUFF_SIZE, 0);
  _instream.rdbuf()->pubsetbuf ( &(_buffer.front()), BUFF_SIZE );
  _instream.open ( _filename.c_str(), std::ifstream::in | std::ifstream::binary );
  if ( !_instream.is_open() ) {
    std::cout <<  "Error: Failed to open file: " + _filename << std::endl;
    return false;
  }
#endif
  return true;
}



