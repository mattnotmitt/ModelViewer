//
// Created by matt on 05/02/19.
//

#pragma once

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

class LegacyLoader {
private:
    typedef struct RGB{
        double r;
        double g;
        double b;
    } RGB;
    RGB colorConverter(int hexValue)
    {
        RGB rgbColor;
        rgbColor.r = ((hexValue >> 16) & 0xFF);  // Extract the RR byte
        rgbColor.g = ((hexValue >> 8) & 0xFF);   // Extract the GG byte
        rgbColor.b = ((hexValue) & 0xFF);        // Extract the BB byte

        return rgbColor;
    }
public:
    vtkSmartPointer<vtkActor> loadModel(std::string &path);
    vtkSmartPointer<vtkPyramid> loadPyramid(const std::vector<std::shared_ptr<Vec3>> &CellVertices);
    vtkSmartPointer<vtkTetra> loadTetrahedron(const std::vector<std::shared_ptr<Vec3>> &CellVertices);
    vtkSmartPointer<vtkHexahedron> loadHexahedron(const std::vector<std::shared_ptr<Vec3>> &CellVertices);
};
