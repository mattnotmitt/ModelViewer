#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    // standard call to setup Qt UI (same as previously)
    renderer = vtkSmartPointer<vtkRenderer>::New();
    ui->setupUi(this);

    connect(ui->actionFileOpen, &QAction::triggered, this, &MainWindow::handleFileOpen);

    // Now need to create a VTK render window and link it to the QtVTK widget
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    ui->qvtkWidget->SetRenderWindow(
            renderWindow);            // note that vtkWidget is the name I gave to my QtVTKOpenGLWidget in Qt creator

    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    std::array<unsigned char, 4> bkg{{51, 77, 102, 255}};
    colors->SetColor("BkgColor", bkg.data());

    // Create a renderer, and render window
    //vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();		// ###### We've already created the renderWindow this time ######
    ui->qvtkWidget->GetRenderWindow()->AddRenderer(
            renderer);

    renderer->SetBackground(colors->GetColor3d("BkgColor").GetData());

    // Setup the renderers's camera
    renderer->ResetCamera();
    //--------------------------------------------- /Code From Example 1 -------------------------------------------------------------------------*/
}

void MainWindow::handleFileOpen() {
    try {
        std::string filename = QFileDialog::getOpenFileName(this, tr("Open Image"), tr(""), tr("Model Files (*.mod *.stl)")).toStdString();
        if (filename.find_last_of(".") != std::string::npos) {
            std::string ext = filename.substr(filename.find_last_of(".") + 1);
            if (ext == "stl") {
                vtkSmartPointer<vtkSTLReader> stlReader =
                        vtkSmartPointer<vtkSTLReader>::New();
                stlReader->SetFileName(filename.c_str());
                stlReader->Update();

                auto shrink = currentModel.shrinkFilter;
                shrink->SetInputData(stlReader->GetOutput());
                shrink->SetShrinkFactor(1);

                // Visualize
                vtkSmartPointer<vtkPolyDataMapper> stlMapper =
                        vtkSmartPointer<vtkPolyDataMapper>::New();
                stlMapper->SetInputData(shrink->GetOutput());

                vtkSmartPointer<vtkActor> stlActor =
                        vtkSmartPointer<vtkActor>::New();
                stlActor->SetMapper(stlMapper);

                renderer->RemoveAllViewProps();
                renderer->AddActor(stlActor);
                renderer->ResetCamera();
                renderer->GetRenderWindow()->Render();

                currentModel.isSTL = true;
                currentModel.currentActor = stlActor;
            } else if (ext == "mod") {
                auto data = loader.loadModel(filename);
                // Visualize.

                auto shrink = currentModel.shrinkFilter;
                shrink->SetInputData(data);
                shrink->SetShrinkFactor(1);

                vtkSmartPointer<vtkDataSetMapper> mapper =
                        vtkSmartPointer<vtkDataSetMapper>::New();
                mapper->SetInputData(shrink->GetOutput());
                mapper->SetColorModeToDefault();

                vtkSmartPointer<vtkActor> actor =
                        vtkSmartPointer<vtkActor>::New();
                actor->SetMapper(mapper);

                renderer->RemoveAllViewProps();
                renderer->AddActor(actor);
                renderer->ResetCamera();
                renderer->GetRenderWindow()->Render();

                currentModel.isSTL = false;
                currentModel.currentActor = actor;
            }
        } else {
            QMessageBox msgbox;
            msgbox.setText("This file does not have the correct extension. It must be one of .mod or .stl.");
            msgbox.exec();
        }
    } catch (std::runtime_error &err) {
        QString msg = QString::fromUtf8(err.what());
        QMessageBox msgbox;
        msgbox.setText(msg);
        msgbox.exec();
    }
}

void MainWindow::handleChangeColour() {
    // TODO: Handle change current actor colour
    auto actor = currentModel.currentActor;
    bool isSTL = currentModel.isSTL;
    return;
}

void MainWindow::handleShrinkActor() {
    // TODO: Handle shrink filter
    auto sf = currentModel.shrinkFilter;
    return;
}

void MainWindow::handleChangeBkg() {
    // TODO: Handle change background colour
    return;
}

MainWindow::~MainWindow() {
    delete ui;
}
