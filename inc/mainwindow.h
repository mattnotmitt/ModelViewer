#pragma once

#include <array>
#include <stdexcept>

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
#include <vtkTriangleFilter.h>

#include "CurrentModel.h"
#include "LegacyLoader.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public slots:
    /**
     *
     */
    void handleFileOpen();
    void handleChangeColour();
    void handleShrinkActor();
    void handleChangeBkg();
    void handleFileSave();
    /*void handleHelp();
    void handlePrint();*/

public:
    /**
     * Loads Window &
     * @param parent
     */
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
    QVTKInteractor* GetInteractor();
    vtkSmartPointer<vtkUnstructuredGrid> PickerGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    int TotalCells = 0;
    int TotalPoints = 0;

private:
    /**
     *
     */
    Ui::MainWindow *ui;
    LegacyLoader loader;
    vtkSmartPointer<vtkRenderer> renderer;
    CurrentModel currentModel;
};

