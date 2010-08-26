/*
 * IMXADataModelWriterDelegate.h
 *
 *  Created on: Dec 2, 2009
 *      Author: mjackson
 */

#ifndef IMXADATAMODELWRITERDELEGATE_H_
#define IMXADATAMODELWRITERDELEGATE_H_

#include "MXA/MXATypes.h"
#include "MXA/Common/MXASetGetMacros.h"
#include "MXA/Base/IDataModel.h"


class MXA_EXPORT IMXADataModelWriterDelegate
{

  public:
    MXA_SHARED_POINTERS(IMXADataModelWriterDelegate)
    MXA_TYPE_MACRO(IMXADataModelWriterDelegate)

    virtual ~IMXADataModelWriterDelegate() {};

    virtual int32_t writeModel(IDataModel::Pointer model) = 0;

  protected:
    IMXADataModelWriterDelegate() {};

  private:
    IMXADataModelWriterDelegate(const IMXADataModelWriterDelegate&);    // Copy Constructor Not Implemented
    void operator=(const IMXADataModelWriterDelegate&);  // Operator '=' Not Implemented
};


#endif /* IMXADATAMODELWRITERDELEGATE_H_ */
