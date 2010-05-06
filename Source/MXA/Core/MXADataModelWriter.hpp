///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, 2010 Michael A. Jackson for BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MXADATAMODELWRITER_H_
#define MXADATAMODELWRITER_H_

#include "MXA/Common/MXASetGetMacros.h"


template<typename TDelegate>
class MXADataModelWriter
{
  public:
    MXA_SHARED_POINTERS(MXADataModelWriter)
    MXA_TYPE_MACRO(MXADataModelWriter)

    typedef typename boost::shared_ptr<TDelegate>      DelegatePointer;

    static Pointer New(TDelegate delegate)
    {
      Pointer sharedPtr(new MXADataModelWriter(delegate));
      return sharedPtr;
    }

    static Pointer New(DelegatePointer out)
    {
      if (NULL == out.get() )
      {
        return MXADataModelWriter::NullPointer();
      }
      Pointer sharedPtr(new MXADataModelWriter(out));
      return sharedPtr;
    }

    virtual ~MXADataModelWriter(){}

    DelegatePointer getDelegate() { return m_Delegate; }

    int32_t writeModel(IDataModel::Pointer model)
    {
      m_Delegate->writeModel(model);
      return 1;
    }

  protected:
    MXADataModelWriter( DelegatePointer out) {
      m_Delegate = out;
    }

    DelegatePointer    m_Delegate;

  private:
    MXADataModelWriter(const MXADataModelWriter&);    // Copy Constructor Not Implemented
    void operator=(const MXADataModelWriter&);        // Operator '=' Not Implemented
};

#endif /* MXADATAMODELWRITER_H_ */
