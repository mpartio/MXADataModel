#ifndef _IDATASOURCE_H_
#define _IDATASOURCE_H_

#include "MXA/MXADLLExport.h"
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Base/IDataModel.h>
#include <MXA/Base/IImportDelegate.h>

#include <vector>

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

    MXA_SHARED_POINTERS(IDataSource)
    typedef std::vector<Pointer> Collection;
    
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
   virtual std::string generateInternalPath ( std::vector<int32_t> &indices) = 0;

   /**
    * Set the value of m_dataRecord
    * @param dataRecord the new value of m_dataRecord
    */
   virtual void setDataRecord ( IDataRecord::Pointer dataRecord ) = 0;

   /**
    * Get the value of m_dataRecord
    * @return the value of m_dataRecord
    */
   virtual IDataRecord::Pointer getDataRecord ( ) = 0;


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
    * @brief Does the datasource exist on the filesystem
    * @return true if it does exist.
    */
   virtual bool dataSourceExists() = 0;

   /**
    * Set the value of m_importDelegate
    * @param dataParser the new value of m_importDelegate
    */
   virtual void setImportDelegate ( IImportDelegate::Pointer dataParser ) = 0;

   /**
    * Get the value of m_importDelegate
    * @return the value of m_importDelegate
    */
   virtual IImportDelegate::Pointer getImportDelegate ( ) = 0;


   /**
    * Set the value of m_dataModel
    * @param dataModel the new value of m_dataModel
    */
   virtual void setDataModel ( IDataModel::Pointer dataModel ) = 0;

   /**
    * Get the value of m_dataModel
    * @return the value of m_dataModel
    */
   virtual IDataModel::Pointer getDataModel ( ) = 0;


   /**
    * Set the value of m_dimensionValues
    * @param new_var the new value of m_dimensionValues
    */
   virtual void setDimensionValues ( std::vector<int32_t> new_var ) = 0;

   /**
    * Get the value of m_dimensionValues
    * @return the value of m_dimensionValues
    */
   virtual std::vector<int32_t> getDimensionValues ( ) = 0;

private:

};



#endif /* _IDATASOURCE_H_ */

