//
// Created by matt on 09/04/19.
//

#pragma once

#include <QColor>

#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkShrinkPolyData.h>

#include <memory>

#include "Model.h"

class CurrentModel {
public:
    bool isSTL;
    vtkSmartPointer<vtkActor> currentActor;
    vtkSmartPointer<vtkPolyData> currentData = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkShrinkPolyData> shrinkFilter = vtkSmartPointer<vtkShrinkPolyData>::New();
    Model currentMetadata;
    QColor color = QColor("#ffffff");
};

