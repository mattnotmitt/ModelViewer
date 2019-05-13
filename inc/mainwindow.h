#pragma once

/**
 * @file Main file of project, handles UI and calling other classes
 */

#include <array>
#include <stdexcept>

#include <QColorDialog>
#include <QFile>
#include <QFileDialog>
#include <QIcon>
#include <QMainWindow>
#include <QMessageBox>
#include <QResource>
#include <QTextStream>
#include <QVTKInteractor.h>


#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCellPicker.h>
#include <vtkCubeSource.h>
#include <vtkDataSetMapper.h>
#include <vtkExtractSelection.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkGeometryFilter.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkMergeCells.h>
#include <vtkPlaneSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkPropPicker.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>
#include <vtkNew.h>
#include <vtkTriangleFilter.h>
#include <vtkNamedColors.h>

#include "CurrentModel.h"
#include "LegacyLoader.h"
/*
class MouseInteractorStyle : public vtkInteractorStyleTrackballCamera {
public:
    MouseInteractorStyle();

    static MouseInteractorStyle *New();

    virtual void OnLeftButtonDown();

    vtkSmartPointer<vtkPolyData> Data;
    vtkSmartPointer<vtkDataSetMapper> selectedMapper;
    vtkSmartPointer<vtkActor> selectedActor;
};*/

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public slots:
    /**
     * Handle slot triggered when file is to be opened
     */
    void handleFileOpen();
    /**
     * Handle slot triggered when STL actor colour is to be changed
     */
    void handleChangeColour();
    /**
     * Handle change in shrink factor slider
     * @param value
     */
    void handleShrinkActor(int value);
    /**
     * Handle slow triggered when renderer background colour is to be changed
     */
    void handleChangeBkg();
    /**
     * Handle slot triggered when file is to be saved
     */
    void handleFileSave();
    /**
     * Handle slot triggered when camera is reset
     */
    void handleCameraReset();
    /**
     * Handle slot triggered when filters reset
     */
    void handleFilterReset();

public:
    /**
     * Loads Window & configures UI
     * @param parent
     */
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
    //QVTKInteractor* GetInteractor();
    //vtkSmartPointer<vtkUnstructuredGrid> PickerGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    //int TotalCells = 0;
    //int TotalPoints = 0;
    /**
     * Update actor stats table
     */
    void updateActorStats();

private:
    /**
     *
     */
    Ui::MainWindow *ui;
    LegacyLoader loader;
    vtkSmartPointer<vtkRenderer> renderer;
    CurrentModel currentModel;
};

