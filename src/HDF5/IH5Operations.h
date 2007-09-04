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
#ifndef IH5OPERATIONS_
#define IH5OPERATIONS_

#include <hdf5.h>

/**
 * @brief This is meant to be a superclass that encapsulates certain HDF5 open/close
 * operations. The idea is to instantiate a class each time you need to do an 
 * open/close operation, then put the class on a stack. At the end of a series
 * of operations, you can 'pop' each class from the stack and then call 'close' 
 * and the HDF5 object will be closed, thus saving resource leaks. The thinking
 * was to combine this with the boost::shared_ptr and create a
 * std::stack<boost::shared_ptr<IH5Operations*> >. That way you can just 'pop'
 * each pointer from the stack and the destructor would be called on the object.
 */
class IH5Operations
{

  public:
    IH5Operations() :
      _id(-1), _closeErr(-1), _openErr(-1)
    {
    }

    virtual ~IH5Operations()
    {
    }

    virtual bool isIdValid()
    {
      return (_id >= 0);
    }

    virtual hid_t getObjectId()
    {
      return _id;
    }
    virtual herr_t open(hid_t locId, const std::string &datasetPath) = 0;
    virtual herr_t close() = 0;
   
    herr_t getCloseError()
    {
      return _closeErr;
    }
    
    herr_t getOpenError()
    {
      return _openErr;
    }

  protected:
    hid_t _id;
    herr_t _closeErr;
    herr_t _openErr;
};


/**
 * @brief
 * @class
 */
class H5TOperations : public IH5Operations
{
  public:
    H5TOperations() {}
    virtual ~H5TOperations() {
      if ( isIdValid()  ) { close(); } //Make sure the id is closed
    }
    
    herr_t open(hid_t locId, const std::string &datasetPath)
    {
      _id = H5Topen(locId, datasetPath.c_str() );
      _openErr = _id;
    }
    
    herr_t close()
    {
      _closeErr = H5Tclose(_id);
      if (_closeErr >= 0) { _id = -1; } // Reset the _id variable to invalidate it
    }
};

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
class H5DOperations : public IH5Operations
{
  public:
    H5DOperations() {}
    virtual ~H5DOperations() {
      if ( isIdValid()  ) { close(); } //Make sure the id is closed
    }
    
    herr_t open(hid_t locId, const std::string &datasetPath)
    {
      _id = H5Dopen(locId, datasetPath.c_str() );
      _openErr = _id;
    }
    
    herr_t close()
    {
      _closeErr = H5Tclose(_id);
      if (_closeErr >= 0) { _id = -1; } // Reset the _id variable to invalidate it
    }
};

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
class H5SOperations : public IH5Operations
{
  public:
    H5SOperations() {}
    virtual ~H5SOperations() {
      if ( isIdValid()  ) { close(); } //Make sure the id is closed
    }
    
    herr_t open(hid_t datasetId, const std::string &notUsed)
    {
      _id = H5Dget_space(locId );
      _openErr = _id;
    }
    
    herr_t close()
    {
      _closeErr = H5Sclose(_id);
      if (_closeErr >= 0) { _id = -1; } // Reset the _id variable to invalidate it
    }
};

#endif /*IH5OPERATIONS_*/
