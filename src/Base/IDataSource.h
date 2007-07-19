#ifndef _IDATASOURCE_H_
#define _IDATASOURCE_H_

#include "Common/DLLExport.h"
#include "Common/MXATypeDefs.h"


/**
 * @brief Interface that defines the methods of a "Data Source" Object
 * @author Mike Jackson
 * @date April 2007
 * @version 1.0
 */
class MXA_EXPORT IDataSource
{

public:
  IDataSource() {}
  virtual ~IDataSource() {}
  
  /**
    * Generates the internal path that is used by the underlying data storage
    * mechanism to place the data into the hierarchy
    * @return std::string
    */
   virtual std::string generateInternalPath ( ) = 0;

   /**
    * Set the value of m_dataRecord
    * @param new_var the new value of m_dataRecord
    */
   virtual void setDataRecord ( MXADataRecordPtr dataRecord ) = 0;

   /**
    * Get the value of m_dataRecord
    * @return the value of m_dataRecord
    */
   virtual MXADataRecordPtr getDataRecord ( ) = 0;


   /**
    * Set the value of m_sourcePath
    * @param new_var the new value of m_sourcePath
    */
   virtual void setSourcePath ( std::string sourcePath ) = 0;

   /**
    * Get the value of m_sourcePath
    * @return the value of m_sourcePath
    */
   virtual std::string getSourcePath ( ) = 0;


   /**
    * Set the value of m_importDelegate
    * @param new_var the new value of m_importDelegate
    */
   virtual void setImportDelegate ( IDataImportDelegatePtr dataParser ) = 0;

   /**
    * Get the value of m_importDelegate
    * @return the value of m_importDelegate
    */
   virtual IDataImportDelegatePtr getImportDelegate ( ) = 0;


   /**
    * Set the value of m_dataModel
    * @param new_var the new value of m_dataModel
    */
   virtual void setDataModel ( MXADataModelPtr dataModel ) = 0;

   /**
    * Get the value of m_dataModel
    * @return the value of m_dataModel
    */
   virtual MXADataModelPtr getDataModel ( ) = 0;


   /**
    * Set the value of m_dimensionValues
    * @param new_var the new value of m_dimensionValues
    */
   virtual void setDimensionValues ( std::vector<int32> new_var ) = 0;

   /**
    * Get the value of m_dimensionValues
    * @return the value of m_dimensionValues
    */
   virtual std::vector<int32> getDimensionValues ( ) = 0;

private:
  
};



#endif /* _IDATASOURCE_H_ */

