/* ============================================================================
 * Copyright (c) 2010, Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2010, Dr. Michael A. Groeber (US Air Force Research Laboratories)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Groeber, Michael A. Jackson, the US Air Force,
 * BlueQuartz Software nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  This code was written under United States Air Force Contract number
 *                           FA8650-07-D-5800
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef _DATACONTAINERMACROS_H_
#define _DATACONTAINERMACROS_H_


#define TEST_PREREQ_DATA( dc, NameSpace, DType, Name, errVariable, errCode, ptrType, ArrayType, size, NumComp)\
  {if (m_##Name##ArrayName.isEmpty() == true){ \
    setErrorCondition(errCode##000);\
    QString _##Name##_ss;\
    _##Name##_ss << "The name of the array for the " << #NameSpace << #DType << #Name << " was empty. Please provide a name for this array" ;\
    addErrorMessage(getHumanLabel(), _##Name##_ss, errCode##000);\
  }\
  m_##Name = dc->get##DType##SizeCheck<ptrType, ArrayType, AbstractFilter>(m_##Name##ArrayName, size, NumComp, NULL);\
  if (NULL == m_##Name ) {\
    errVariable = errCode;\
  }}



/**
 * @brief These are used in the filters to run checks on available arrays
 * @param dc The Data Container variable
 * @param NameSpace
 * @param DType The type of data: CellData, FieldData....
 * @param Name The name of the variable or constant
 * @param ss A String Stream object
 * @param err The default error code to be returned if something goes wrong
 * @param ptrType The primitive type of the data
 * @param ArrayType The Type of DataArray<T> that is used
 * @param size The number of tuples in the array
 * @param NumComp The number of components of the DataArray
 */
