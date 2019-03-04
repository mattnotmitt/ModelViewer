//
// Created by matt on 05/02/19.
//

#include "LegacyLoader.h"

vtkSmartPointer<vtkActor> LegacyLoader::loadModel(std::string &path) {
    auto LegacyModel = Model(path);
    vtkSmartPointer<vtkLookupTable> ColourLUT =
            vtkSmartPointer<vtkLookupTable>::New();
    // Set the background color.
    auto uGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    auto legacyCells = LegacyModel.getCells();
    auto legacyCellsSize = (vtkIdType) legacyCells.size();
    uGrid->Allocate(legacyCellsSize);
    /*
    ColourLUT->SetNumberOfTableValues(legacyCellsSize);
    ColourLUT->Build();
    auto cellData = vtkSmartPointer<vtkIntArray>::New();
    for (int i = 0; i < legacyCellsSize; i++)
    {
        cellData->InsertNextValue(i);
    }*/
    vtkSmartPointer<vtkUnsignedCharArray> cellData =
            vtkSmartPointer<vtkUnsignedCharArray>::New();
    cellData->SetNumberOfComponents(3);
    cellData->SetNumberOfTuples(legacyCellsSize);

    for (const auto& cellPair: legacyCells){
        auto cell = cellPair.second;

        const char* colourString = cell->getCellMaterial()->getColour().c_str();
        std::cout << colourString << std::endl;
        auto colour = colorConverter((int) std::strtol(colourString, nullptr, 16));
        //delete colourString;
        //auto lut = ColourLUT;
        //std::cout << cell->getIndex() << " raw: " << colour.r << ", " << colour.g << ", " << colour.b << std::endl;
        double colours[3] = {colour.r, colour.g, colour.b};
        cellData->InsertTuple(cell->getIndex(), colours);
        /*
        ColourLUT->SetTableValue(cell->getIndex(), colours);
        double rgb[3];
        ColourLUT->GetColor(cell->getIndex(), rgb);
        std::cout << cell->getIndex() << " vtk: " << rgb[0] << ", " << rgb[1] << ", " << rgb[2] << std::endl;
        */

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
    /*
    double rgb[3];
    double rgb2[3];
    ColourLUT->GetColor(0, rgb);
    ColourLUT->GetColor(128, rgb2);
    std::cout << rgb[0] << ", " << rgb[1] << ", " << rgb[2] << std::endl;
    std::cout << rgb2[0] << ", " << rgb2[1] << ", " << rgb2[2] << std::endl;*/

    uGrid->GetCellData()->SetScalars(cellData);
    std::cout << *(cellData->GetTuple(1)) << std::endl;
    //std::cout << *(uGrid->GetCellData()) << std::endl;
    //std::cout << ColourLUT->GetNumberOfTableValues() << std::endl;
    //std::cout << *ColourLUT << std::endl;
    // Create the points.
    vtkSmartPointer<vtkPoints> points =
            vtkSmartPointer<vtkPoints>::New();
    for (const auto& vertex: LegacyModel.getVertices()){ // Loop through vertices and save information about them to file
        auto point = vertex.second;
        points->InsertPoint(point.getIndex(), point.getX(), point.getY(), point.getZ());
    }
    uGrid->SetPoints(points);
    // Visualize.
    vtkSmartPointer<vtkDataSetMapper> mapper =
            vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(uGrid);
    mapper->SetColorModeToDefault();
    //mapper->SetLookupTable(ColourLUT);
    //mapper->UseLookupTableScalarRangeOff();
    //mapper->SetScalarModeToUseCellData();
    /*
    mapper->ScalarVisibilityOn();
    mapper->SetScalarModeToUseCellData();
    mapper->UseLookupTableScalarRangeOn();
    mapper->SetColorModeToMapScalars();
    mapper->Update();*/


    vtkSmartPointer<vtkActor> actor =
            vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    return actor;
}

vtkSmartPointer<vtkPyramid> LegacyLoader::loadPyramid(const std::vector<std::shared_ptr<Vec3>> &CellVertices) {
    return vtkSmartPointer<vtkPyramid>::New();
}

vtkSmartPointer<vtkTetra> LegacyLoader::loadTetrahedron(const std::vector<std::shared_ptr<Vec3>> &CellVertices) {
    return vtkSmartPointer<vtkTetra>::New();
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
