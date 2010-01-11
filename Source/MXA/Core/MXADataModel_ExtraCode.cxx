#if 0
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::setRootNode(MXANode::SharedPtr rootNode)
{
  this->_rootNode = rootNode;
  #warning Walk this heirarchy and set all the other nodes for this class
}

//-------------------------------------------------
// 
//-------------------------------------------------
MXANode::SharedPtr MXADataModel::getRootNode()
{
  return _rootNode;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::setRequiredMetaData(string data_label, string data_value)
{
  map<string, string>& metadata = getRequiredMetaData();
  metadata[data_label] = data_value;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::clearRequiredMetaData()
{
  setRequiredMetaData("", "", "", "", "Unlimited", "", "Original", "");
}  

  


//----------------------------
// User Meta Data
void MXADataModel::_initializeUserMetaData()
{
#if 0
  DatasetData dset;
  dset.name = "User Defined";
  dset.obj_path = MXAUserMetaDataPath;

  _userMetaData->setData(dset);
#endif
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
map<string, string>& MXADataModel::getUserMetaData()
{
  #if 0
  pDatasetData dset = getDatasetData(_userMetaData);
  return dset->attributes;
#endif
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::clearUserMetaData()
{
  map<string, string>& metadata = getUserMetaData();
  metadata.clear();
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::setUserMetaData(map<string, string> &metadata)
{
  _userMetaData->data.attributes = metadata;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::setUserMetaData(string data_label, string data_value)
{
  map<string, string>& metadata = getUserMetaData();
  metadata[data_label] = data_value;
}
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
string MXADataModel::getUserMetaDataValue(string data_label)
{
  map<string, string>& metadata = getUserMetaData();
  return metadata[data_label];
}
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::removeUserMetaData(string data_label)
{
  map<string, string>& metadata = getUserMetaData();
  metadata.erase(data_label);
}






//----------------------------
// Data Dimensions
int MXADataModel::numDataDimensions()
{
  return _dataDimensions->int_data;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
pDimensionData MXADataModel::getDimensionData(MXANode *dNode)
{
  return &dNode->data;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
vector<pDimensionData> MXADataModel::getDataDimensions()
{
  vector<pDimensionData> dims;
  pDimensionData dim;
  vector<MXANode *>::const_iterator iter;
  for (iter = _dataDimensions->children.begin();
       iter != _dataDimensions->children.end();
       iter++) {
    dim = getDimensionData((*iter));
    dims.push_back(dim);
  }
  return dims;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXANode::SharedPtr MXADataModel::getDataDimension(int order)
{
  pDimensionData dim;
  vector<MXANode *>::const_iterator iter;
  for (iter = _dataDimensions->children.begin();
       iter != _dataDimensions->children.end();
       iter++) {
    dim = getDimensionData((*iter));
    if (dim->order == order) {
      return (*iter);
    }
  }
  return NULL;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXANode::SharedPtr MXADataModel::addDataDimension(string name, string altName,
          map<string, string> attributes,
          int32_t order, int32_t start_value, int32_t count,
          int32_t increment, int32_t end_value)
{
  DimensionData dim;
  dim.uniform = true;
  
  dim.name = name;
  dim.alt_name = altName;

  string dimPath = MXA::MXADataDimension::ContainerPath + "/" + StringUtils::numToString(order);
  dim.obj_path = dimPath;

  dim.order = order;
  dim.count = count;
  dim.increment = increment;
  dim.start_value = start_value;
  if (end_value == -1) {
    dim.end_value = start_value + (increment * (count - 1));
  } else {
    dim.end_value = end_value;
  }

  // Make sure the required attributes are set
  attributes[MXA_NAME_TAG] = name;
  attributes[MXA_ALT_NAME_TAG] = altName;
  attributes[MXA_ORDER_TAG] = StringUtils::numToString(order);
  attributes[MXA_START_VALUE_TAG] = StringUtils::numToString(start_value);
  attributes[MXA_COUNT_TAG] = StringUtils::numToString(count);
  attributes[MXA_INCREMENT_TAG] = StringUtils::numToString(increment);
  attributes[MXA_END_VALUE_TAG] = StringUtils::numToString(dim.end_value);
  attributes[MXA_UNIFORM_TAG] = "1";
  dim.attributes = attributes;

  MXANode::SharedPtr node (new MXANode(MXANode::Dimension, name, dim));
  // Set the int_data to the current value for the dimension
  node->setData(start_value);

  _dataDimensions->addChild(node);

  _dataDimensions->int_data++;
  return node;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXANode::SharedPtr MXADataModel::addDataDimension(string name, string altName, 
          int32_t order, int32_t start_value, 
          int32_t count, int32_t increment, 
          int32_t end_value)
{
  map<string, string> attributes;
  return addDataDimension(name, altName, attributes, order,
        start_value, count, increment, end_value);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXANode::SharedPtr MXADataModel::addDataDimension(string name, string altName)
{
  int32_t order = _dataDimensions->int_data;
  map<string, string> attributes;
  return addDataDimension(name, altName, attributes, order, 0, 1, 1, 0);
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXANode::SharedPtr MXADataModel::addDataDimension(string name, string altName, 
          map<string, string> attributes,
          int32_t order, list<int> indecies)
{
  int32_t num_indecies = indecies.size();
  
  DimensionData dim;
  dim.uniform = false;

  dim.name = name;
  dim.alt_name = altName;

  string dimPath = MXADataDimension::ContainerPath + "/" + StringUtils::numToString(order);
  dim.obj_path = dimPath;

  dim.order = order;
  dim.count = num_indecies;
  dim.increment = 0;
  dim.start_value = indecies.front();
  dim.end_value = indecies.back();
  dim.indecies = indecies;

  // Make sure the required attributes are set
  attributes[MXA_NAME_TAG] = name;
  attributes[MXA_ALT_NAME_TAG] = altName;
  attributes[MXA_ORDER_TAG] = StringUtils::numToString(order);
  attributes[MXA_START_VALUE_TAG] = StringUtils::numToString(num_indecies);
  attributes[MXA_COUNT_TAG] = StringUtils::numToString(dim.count);
  attributes[MXA_END_VALUE_TAG] = StringUtils::numToString(dim.end_value);
  attributes[MXA_INCREMENT_TAG] = "0";
  attributes[MXA_UNIFORM_TAG] = "0";
  dim.attributes = attributes;

  MXANode *node = new MXANode(MXANode::Dimension, name, dim);
  // Set the int_data to the current value for the dimension
  node->setData(dim.start_value);

  _dataDimensions->addChild(node);

  _dataDimensions->int_data++;
  return node;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXANode::SharedPtr MXADataModel::addDataDimension(string name, string altName, 
          int32_t order, std::list<int> indecies)
{
  map<string, string> attributes;
  return addDataDimension(name, altName, attributes, order, indecies);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::removeDataDimension(MXANode *node)
{
  _dataDimensions->removeChild(node);
  _dataDimensions->int_data--;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::moveDataDimension(int src_order, int32_t dst_order)
{
  if (src_order == dst_order) {
    std::cout << "Attempting to move data dimension to same spot" << std::endl;
    return;
  }

  int32_t adder;
  if (src_order > dst_order) {
    adder = -1;
  } else {
    adder = 1;
  }

  MXANode *oNode = _dataDimensions->children.at(src_order);
  for (int i=src_order; i!=dst_order; i += adder) {
    _dataDimensions->children[i] = _dataDimensions->children[i+adder];
    _dataDimensions->children[i]->data.order = i;
    _dataDimensions->children[i]->setAttributeValue("Order", StringUtils::numToString(i));
  }
  oNode->data.order = dst_order;
  oNode->setAttributeValue("Order", StringUtils::numToString(dst_order));
  _dataDimensions->children[dst_order] = oNode;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::setDimensionUniform(MXANode *node, bool uniform)
{
  node->data.uniform = uniform;
  node->setAttributeValue(MXA_UNIFORM_TAG, uniform);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::setDimensionCount(MXANode *node, int32_t new_count)
{
  node->data.count = new_count;
  node->setAttributeValue(MXA_COUNT_TAG, new_count);

  // If uniform - the end_value changes, if it's not uniform,
  //   the indicies list gets shorter and the end value changes
  if (node->data.uniform) {
    int32_t new_end =
      node->data.start_value + (node->data.increment * (node->data.count - 1));
    node->data.end_value = new_end;
    node->setAttributeValue(MXA_END_VALUE_TAG, new_end);
  } else {
    // update the indecies and end_value;
  }    
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::setDimensionIncrement(MXANode *node, int32_t new_increment)
{
  node->data.increment = new_increment;
  node->setAttributeValue(MXA_INCREMENT_TAG, new_increment);

  // The end value changes if the increment changes (if uniform)
  if (node->data.uniform) {
    int32_t new_end =
      node->data.start_value + (node->data.increment * (node->data.count - 1));
    node->data.end_value = new_end;
    node->setAttributeValue(MXA_END_VALUE_TAG, new_end);
  } 
  // This shouldn't change if it isn't uniform 
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::setDimensionStartValue(MXANode *node, int32_t new_start)
{
  int32_t old_start = node->data.start_value;
  node->data.start_value = new_start;
  node->setAttributeValue(MXA_START_VALUE_TAG, new_start);

  // The count has also changed if the start value changes
  int32_t new_count;
  if (node->data.uniform) {
    new_count = node->data.count + (new_start - old_start);
  } else {
    // if not uniform - just one more has been added
    new_count = node->data.count + 1;
  }
  node->data.count = new_count;
  node->setAttributeValue(MXA_COUNT_TAG, new_count);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int MXADataModel::numDataRecords()
{
  return _dataRecords->int_data;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
pRecordData MXADataModel::getRecordData(MXANode *node)
{
  return &node->data;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXANode::SharedPtr MXADataModel::getDataRecordsRoot()
{
  return _dataRecords;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool _recguidCompare(const void *guid, const void *node)
{
  const MXANode *rnode = static_cast<const MXANode *>(node);
  const RecordData rec = rnode->data;
  const int32_t *tst_guid = static_cast<const int32_t *>(guid); 
  return (rec.guid == *tst_guid);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool _recluidCompare(const void *luid, const void *node)
{
  const MXANode *rnode = static_cast<const MXANode *>(node);
  const RecordData rec = rnode->data;
  const int32_t *tst_luid = static_cast<const int32_t *>(luid); 
  return (rec.luid == *tst_luid);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXANode::SharedPtr MXADataModel::getDataRecord(int guid)
{  
  MXANode * node;
  node = _dataRecords->findSubNodeIf(&_recguidCompare, (const void *)&guid);
  return node;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXANode::SharedPtr MXADataModel::getDataRecordChild(MXANode *parent, int32_t luid)
{
  MXANode *node;
  node = parent->findChildIf(&_recluidCompare, (const void *)&luid);
  return node;
} 

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string MXADataModel::getDataRecordParentPath(MXANode *node)
{
  std::string path("");
  if (node->type != MXANode::Record) {
    return path;
  }

  MXANode *parent = node->parent;
  if (parent->type == MXANode::DataRecords) {
    return path;
  }

  return getDataRecordPath(parent);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string MXADataModel::getDataRecordPath(MXANode *node)
{
  std::string path("");
  if (node->type != MXANode::Record) {
    return path;
  }

  MXANode *parent = node->parent;
  if (parent->type != MXANode::DataRecords) {
    path = getDataRecordPath(parent);
  }

  pRecordData rec = getRecordData(node);
  if (parent->type != MXANode::DataRecords) {
    path += "/";
  }
  path += StringUtils::numToString(rec->luid);
  return path;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string MXADataModel::getDataRecordPath(int recGuid)
{
  MXANode *recNode = getDataRecord(recGuid);
  return getDataRecordPath(recNode);
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXANode * MXADataModel::addDataRecord(string name, string altName, 
             string objPath, 
             map<string, string> attributes,
             int32_t luid, int32_t guid, MXANode *parent)
{
  int32_t num_records = _dataRecords->int_data;
  RecordData rec;

  rec.name = name;
  rec.alt_name = altName;
  rec.obj_path = objPath;

  rec.luid = luid;
  if (guid == -1) {
    rec.guid = num_records;
  } else {
    rec.guid = guid;
  }

  // Make sure the required attributes are set
  attributes[MXA_NAME_TAG] = name;
  attributes[MXA_ALT_NAME_TAG] = altName;
  attributes[MXA_LUID_TAG] = StringUtils::numToString(rec.luid);
  attributes[MXA_GUID_TAG] = StringUtils::numToString(rec.guid);
  rec.attributes = attributes;

  MXANode *node = new MXANode(MXANode::Record, name, rec);
  if (parent == NULL) {
    _dataRecords->addChild(node);
  } else {
    parent->addChild(node);
  }

  _dataRecords->int_data++;

  return node;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXANode * MXADataModel::addDataRecord(string name, string altName, 
             string objPath, 
             int32_t luid, int32_t guid, MXANode *parent)
{
  map<string, string> attributes;
  return addDataRecord(name, altName, objPath, attributes,
           luid, guid, parent);
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXANode * MXADataModel::addDataRecord(string name, string altName, 
             MXANode *parent)
{
  MXANode *nodeParent;
  if (parent == NULL) {
    nodeParent = getDataRecordsRoot();
  } else {
    nodeParent = parent;
  }

  int32_t guid = _dataRecords->int_data;
  int32_t luid = nodeParent->numChildren();
  string recPath = getDataRecordPath(nodeParent);
  recPath += "/" + luid;

  map<string, string> attributes;
  return addDataRecord(name, altName, recPath, attributes,
           luid, guid, nodeParent);
}


void MXADataModel::removeDataRecord(MXANode *node)
{
  MXANode *parent = node->parent;
  parent->removeChild(node);
  parent->int_data--;
}



// ------------------------------------------------------------
//  Data Record Sources

void MXADataModel::clearDataSources()
{
  _num_sources = 0;
  _dataSources.clear();
}

std::string MXADataModel::dimensionOrderString(DataSource source)
{
  std::string order_str("");
  for (std::map<int, int>::iterator iter = source.dimensions.begin(); iter != source.dimensions.end(); ++iter ) {
    order_str += StringUtils::numToString( (*iter).second ) + "/";
  }
  //Remove the Trailing Slash
  order_str.erase(order_str.size() - 1);
  #if DEBUG
  std::cout << "order_str:" << order_str << std::endl;
  #endif
  return order_str;
}

std::string MXADataModel::generateDataSourcePath(DataSource source)
{

  // does NOT have a terminating /
  std::string sourcePath;
  sourcePath = getDataRoot() + dimensionOrderString(source);

  if (source.record_path != "") {
    sourcePath += "/" + source.record_path;
  }
  #if DEBUG
  std::cout << " Final Source Path: " << sourcePath << std::endl;
  #endif
  return sourcePath;
}


pDataSource MXADataModel::findDataSource(std::map<int, int> dimMap,
          std::string recPath, int32_t luid)
{
  std::vector<DataSource>::iterator iter;
  for (iter=_dataSources.begin(); iter!=_dataSources.end(); iter++) {
    if (recPath == iter->record_path &&
  dimMap == iter->dimensions &&
  luid == iter->record_id) {
      return &(*iter);
    }
  }
  return NULL;
}


std::map<int, int> MXADataModel::getDimensionOrderMap()
{
  std::map<int, int> dimensionMap;
  
  std::vector<MXANode *>::const_iterator iter;
  for(iter = _dataDimensions->children.begin();
      iter != _dataDimensions->children.end(); iter++) {
    dimensionMap[(*iter)->data.order] = (*iter)->int_data;
  }

  return dimensionMap;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::addDataSource(int recordGuid, std::string &sourcePath)
{
  MXANode *rNode = getDataRecord(recordGuid);
  addDataSource(rNode, sourcePath);  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::addDataSource(MXANode *rNode, std::string &sourcePath)
{
  std::map<int, int> dimMap = getDimensionOrderMap();
  std::string recPath = getDataRecordParentPath(rNode);
  int32_t luid = rNode->data.luid;

  addDataSource(dimMap, recPath, luid, sourcePath, NULL);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::addDataSource(std::map<int, int> dimensionValues,
        std::string recordPath, int32_t luid,
        std::string sourcePath, AbstractDataParserDelegate* delegate)
{
  pDataSource source = findDataSource(dimensionValues, recordPath, luid);
  if (source == NULL) {
   // std::cout << "MXAataModel::addDataSource source was NULL" << std::endl;
    DataSource newSource;
    newSource.dimensions = dimensionValues;
    newSource.record_path = recordPath;
    newSource.record_id = luid;
    newSource.source_path = sourcePath;
    newSource.parseDelegate = delegate;
    _dataSources.push_back(newSource);  //Pushing a copy onto the vector
    _num_sources++;
  } else { // Just update the existing data source
    //std::cout << "MXAataModel::addDataSource source was NOT NULL" << std::endl;
    source->source_path = sourcePath;
  }
}


void MXADataModel::removeDataSource(DataSource source)
{
  std::vector<DataSource>::iterator iter;
  for (iter=_dataSources.begin(); iter!=_dataSources.end(); iter++) {
    if ((*iter) == source) {
      _dataSources.erase(iter);
      return;
    }
  }
}


// Data Path Strings

std::string MXADataModel::generateDimensionLabelsPath()
{
  std::list<std::string> labels;
  std::vector<MXANode *>::const_iterator iter;
  for(iter = _dataDimensions->children.begin();
      iter != _dataDimensions->children.end(); iter++) {
    labels.push_back((*iter)->name);
  }
  std::string path_str("");
  if (labels.size() > 0) {
    path_str = StringUtils::joinList("/", labels);
  }
  return path_str;
}


std::string MXADataModel::generateDimensionValuesPath()
{
  std::list<int> dimensionValues;
  
  std::vector<MXANode *>::const_iterator iter;
  for(iter = _dataDimensions->children.begin();
      iter != _dataDimensions->children.end(); iter++) {
    dimensionValues.push_back((*iter)->int_data);
  }  
  
  std::string path_str("");
  if (dimensionValues.size() > 0) {
    path_str = StringUtils::joinList("/", dimensionValues);
  }
  return path_str;
}


std::string MXADataModel::generateFullRecordPath(MXANode *rNode)
{
  // returns the entire string from data root and dimensions 
  // down to this record luid
  std::string rPath = getDataRecordPath(rNode);
  if (rPath == "") {
    return rPath;
  }

  std::string recordPath("");
  recordPath = getDataRoot() + generateDimensionValuesPath() + "/" + rPath;
    #if DEBUG
    std::cout << "Record Path: " << recordPath << std::endl;
    #endif
  return recordPath;
}
#endif