#define GET_PREREQ_DATA( dc, NameSpace, DType, Name, err, ptrType, ArrayType, size, NumComp)\
  { QString ss; \
  if (m_##Name##ArrayName.isEmpty() == true){ \
    setErrorCondition(err##000);\
    ss = QObject::tr("The name of the array for the %1::%2::%3 was empty. Please provide a name for this array").arg(#NameSpace).arg(#DType).arg(#Name);\
    addErrorMessage(getHumanLabel(), ss, getErrorCondition());\
  }\
  if (dc->does##DType##Exist(m_##Name##ArrayName) == false) {\
    setErrorCondition(err##001);\
    ss = QObject::tr("An array with name '%1' in the %2 grouping does not exist and is required for this filter to execute.").arg(m_##Name##ArrayName).arg(#DType);\
    addErrorMessage(getHumanLabel(), ss, getErrorCondition());\
  }\
  else { \
  /* QString _s(#Name); \
  addRequired##DType(_s);*/\
  m_##Name = dc->get##DType##SizeCheck<ptrType, ArrayType, AbstractFilter>(m_##Name##ArrayName, size, NumComp, this);\
  if (NULL == m_##Name ) {\
    ss = QObject::tr("\nThe current array with name '%1' is not valid for the internal array named 'm_%2' for this filter."\
    "The preflight failed for one or more reasons. Check additional error messages for more details.").arg(m_##Name##ArrayName).arg(#Name);\
    setErrorCondition(err##002);\
    addErrorMessage(getHumanLabel(), ss, getErrorCondition()); \
  }}}

/**
 * @brief These are used in the filters to run checks on available arrays
 * @param dc The Data Container variable
 * @param DType The type of data: CellData, FieldData....
 * @param Name The name of the variable or constant
 * @param ss A String Stream object
 * @param err The default error code to be returned if something goes wrong
 * @param ptrType The primitive type of the data
 * @param ArrayType The Type of DataArray<T> that is used
 * @param size The number of tuples in the array
 * @param NumComp The number of components of the DataArray
 */
#define GET_PREREQ_DATA_2( dc, DType, Name, err, ptrType, ArrayType, size, NumComp)\
  {QString ss;\
  if (m_##Name##ArrayName.isEmpty() == true){ \
    setErrorCondition(err##000);\
    ss = QObject::tr("The name of the array for the %1 was empty. Please provide a name for this array").arg(#Name);\
    addErrorMessage(getHumanLabel(), ss, getErrorCondition());\
  }\
  if (dc->does##DType##Exist(m_##Name##ArrayName) == false) {\
    setErrorCondition(err##001);\
    ss = QObject::tr("An array with name '%1' in the %2 grouping does not exist and is required for this filter to execute.").arg(m_##Name##ArrayName).arg(#DType);\
    addErrorMessage(getHumanLabel(), ss, getErrorCondition());\
  }\
  else { \
  /* QString _s(#Name); \
  addRequired##DType(_s);*/\
  m_##Name = dc->get##DType##SizeCheck<ptrType, ArrayType, AbstractFilter>(m_##Name##ArrayName, size, NumComp, this);\
  if (NULL == m_##Name ) {\
    ss = QObject::tr("\nThe current array with name '%1' is not valid for the internal array named '%2::%3' for this filter."\
     "The preflight failed for one or more reasons. Check additional error messages for more details.").arg(m_##Name##ArrayName).arg(#DType).arg(#Name);\
    setErrorCondition(err##002);\
    addErrorMessage(getHumanLabel(), ss, getErrorCondition()); \
  }}}


#define CREATE_NON_PREREQ_DATA(dc, NameSpace, DType, Name, ptrType, ArrayType, initValue, size, NumComp)\
  {\
  if (m_##Name##ArrayName.isEmpty() == true)\
  {\
    setErrorCondition(-10000);\
    QString ss = QObject::tr("The name of the array for the "  #NameSpace  #DType  #Name " was empty. Please provide a name for this array.");\
    addErrorMessage(getHumanLabel(), ss, -10000);\
  }\
  /* QString _s(#Name);*/\
  m_##Name = dc->get##DType##SizeCheck<ptrType, ArrayType, AbstractFilter>(m_##Name##ArrayName, size, NumComp, NULL);\
  if (NULL ==  m_##Name ) \
  {\
    ArrayType::Pointer p = ArrayType::CreateArray((size * NumComp), m_##Name##ArrayName);\
    if (NULL == p.get()) \
    {\
      QString ss;\
      if (m_##Name##ArrayName.isEmpty() == true) \
      {\
        ss = QObject::tr("Filter %1 attempted to create array with an empty name and that is not allowed.").arg(getNameOfClass());\
        setErrorCondition(-501);\
      } else {\
         ss = QObject::tr("Filter %1 attempted to create array' %2' but was unsuccessful. This is most likely due to not enough contiguous memory.").arg(getNameOfClass()).arg(m_##Name##ArrayName);\
        setErrorCondition(-500);\
      }\
      addErrorMessage(getHumanLabel(), ss, -50001);\
    } \
    else if (p->getPointer(0) == NULL)\
    {\
      setErrorCondition(-11000);\
      QString ss = QObject::tr("'%1' was sized to Zero which may cause the program to crash in filters that use this array, including the current filter."\
        " Please add a filter before this filter that will result in a properly sized array.").arg(m_##Name##ArrayName);\
      addErrorMessage(getHumanLabel(), ss, getErrorCondition());\
    }\
    else {\
      p->initializeWithValues(initValue);\
      p->SetNumberOfComponents(NumComp);\
      p->SetName(m_##Name##ArrayName);\
      dc->add##DType(m_##Name##ArrayName, p);\
      m_##Name = p->getPointer(0);\
      CreatedArrayHelpIndexEntry::Pointer e = CreatedArrayHelpIndexEntry::New();\
      e->setFilterName(this->getNameOfClass());\
      e->setFilterHumanLabel(this->getHumanLabel());\
      e->setFilterGroup(this->getGroupName());\
      e->setFilterSubGroup(this->getSubGroupName());\
      e->setArrayDefaultName(m_##Name##ArrayName);\
      e->setArrayGroup(#DType);\
      e->setArrayNumComponents(NumComp);\
      e->setArrayType(#ArrayType);\
      addCreatedArrayHelpIndexEntry(e);\
    }\
  }\
}


/**
 *@brief
 */
#define ADD_HELP_INDEX_ENTRY(DType, Name, ArrayType, NumComp)\
CreatedArrayHelpIndexEntry::Pointer e = CreatedArrayHelpIndexEntry::New();\
e->setFilterName(this->getNameOfClass());\
e->setFilterHumanLabel(this->getHumanLabel());\
e->setFilterGroup(this->getGroupName());\
e->setFilterSubGroup(this->getSubGroupName());\
e->setArrayDefaultName(m_##Name##ArrayName);\
e->setArrayGroup(#DType);\
e->setArrayNumComponents(NumComp);\
e->setArrayType(#ArrayType);\
addCreatedArrayHelpIndexEntry(e);



#define METHOD_DEF_TEMPLATE_GETARRAYDATA(GetMethod)\
template<typename PtrType, typename DataArrayType, typename AbstractFilter>\
PtrType* GetMethod##SizeCheck(const QString &arrayName, size_t size, int numComp, AbstractFilter* obv)\
{\
PtrType* gi = NULL;\
IDataArray::Pointer iDataArray = GetMethod(arrayName);\
if (iDataArray.get() == 0) {\
  return gi;\
}\
if (size*numComp != iDataArray->GetSize()) {\
  QString s = QObject::tr(" - Array '%1' from the DataContainer class did not have the required number of elements. Required: %2 Contains: %3").arg(arrayName).arg((size*numComp)).arg(iDataArray->GetSize());\
  if (NULL != obv) {obv->setErrorCondition(-501);\
  obv->addErrorMessage(obv->getHumanLabel(), s, -501);}\
  return gi;\
}\
if (numComp != iDataArray->GetNumberOfComponents()) {\
      if (NULL != obv) {\
      QString ss = QObject::tr("Filter '%1'' requires an array where the number of components is %2 but the array"\
      " that was supplied has %3.").arg(obv->getHumanLabel()).arg(numComp).arg(iDataArray->GetNumberOfComponents());\
      obv->addErrorMessage(obv->getHumanLabel(), ss, -503);}\
      return gi;\
}\
  gi = IDataArray::SafeReinterpretCast<IDataArray*, DataArrayType*, PtrType* >(iDataArray.get());\
  if (NULL == gi) {\
    typename DataArrayType::Pointer dat = DataArrayType::CreateArray(1, "JUNK-INTERNAL-USE-ONLY");\
    QString s = QObject::tr(" - The filter requested an array named '%1' with type '%2' from the %3.\n"\
      "An Array with name '%4' is stored in the %5 but is of type %6\n")\
    .arg(arrayName).arg(dat->getTypeAsString()).arg(getNameOfClass()).arg(arrayName).arg(getNameOfClass()).arg(iDataArray->getTypeAsString());\
    if (NULL != obv) {obv->setErrorCondition(-502);\
    obv->addErrorMessage(obv->getHumanLabel(), s, -502);}\
    return gi;\
  }\
return gi;\
}



#define METHOD_DEF_TEMPLATE_INITIALIZEARRAYDATA(Field)\
template<typename PtrType, typename DataArrayType, typename AbstractFilter>\
PtrType* create##Field##Data(const QString &arrayName, size_t size, int numComp, AbstractFilter* obv)\
{\
  PtrType* valuePtr = NULL;\
  IDataArray::Pointer iDataArray = get##Field##Data(arrayName);\
  if (iDataArray.get() == NULL) { \
    iDataArray = DataArrayType::CreateArray(size * numComp, arrayName);\
    iDataArray->initializeWithZeros();\
    iDataArray->SetNumberOfComponents(numComp);\
    if (NULL == iDataArray.get()) { \
      QString s = QObject::tr(": Array '%1' could not allocate %2 elements.").arg(arrayName).arg(size);\
      if (NULL != obv) {obv->setErrorCondition(-25);\
      obv->addErrorMessage(getNameOfClass(), s, -25);}\
      return valuePtr;\
    }\
    add##Field##Data(arrayName, iDataArray);\
  } \
  valuePtr =\
  IDataArray::SafeReinterpretCast<IDataArray*, DataArrayType*, PtrType* >(iDataArray.get());\
  if (NULL == valuePtr) {\
      QString s = QObject::tr(": Array '%1' could not be cast to proper type").arg(arrayName);\
    if (NULL != obv) {obv->setErrorCondition(-12);\
    obv->addErrorMessage(getNameOfClass(), s, -12);}\
    return valuePtr;\
  }\
  return valuePtr;\
}


#define GET_NAMED_ARRAY_SIZE_CHK_RETVALUE(dataContainer, field, name, typeClass, m_msgType, size, valuePtr) \
m_msgType* valuePtr = NULL;\
{\
  IDataArray::Pointer iDataArray = dataContainer->get##field##Data(name);\
  if (iDataArray.get() == NULL) { \
    QString s = QObject::tr(": Array %1 from the DataContainer class was not in the DataContainer").arg(name);\
    setErrorCondition(-10);\
    addErrorMessage(getNameOfClass(), s, -10);\
    return -10;\
  } \
  if (static_cast<size_t>(size) != iDataArray->GetSize()) {\
    QString s = QObject::tr(": Array %1 from the DataContainer class did not have the correct number of elements.").arg(name);\
    setErrorCondition(-20);\
    addErrorMessage(getNameOfClass(), s, -20);\
    return -20;\
  }\
  valuePtr =\
  IDataArray::SafeReinterpretCast<IDataArray*, typeClass*, m_msgType* >(dataContainer->get##field##Data(name).get());\
  if (NULL == valuePtr) {\
    QString s = QObject::tr(": Array %1 from the DataContainer class could not be cast to type ").arg(#m_msgType);\
    setErrorCondition(-30);\
    addErrorMessage(getNameOfClass(), s, -30);\
    return -30;\
  }\
}

#define GET_NAMED_ARRAY_SIZE_CHK_NOMSG(dataContainer, field, name, typeClass, m_msgType, size, valuePtr) \
m_msgType* valuePtr = NULL;\
{\
  IDataArray::Pointer iDataArray = dataContainer->get##field##Data(name);\
  if (iDataArray.get() == NULL) { \
    return;\
  } \
  if (static_cast<size_t>(size) != iDataArray->GetSize()) {\
    return;\
  }\
  valuePtr =\
  IDataArray::SafeReinterpretCast<IDataArray*, typeClass*, m_msgType* >(dataContainer->get##field##Data(name).get());\
  if (NULL == valuePtr) {\
    return;\
  }\
}

#define GET_NAMED_ARRAY_SIZE_CHK_NOMSG_RET(dataContainer, field, name, typeClass, m_msgType, size, valuePtr) \
m_msgType* valuePtr = NULL;\
{\
  IDataArray::Pointer iDataArray = dataContainer->get##field##Data(name);\
  if (iDataArray.get() == NULL) { \
    return -10;\
  } \
  if (static_cast<size_t>(size) != iDataArray->GetSize()) {\
    qDebug() << name << " Size did not match." << size << " vs " << iDataArray->getNumberOfTuples() ;\
    return -20;\
  }\
  valuePtr =\
  IDataArray::SafeReinterpretCast<IDataArray*, typeClass*, m_msgType* >(dataContainer->get##field##Data(name).get());\
  if (NULL == valuePtr) {\
    return -30;\
  }\
}



#define DOES_DATASET_EXIST_DECL(DType)\
virtual bool does##DType##Exist(const QString &name);

#define DOES_DATASET_EXIST_DEFN(Class, DType)\
bool Class::does##DType##Exist(const QString &name) {\
  /*QMap<QString, IDataArray::Pointer>::iterator iter = m_##DType.find(name);*/\
  return  m_##DType.contains(name);\
}

#endif /* _DATACONTAINERMACROS_H_ */


/*
   // Cell Data

  GET_PREREQ_DATA(m, DREAM3D, CellData, GrainIds, -300, int32_t, Int32ArrayType, voxels, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellData, AlreadyChecked, -300, bool, BoolArrayType, voxels, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellData, GoodVoxels, -300, bool, BoolArrayType, voxels, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellData, Quats, -300, float, FloatArrayType, voxels, 5)
  GET_PREREQ_DATA(m, DREAM3D, CellData, SurfaceVoxels, -301, int8_t, Int8ArrayType, voxels)
  GET_PREREQ_DATA(m, DREAM3D, CellData, KernelAverageMisorientations, -300, float, FloatArrayType, voxels, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellData, GrainMisorientations, -300, float, FloatArrayType, voxels, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellData, MisorientationGradients, -300, float, FloatArrayType, voxels, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellData, NearestNeighbors, -300, int32_t, Int32ArrayType, voxels, 3)
  GET_PREREQ_DATA(m, DREAM3D, CellData, NearestNeighborDistances, -300, float, FloatArrayType, voxels, 3)
  GET_PREREQ_DATA(m, DREAM3D, CellData, Neighbors, -300, int32_t, Int32ArrayType, voxels, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellData, Phases, C, -300, int32_t, Int32ArrayType,  voxels, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellData, CellEulerAngles, -300, float, FloatArrayType,  voxels, 3)


  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellData, GrainIds, int32_t, Int32ArrayType, voxels, 1)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellData, Quats, float, FloatArrayType, fields, 5)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellData, GrainIds, int32_t, Int32ArrayType, voxels, 1)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellData, SurfaceVoxels, int8_t, Int8ArrayType, voxels, 1)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellData, Neighbors, int32_t, Int32ArrayType, voxels, 1)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellData, AlreadyChecked, bool, BoolArrayType, voxels, 1)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellData, Phases, C, int32_t, Int32ArrayType, voxels, 1)
  CREATE_NON_PREREQ_DATA( m, DREAM3D, CellData, CellEulerAngles, float, FloatArrayType, voxels, 3)


  // Field Data

  GET_PREREQ_DATA(m, DREAM3D, CellFieldData, Phases, F, -303,  int32_t, Int32ArrayType, fields, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellFieldData, Active, -304, bool, BoolArrayType, fields, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellData, Quats, -300, float, FloatArrayType, voxels, 5)
  GET_PREREQ_DATA(m, DREAM3D, CellData, EulerAngles, -304, float, FloatArrayType, voxels, 3)
  GET_PREREQ_DATA(m, DREAM3D, CellData, GoodVoxels, -304, bool, BoolArrayType, voxels, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellFieldData, NumNeighbors, -306, int32_t, Int32ArrayType, fields, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellFieldData, AvgQuats, -301, float, FloatArrayType, fields, 4)
  GET_PREREQ_DATA(m, DREAM3D, CellFieldData, NumCells, -302, int32_t, Int32ArrayType, fields, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellFieldData, SurfaceFields, -303, bool, BoolArrayType, fields, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellFieldData, EquivalentDiameters, -305, float, FloatArrayType, fields)
  GET_PREREQ_DATA(m, DREAM3D, CellFieldData, Omega3s, -306, float, FloatArrayType, fields, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellFieldData, AxisEulerAngles, -307, float, FloatArrayType, fields, 3)
  GET_PREREQ_DATA(m, DREAM3D, CellFieldData, AxisLengths, -308, float, FloatArrayType, fields)
  GET_PREREQ_DATA(m, DREAM3D, CellFieldData, Volumes, -309, float, FloatArrayType, fields, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellFieldData, Centroids, -310, float, FloatArrayType, fields, 3)
  GET_PREREQ_DATA(m, DREAM3D, CellFieldData, Schmids, -305, float, FloatArrayType, fields, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellFieldData, SlipSystems, -306, int32_t, Int32ArrayType, fields, 1)



  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellFieldData, Schmids, float, FloatArrayType, fields, 1)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellFieldData, GrainAvgMisorientations, float, FloatArrayType, fields, 3)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellFieldData, BiasedFields, bool, BoolArrayType, fields, 1)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellFieldData, AvgQuats, float, FloatArrayType, fields, 5)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellFieldData, Active, bool, BoolArrayType, fields, 1)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellFieldData, Neighborhoods, int32_t, Int32ArrayType, fields, 3)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellFieldData, NumCells, int32_t, Int32ArrayType, fields, 1)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellFieldData, Centroids, float, FloatArrayType, fields, 3)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellFieldData, Volumes, float, FloatArrayType, fields, 1)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellFieldData, AxisLengths, float, FloatArrayType, fields, 3)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellFieldData, AxisEulerAngles, float, FloatArrayType, fields, 3)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellFieldData, Omega3s, float,FloatArrayType, fields, 1)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellFieldData, EquivalentDiameters, float,FloatArrayType, fields, 1)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellFieldData, FieldEulerAngles, float, FloatArrayType, fields, 3)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellFieldData, Phases, F, int32_t, Int32ArrayType, fields, 1)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellFieldData, SlipSystems, int32_t, Int32ArrayType, fields, 1)
  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellFieldData, AspectRatios, float,FloatArrayType, fields, 2)

 // Now we are going to get a "Pointer" to the NeighborList object out of the DataContainer
  m_NeighborList = NeighborList<int>::SafeObjectDownCast<IDataArray*, NeighborList<int>* >
                                          (m->getCellFieldData(DREAM3D::FieldData::NeighborList).get());
  if(m_NeighborList == NULL)
  {
    ss << "NeighborLists Array Not Initialized At Beginning of MatchCrystallography Filter" ;
    setErrorCondition(-308);
  }

  // And we do the same for the SharedSurfaceArea list
  m_SharedSurfaceAreaList = NeighborList<float>::SafeObjectDownCast<IDataArray*, NeighborList<float>*>
                                 (m->getCellFieldData(DREAM3D::FieldData::SharedSurfaceAreaList).get());
  if(m_SharedSurfaceAreaList == NULL)
  {
    ss << "SurfaceAreaLists Array Not Initialized At Beginning of MatchCrystallography Filter" ;
    setErrorCondition(-309);
  }



  GET_PREREQ_DATA(m, DREAM3D, EnsembleData, TotalSurfaceArea, -303,  float, FloatArrayType, m->crystruct.size(), 1);




 */
