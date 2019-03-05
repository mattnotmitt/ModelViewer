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

    // Now use the VTK libraries to render something. To start with you can copy-paste the cube example code, there are comments to show where the code must be modified.
    //--------------------------------------------- Code From Example 1 --------------------------------------------------------------------------
    // Create a cube using a vtkCubeSource
    vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();

    // Create a mapper that will hold the cube's geometry in a format suitable for rendering
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputConnection(cubeSource->GetOutputPort());

    // Create an actor that is used to set the cube's properties for rendering and place it in the window
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->EdgeVisibilityOn();

    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    std::array<unsigned char, 4> bkg{{51, 77, 102, 255}};
    colors->SetColor("BkgColor", bkg.data());
    actor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());

    // Create a renderer, and render window
    //vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();		// ###### We've already created the renderWindow this time ######
    ui->qvtkWidget->GetRenderWindow()->AddRenderer(
            renderer);                                    // ###### ask the QtVTKOpenGLWidget for its renderWindow ######

    // Link a renderWindowInteractor to the renderer (this allows you to capture mouse movements etc)  ###### Not needed with Qt ######
    //vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    //renderWindowInteractor->SetRenderWindow( ui->vtkWidget->GetRenderWindow() );

    // Add the actor to the scene

    //renderer->AddActor(legacyActor2);
    //renderer->AddActor(actor);
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

                // Visualize
                vtkSmartPointer<vtkPolyDataMapper> stlMapper =
                        vtkSmartPointer<vtkPolyDataMapper>::New();
                stlMapper->SetInputConnection(stlReader->GetOutputPort());

                vtkSmartPointer<vtkActor> stlActor =
                        vtkSmartPointer<vtkActor>::New();
                stlActor->SetMapper(stlMapper);
                renderer->AddActor(stlActor);
                renderer->ResetCamera();
                renderer->GetRenderWindow()->Render();
            } else if (ext == "mod") {
                renderer->AddActor(loader.loadModel(filename));
                renderer->ResetCamera();
                renderer->GetRenderWindow()->Render();
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

MainWindow::~MainWindow() {
    delete ui;
}
