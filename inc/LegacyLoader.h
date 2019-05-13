//
// Created by matt on 05/02/19.
//

#pragma once

/**
 * @file LegacyLoader Class Definitions
 */

// C stdlib
#include <stdio.h>
// C++ stdlib
#include <string>
#include <vector>
#include <memory>
// VTK Libraries
#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkDataSetMapper.h>
#include <vtkIntArray.h>
#include <vtkHexahedron.h>
#include <vtkLookupTable.h>
#include <vtkNamedColors.h>
#include <vtkPoints.h>
#include <vtkProperty.h>
#include <vtkPyramid.h>
#include <vtkSmartPointer.h>
#include <vtkTetra.h>
#include <vtkUnstructuredGrid.h>
// ModelLoader Libraries
#include <Material.h>
#include <Model.h>
#include <Vec3.h>

/**
 * Stores data loaded by LegacyLoader::loadModel
 */
class ModelData {
public:
    vtkSmartPointer<vtkUnstructuredGrid> loadedModel;
    Model modelData;
};

/**
 * Facilitates loading proprietary model format into ModelViewer
 */
class LegacyLoader {
private:
    /**
     * Stores RGB data
     */
    typedef struct RGB{
        double r;
        double g;
        double b;
    } RGB;
    /**
     * Converts hex value to RGB values
     * @param hexValue
     * @return RGB
     */
    RGB colorConverter(int hexValue)
    {
        RGB rgbColor;
        rgbColor.r = ((hexValue >> 16) & 0xFF);  // Extract the RR byte
        rgbColor.g = ((hexValue >> 8) & 0xFF);   // Extract the GG byte
        rgbColor.b = ((hexValue) & 0xFF);        // Extract the BB byte

        return rgbColor;
    }
public:
    /**
     * Loads model from specified path
     * @param path
     * @return
     */
    ModelData loadModel(std::string &path);
    /**
     * Retrieves data from Pyramid class instance
     * @param CellVertices
     * @return
     */
    vtkSmartPointer<vtkPyramid> loadPyramid(const std::vector<std::shared_ptr<Vec3>> &CellVertices);
    /**
     * Retrieves data from Tetrahedron class instance
     * @param CellVertices
     * @return
     */
    vtkSmartPointer<vtkTetra> loadTetrahedron(const std::vector<std::shared_ptr<Vec3>> &CellVertices);
    /**
     * Retrieves data from Hexahedron class instance
     * @param CellVertices
     * @return
     */
    vtkSmartPointer<vtkHexahedron> loadHexahedron(const std::vector<std::shared_ptr<Vec3>> &CellVertices);
};