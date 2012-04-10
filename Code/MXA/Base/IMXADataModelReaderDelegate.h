/*
 * IMXADataModelReaderDelegate.h
 *
 *  Created on: Dec 2, 2009
 *      Author: mjackson
 */

#ifndef IMXADATAMODELREADERDELEGATE_H_
#define IMXADATAMODELREADERDELEGATE_H_

#include "MXA/MXA.h"
#include "MXA/Common/MXASetGetMacros.h"
#include "MXA/Base/IDataModel.h"


class MXA_EXPORT IMXADataModelReaderDelegate
{

  public:
    MXA_SHARED_POINTERS(IMXADataModelReaderDelegate)
    MXA_TYPE_MACRO(IMXADataModelReaderDelegate)

    virtual ~IMXADataModelReaderDelegate() {};

    virtual IDataModel::Pointer readModel() = 0;

  protected:
    IMXADataModelReaderDelegate() {};

  private:
    IMXADataModelReaderDelegate(const IMXADataModelReaderDelegate&);    // Copy Constructor Not Implemented
    void operator=(const IMXADataModelReaderDelegate&);  // Operator '=' Not Implemented
};



#endif /* IMXADATAMODELREADERDELEGATE_H_ */
