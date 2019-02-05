//
// Created by matt on 05/02/19.
//

#pragma once

// C++ stdlib
#include <string>
#include <vector>
#include <memory>
// VTK Libraries
#include <vtkActor.h>
#include <vtkSmartPointer.h>
// ModelLoader Libraries
#include <ModelLoader/inc/Vec3.h>
#include <ModelLoader/inc/Material.h>

class LegacyLoader {
    std::vector<vtkSmartPointer<vtkActor>> loadModel(std::string path);
    vtkSmartPointer<vtkActor> loadPyramid(std::vector<std::shared_ptr<Vec3>> &CellVertices,
            std::shared_ptr<Material> CellMaterial);
    vtkSmartPointer<vtkActor> loadTetrahedron(std::vector<std::shared_ptr<Vec3>> &CellVertices,
            std::shared_ptr<Material> CellMaterial);
    vtkSmartPointer<vtkActor> loadHexahedron(std::vector<std::shared_ptr<Vec3>> &CellVertices,
            std::shared_ptr<Material> CellMaterial);
};
