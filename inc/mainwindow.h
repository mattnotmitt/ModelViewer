#pragma once

#include <array>
#include <stdexcept>

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QVTKInteractor.h>

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCellPicker.h>
#include <vtkCubeSource.h>
#include <vtkDataSetMapper.h>
#include <vtkExtractSelection.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPlaneSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
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

public:
    /**
     * Loads Window &
     * @param parent
     */
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
    QVTKInteractor* GetInteractor();

private:
    /**
     *
     */
    Ui::MainWindow *ui;
    LegacyLoader loader;
    vtkSmartPointer<vtkRenderer> renderer;
};

