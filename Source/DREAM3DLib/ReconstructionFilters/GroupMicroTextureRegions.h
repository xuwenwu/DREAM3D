/* ============================================================================
 * Copyright (c) 2011 Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2011 Dr. Michael A. Groeber (US Air Force Research Laboratories)
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

#ifndef GroupMicroTextureRegions_H_
#define GroupMicroTextureRegions_H_

#include <vector>
#include <QtCore/QString>

#include "DREAM3DLib/DREAM3DLib.h"
#include "DREAM3DLib/Common/DREAM3DSetGetMacros.h"
#include "DREAM3DLib/DataArrays/IDataArray.h"

#include "DREAM3DLib/Common/AbstractFilter.h"
#include "DREAM3DLib/DataContainers/VolumeDataContainer.h"
#include "DREAM3DLib/OrientationOps/OrientationOps.h"
#include "DREAM3DLib/DataArrays/NeighborList.hpp"

/**
 * @class GroupMicroTextureRegions GroupMicroTextureRegions.h DREAM3DLib/ReconstructionFilters/GroupMicroTextureRegions.h
 * @brief
 * @author
 * @date Nov 19, 2011
 * @version 1.0
 */
class DREAM3DLib_EXPORT GroupMicroTextureRegions : public AbstractFilter
{
  public:
    DREAM3D_SHARED_POINTERS(GroupMicroTextureRegions)
    DREAM3D_STATIC_NEW_MACRO(GroupMicroTextureRegions)
    DREAM3D_TYPE_MACRO_SUPER(GroupMicroTextureRegions, AbstractFilter)


    virtual ~GroupMicroTextureRegions();
    DREAM3D_INSTANCE_STRING_PROPERTY(DataContainerName)
    DREAM3D_INSTANCE_STRING_PROPERTY(CellFeatureAttributeMatrixName)
    DREAM3D_INSTANCE_STRING_PROPERTY(CellEnsembleAttributeMatrixName)
    DREAM3D_INSTANCE_STRING_PROPERTY(CellAttributeMatrixName)
    
   //------ Required Cell Data
        //------ Created Cell Data
            //------ Required Feature Data
                DREAM3D_INSTANCE_STRING_PROPERTY(ContiguousNeighborListArrayName)
    DREAM3D_INSTANCE_STRING_PROPERTY(NonContiguousNeighborListArrayName)
    //------ Created Feature Data
        
    //------ Required Ensemble Data
    
    virtual const QString getGroupName() { return DREAM3D::FilterGroups::ReconstructionFilters; }
    virtual const QString getSubGroupName() {return DREAM3D::FilterSubGroups::GroupingFilters;}
    virtual const QString getHumanLabel() { return "Identify MicroTexture (C-Axis Misorientation)"; }

    DREAM3D_INSTANCE_PROPERTY(float, CAxisTolerance)
    DREAM3D_INSTANCE_PROPERTY(bool, UseNonContiguousNeighbors)

    virtual void setupFilterParameters();
    virtual int writeFilterParameters(AbstractFilterParametersWriter* writer, int index);

    /**
    * @brief This method will read the options from a file
    * @param reader The reader that is used to read the options from a file
    */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index);

    /**
     * @brief Reimplemented from @see AbstractFilter class
     */
    virtual void execute();
    virtual void preflight();

  protected:
    GroupMicroTextureRegions();

    void merge_micro_texture_regions();
    void characterize_micro_texture_regions();

  private:
    DEFINE_PTR_WEAKPTR_DATAARRAY(int32_t, FeatureIds)
    DEFINE_PTR_WEAKPTR_DATAARRAY(int32_t, CellParentIds)
    DEFINE_PTR_WEAKPTR_DATAARRAY(int32_t, FeatureParentIds)
    DEFINE_PTR_WEAKPTR_DATAARRAY(float, MTRdensity)
    DEFINE_PTR_WEAKPTR_DATAARRAY(float, AvgQuats)
    DEFINE_PTR_WEAKPTR_DATAARRAY(bool, Active)
    DEFINE_PTR_WEAKPTR_DATAARRAY(int32_t, FeaturePhases)
    DEFINE_PTR_WEAKPTR_DATAARRAY(float, Volumes)
    NeighborList<int>* m_ContiguousNeighborList;
    NeighborList<int>* m_NonContiguousNeighborList;

    DEFINE_PTR_WEAKPTR_DATAARRAY(unsigned int, CrystalStructures)


    std::vector<int> parentnumbers;
    std::vector<bool> beenChecked;
    std::vector<float> intensities;


    QVector<OrientationOps::Pointer> m_OrientationOps;

    void dataCheck(bool preflight, size_t voxels, size_t features, size_t ensembles);


    GroupMicroTextureRegions(const GroupMicroTextureRegions&); // Copy Constructor Not Implemented
    void operator=(const GroupMicroTextureRegions&); // Operator '=' Not Implemented
};

#endif /* GroupMicroTextureRegions_H_ */
