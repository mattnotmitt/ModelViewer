//
// Created by matt on 09/04/19.
//

#pragma once

#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkShrinkPolyData.h>

class CurrentModel {
public:
    bool isSTL;
    vtkSmartPointer<vtkActor> currentActor;
    vtkSmartPointer<vtkShrinkPolyData> shrinkFilter = vtkSmartPointer<vtkShrinkPolyData>::New();
};

