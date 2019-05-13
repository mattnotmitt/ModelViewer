//
// Created by matt on 09/04/19.
//

#pragma once

/**
 * @file CurrentModel class definitions
 */

#include <QColor>

#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkShrinkPolyData.h>

#include <memory>

#include "Model.h"

/**
 * Stores currently loaded model in viewing software
 */
class CurrentModel {
public:
    /**
     * Stores whether or not current model is sourced from STL file
     */
    bool isSTL;
    /**
     * Stores current vtkActor
     */
    vtkSmartPointer<vtkActor> currentActor;
    /**
     * Stores current raw data that created the currentActor
     */
    vtkSmartPointer<vtkPolyData> currentData = vtkSmartPointer<vtkPolyData>::New();
    /**
     * Stores current filter that shrinks the data
     */
    vtkSmartPointer<vtkShrinkPolyData> shrinkFilter = vtkSmartPointer<vtkShrinkPolyData>::New();
    /**
     * Stores current Model class that non-STL models are loaded from
     */
    Model currentMetadata;
    /**
     * Stores current color of STL models
     */
    QColor color = QColor("#ffffff");
};

