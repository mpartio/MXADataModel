#ifndef _IDATASOURCE_H_
#define _IDATASOURCE_H_

#include <MXA/Common/DLLExport.h>
#include <MXA/Common/MXATypeDefs.h>


/**
 * @brief Interface that defines the methods of a "Data Source" Object
 * @author Mike Jackson
 * @date April 2007
 * @version $Revision: 1.2 $
 */
class MXA_EXPORT IDataSource
{

public:
  IDataSource() {}
  virtual ~IDataSource() {}
  
  /**
    * @brief Generates the internal path that is used by the underlying data storage
    * mechanism to place the data into the hierarchy
    * @return std::string
    */
   virtual std::string generateInternalPath ( ) = 0;

   /**
    * @brief Generates the internal path that is used by the underlying data storage
    * mechanism to place the data into the hierarchy
    * @param indices A new set of indices to use to generate the paths
    * @return The generated internal path. Empty if there was an error.
    */
   virtual std::string generateInternalPath ( std::vector<int32> &indices) = 0;
   
   /**
    * Set the value of m_dataRecord
    * @param dataRecord the new value of m_dataRecord
    */
   virtual void setDataRecord ( IDataRecordPtr dataRecord ) = 0;

   /**
    * Get the value of m_dataRecord
    * @return the value of m_dataRecord
    */
   virtual IDataRecordPtr getDataRecord ( ) = 0;


   /**
    * Set the value of m_sourcePath
    * @param sourcePath the new value of m_sourcePath
    */
   virtual void setSourcePath ( std::string sourcePath ) = 0;

   /**
    * Get the value of m_sourcePath
    * @return the value of m_sourcePath
    */
   virtual std::string getSourcePath ( ) = 0;


   /**
    * Set the value of m_importDelegate
    * @param dataParser the new value of m_importDelegate
    */
   virtual void setImportDelegate ( IImportDelegatePtr dataParser ) = 0;

   /**
    * Get the value of m_importDelegate
    * @return the value of m_importDelegate
    */
   virtual IImportDelegatePtr getImportDelegate ( ) = 0;


   /**
    * Set the value of m_dataModel
    * @param dataModel the new value of m_dataModel
    */
   virtual void setDataModel ( IDataModelPtr dataModel ) = 0;

   /**
    * Get the value of m_dataModel
    * @return the value of m_dataModel
    */
   virtual IDataModelPtr getDataModel ( ) = 0;


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

