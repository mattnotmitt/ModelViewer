//
// Created by matt on 05/02/19.
//

#include "LegacyLoader.h"

vtkSmartPointer<vtkUnstructuredGrid> LegacyLoader::loadModel(std::string &path) {
    auto LegacyModel = Model(path);
    vtkSmartPointer<vtkLookupTable> ColourLUT =
            vtkSmartPointer<vtkLookupTable>::New();
    // Set the background color.
    auto uGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    auto legacyCells = LegacyModel.getCells();
    auto legacyCellsSize = (vtkIdType) legacyCells.size();
    uGrid->Allocate(legacyCellsSize);

    vtkSmartPointer<vtkUnsignedCharArray> cellData =
            vtkSmartPointer<vtkUnsignedCharArray>::New();
    cellData->SetNumberOfComponents(3);
    cellData->SetNumberOfTuples(legacyCellsSize);

    for (const auto& cellPair: legacyCells){
        auto cell = cellPair.second;

        const char* colourString = cell->getCellMaterial()->getColour().c_str();
        auto colour = colorConverter((int) std::strtol(colourString, nullptr, 16));
        double colours[3] = {colour.r, colour.g, colour.b};
        cellData->InsertTuple(cell->getIndex(), colours);

        switch (cell->CellType) {
            case (Cell::Type::PYRAMID): {
                auto pyramid = loadPyramid(cell->getCellVertices());
                uGrid->InsertNextCell(pyramid->GetCellType(), pyramid->GetPointIds());
                break;
            } case (Cell::Type::TETRAHEDRON): {
                auto tetra = loadTetrahedron(cell->getCellVertices());
                uGrid->InsertNextCell(tetra->GetCellType(), tetra->GetPointIds());
                break;
            } case (Cell::Type::HEXAHEDRON): {
                auto hexahedron = loadHexahedron(cell->getCellVertices());
                uGrid->InsertNextCell(hexahedron->GetCellType(), hexahedron->GetPointIds());
                break;
            } case (Cell::Type::NONE):
                break;
        }
    }

    uGrid->GetCellData()->SetScalars(cellData);

    // Create the points.
    vtkSmartPointer<vtkPoints> points =
            vtkSmartPointer<vtkPoints>::New();
    for (const auto& vertex: LegacyModel.getVertices()){ // Loop through vertices and save information about them to file
        auto point = vertex.second;
        points->InsertPoint(point.getIndex(), point.getX(), point.getY(), point.getZ());
    }
    uGrid->SetPoints(points);

    return uGrid;
}

vtkSmartPointer<vtkPyramid> LegacyLoader::loadPyramid(const std::vector<std::shared_ptr<Vec3>> &CellVertices) {
    // TODO: Implement pyramid loader
	vtkSmartPointer<vtkPyramid> pyramid =
		vtkSmartPointer<vtkPyramid>::New();

	for (auto i = 0; i < CellVertices.size(); ++i)
	{
		auto vertex = *CellVertices[i];
		pyramid->GetPointIds()->SetId(i, vertex.getIndex());
	}

    return pyramid;
}

vtkSmartPointer<vtkTetra> LegacyLoader::loadTetrahedron(const std::vector<std::shared_ptr<Vec3>> &CellVertices) {
    // TODO: Implement tetrahedron loader
	vtkSmartPointer<vtkTetra> tetra =
		vtkSmartPointer<vtkTetra>::New();

	for (auto i = 0; i < CellVertices.size(); ++i)
	{
		auto vertex = *CellVertices[i];
		tetra->GetPointIds()->SetId(i, vertex.getIndex());
	}
	return tetra;

}

vtkSmartPointer<vtkHexahedron> LegacyLoader::loadHexahedron(const std::vector<std::shared_ptr<Vec3>> &CellVertices) {
    // Create a hexahedron from the points.
    vtkSmartPointer<vtkHexahedron> hex =
            vtkSmartPointer<vtkHexahedron>::New();

    for (auto i = 0; i < CellVertices.size(); ++i)
    {
        auto vertex = *CellVertices[i];
        hex->GetPointIds()->SetId(i, vertex.getIndex());
    }

    return hex;

}
