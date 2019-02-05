//
// Created by matt on 05/02/19.
//

#include "LegacyLoader.h"

std::vector<vtkSmartPointer<vtkActor>> LegacyLoader::loadModel(std::string path) {
    return std::vector<vtkSmartPointer<vtkActor>>();
}

vtkSmartPointer<vtkActor> LegacyLoader::loadPyramid(std::vector<std::shared_ptr<Vec3>> &CellVertices,
        std::shared_ptr<Material> CellMaterial) {
    return vtkSmartPointer<vtkActor>();
}

vtkSmartPointer<vtkActor> LegacyLoader::loadTetrahedron(std::vector<std::shared_ptr<Vec3>> &CellVertices,
        std::shared_ptr<Material> CellMaterial) {
    return vtkSmartPointer<vtkActor>();
}

vtkSmartPointer<vtkActor>LegacyLoader::loadHexahedron(std::vector<std::shared_ptr<Vec3>> &CellVertices,
        std::shared_ptr<Material> CellMaterial) {
    return vtkSmartPointer<vtkActor>();
}
