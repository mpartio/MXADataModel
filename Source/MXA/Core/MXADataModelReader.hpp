///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MXADATAMODELREADER_HPP_
#define MXADATAMODELREADER_HPP_


#include "MXA/Common/MXASetGetMacros.h"


template<typename TDelegate>
class MXADataModelReader
{
  public:
    MXA_SHARED_POINTERS(MXADataModelReader)
    MXA_TYPE_MACRO(MXADataModelReader)

    /**
     *
     */
    typedef typename boost::shared_ptr<TDelegate>      DelegatePointer;

    /**
     *
     * @param delegate
     * @return
     */
    static Pointer New(TDelegate delegate)
    {
      Pointer sharedPtr(new MXADataModelReader(delegate));
      return sharedPtr;
    }

    /**
     *
     * @param out
     * @return
     */
    static Pointer New(DelegatePointer out)
    {
      if (NULL == out.get() )
      {
        return MXADataModelReader::NullPointer();
      }
      Pointer sharedPtr(new MXADataModelReader(out));
      return sharedPtr;
    }

    virtual ~MXADataModelReader(){}

    /**
     * @Brief Property to force the return of a valid model only True by default
     */
    MXA_INSTANCE_PROPERTY_m(bool, ReturnValidModels)

    /**
     * @brief
     */
    DelegatePointer getDelegate() { return m_Delegate; }

    /**
     *
     * @return
     */
    IDataModel::Pointer readModel()
    {
      IDataModel::Pointer model = m_Delegate->readModel();
      std::string message;
      if (m_ReturnValidModels == true && NULL != model.get() && model->isValid(message) == false )
      {
        std::cout << logTime() << "DataModel is NOT valid: Error Message is: \n" << message << "\n      "
              << "Source File: " << __FILE__ << "(" << __LINE__ << ")" << std::endl;
        model = IDataModel::NullPointer();
      }
      return model;
    }

  protected:
    MXADataModelReader( DelegatePointer out) :
    m_ReturnValidModels(true)
    {
      m_Delegate = out;
    }

    DelegatePointer    m_Delegate;

  private:
    MXADataModelReader(const MXADataModelReader&);    // Copy Constructor Not Implemented
    void operator=(const MXADataModelReader&);        // Operator '=' Not Implemented
};


#endif /* MXADATAMODELREADER_HPP_ */
