/* ============================================================================
 * Copyright (c) 2010, Michael A. Jackson (BlueQuartz Software)
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
 * Neither the name of Michael A. Groeber, Michael A. Jackson,
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
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "FindNRingNeighbors.h"

#include <stdio.h>
#include <sstream>

#include "DREAM3DLib/Common/ScopedFileMonitor.hpp"
#include "DREAM3DLib/Utilities/DREAM3DEndian.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindNRingNeighbors::FindNRingNeighbors() :
  m_SurfaceDataContainer(NULL),
  m_TriangleId(-1),
  m_RegionId0(0),
  m_RegionId1(0),
  m_Ring(2),
  m_WriteBinaryFile(false),
  m_WriteConformalMesh(true)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindNRingNeighbors::~FindNRingNeighbors()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindNRingNeighbors::setRegionIds(int g, int r)
{
  m_RegionId0 = g;
  m_RegionId1 = r;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FaceArray::UniqueFaceIds_t& FindNRingNeighbors::getNRingTriangles()
{
  return m_NRingTriangles;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindNRingNeighbors::generate(int32_t* faceLabels)
{
  BOOST_ASSERT(m_SurfaceDataContainer != NULL);
  SurfaceDataContainer* sm = getSurfaceDataContainer();

  // Clear out any previous triangles
  m_NRingTriangles.clear();

  // Get the Triangle List from the Data Container
  FaceArray::Pointer trianglesPtr = sm->getFaces();
  if(trianglesPtr == NULL)
  {
    return;
  }
  FaceArray::Face_t* triangles = trianglesPtr->getPointer(0);

  // Make sure we have the proper connectivity built
  Int32DynamicListArray::Pointer node2TrianglePtr = trianglesPtr->getFacesContainingVert();
  if (node2TrianglePtr.get() == NULL)
  {
    sm->getFaces()->findFacesContainingVert();
    node2TrianglePtr = sm->getFaces()->getFacesContainingVert();
  }

  // Figure out these boolean values for a sanity check
  bool check0 = faceLabels[m_TriangleId * 2] == m_RegionId0 && faceLabels[m_TriangleId * 2 + 1] == m_RegionId1;
  bool check1 = faceLabels[m_TriangleId * 2 + 1] == m_RegionId0 && faceLabels[m_TriangleId * 2] == m_RegionId1;

#if 1
  if ( check0 == false && check1 == false)
  {
    qDebug() << "FindNRingNeighbors Seed triangle ID does not have a matching Region ID for " << m_RegionId0 << " & " << m_RegionId1 << "\n";
    qDebug() << "Region Ids are: " << faceLabels[m_TriangleId * 2] << " & " << faceLabels[m_TriangleId * 2 + 1] << "\n";
    return;
  }
#endif


  // Add our seed triangle
  m_NRingTriangles.insert(m_TriangleId);

  for (int ring = 0; ring < m_Ring; ++ring)
  {
    // Make a copy of the 1 Ring Triangles that we just found so that we can use those triangles as the
    // seed triangles for the 2 Ring triangles
    FaceArray::UniqueFaceIds_t lcvTriangles(m_NRingTriangles);

    // Now that we have the 1 ring triangles, get the 2 Ring neighbors from that list
    for(FaceArray::UniqueFaceIds_t::iterator triIter = lcvTriangles.begin(); triIter != lcvTriangles.end(); ++triIter)
    {
      FaceArray::Face_t& face = triangles[*triIter];
      // For each node, get the triangle ids that the node belongs to
      for(int i = 0; i < 3; ++i)
      {
        // Get all the triangles for this Node id
        uint16_t tCount = node2TrianglePtr->getNumberOfElements(face.verts[i]);
        int32_t* data = node2TrianglePtr->getElementListPointer(face.verts[i]);

        // Copy all the triangles into our "2Ring" set which will be the unique set of triangle ids
        for(uint16_t t = 0; t < tCount; ++t)
        {
          int tid = data[t];
          check0 = faceLabels[tid * 2] == m_RegionId0 && faceLabels[tid * 2 + 1] == m_RegionId1;
          check1 = faceLabels[tid * 2 + 1] == m_RegionId0 && faceLabels[tid * 2] == m_RegionId1;
          if (check0 == true || check1 == true)
          {
            m_NRingTriangles.insert(static_cast<int>(tid) );
          }
        }
      }
    }
  }

}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int FindNRingNeighbors::writeVTKFile(const QString& outputVtkFile)
{

  SurfaceDataContainer* m = getSurfaceDataContainer();  /* Place all your code to execute your filter here. */
  VertexArray& nodes = *(m->getVertices());
  int nNodes = nodes.getNumberOfTuples();

  FILE* vtkFile = NULL;
  vtkFile = fopen(outputVtkFile.toLatin1().data(), "wb");
  if (NULL == vtkFile)
  {
    // QString ss = QObject::tr("Error creating file '%1'").arg(outputVtkFile);
    return -100;
  }
  ScopedFileMonitor vtkFileMonitor(vtkFile);

  fprintf(vtkFile, "# vtk DataFile Version 2.0\n");
  fprintf(vtkFile, "Data set from DREAM.3D Surface Meshing Module\n");
  if (m_WriteBinaryFile)
  {
    fprintf(vtkFile, "BINARY\n");
  }
  else
  {
    fprintf(vtkFile, "ASCII\n");
  }
  fprintf(vtkFile, "DATASET POLYDATA\n");


  fprintf(vtkFile, "POINTS %d float\n", nNodes);
  float pos[3] = {0.0f, 0.0f, 0.0f};

  size_t totalWritten = 0;
  // Write the POINTS data (Vertex)
  for (int i = 0; i < nNodes; i++)
  {
    VertexArray::Vert_t& n = nodes[i]; // Get the current Node
    //  if (m_SurfaceMeshNodeType[i] > 0)
    {
      pos[0] = static_cast<float>(n.pos[0]);
      pos[1] = static_cast<float>(n.pos[1]);
      pos[2] = static_cast<float>(n.pos[2]);
      if (m_WriteBinaryFile == true)
      {
        DREAM3D::Endian::FromSystemToBig::convert<float>(pos[0]);
        DREAM3D::Endian::FromSystemToBig::convert<float>(pos[1]);
        DREAM3D::Endian::FromSystemToBig::convert<float>(pos[2]);
        totalWritten = fwrite(pos, sizeof(float), 3, vtkFile);
        if (totalWritten != sizeof(float) * 3)
        {

        }
      }
      else
      {
        fprintf(vtkFile, "%f %f %f\n", pos[0], pos[1], pos[2]); // Write the positions to the output file
      }
    }
  }

  // Write the triangle indices into the vtk File
  FaceArray& triangles = *(m->getFaces());

  int tData[4];
  int nT = m_NRingTriangles.size();
  int triangleCount = nT;
  //  int tn1, tn2, tn3;
  if (false == m_WriteConformalMesh)
  {
    triangleCount = nT * 2;
  }
  // Write the CELLS Data
  fprintf(vtkFile, "POLYGONS %d %d\n", triangleCount, (triangleCount * 4));
  for (QSet<int32_t>::iterator iter = m_NRingTriangles.begin(); iter != m_NRingTriangles.end(); ++iter)
  {
    int32_t tid = *iter;
    tData[1] = triangles[tid].verts[0];
    tData[2] = triangles[tid].verts[1];
    tData[3] = triangles[tid].verts[2];
    if (m_WriteBinaryFile == true)
    {
      tData[0] = 3; // Push on the total number of entries for this entry
      DREAM3D::Endian::FromSystemToBig::convert<int>(tData[0]);
      DREAM3D::Endian::FromSystemToBig::convert<int>(tData[1]); // Index of Vertex 0
      DREAM3D::Endian::FromSystemToBig::convert<int>(tData[2]); // Index of Vertex 1
      DREAM3D::Endian::FromSystemToBig::convert<int>(tData[3]); // Index of Vertex 2
      fwrite(tData, sizeof(int), 4, vtkFile);
      if (false == m_WriteConformalMesh)
      {
        tData[0] = tData[1];
        tData[1] = tData[3];
        tData[3] = tData[0];
        tData[0] = 3;
        DREAM3D::Endian::FromSystemToBig::convert<int>(tData[0]);
        fwrite(tData, sizeof(int), 4, vtkFile);
      }
    }
    else
    {
      fprintf(vtkFile, "3 %d %d %d\n", tData[1], tData[2], tData[3]);
      if (false == m_WriteConformalMesh)
      {
        fprintf(vtkFile, "3 %d %d %d\n", tData[3], tData[2], tData[1]);
      }
    }
  }



  fprintf(vtkFile, "\n");
  return 0;
}


