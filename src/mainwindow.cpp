#include "mainwindow.h"
#include "ui_mainwindow.h"

// Handle mouse events
class MouseInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static MouseInteractorStyle* New();

    MouseInteractorStyle(){
        selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        selectedActor = vtkSmartPointer<vtkActor>::New();
    }

vtkTypeMacro(MouseInteractorStyle, vtkInteractorStyleTrackballCamera);

    virtual void OnLeftButtonDown()
    {
        // Get the location of the click (in window coordinates)
        int* pos = this->GetInteractor()->GetEventPosition();

        vtkSmartPointer<vtkCellPicker> picker =
                vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.001);

        // Pick from this location.
        picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

        double* worldPosition = picker->GetPickPosition();
        std::cout << "Cell id is: " << picker->GetCellId() << std::endl;
        std::cout << "Actor is: " << picker->GetActor() << std::endl;

        if(picker->GetCellId() != -1)
        {
            std::cout << "Pick position is: " << worldPosition[0] << " " << worldPosition[1]
                      << " " << worldPosition[2] << endl;

            vtkSmartPointer<vtkIdTypeArray> ids =
                    vtkSmartPointer<vtkIdTypeArray>::New();
            ids->SetNumberOfComponents(1);
            ids->InsertNextValue(picker->GetCellId());

            vtkSmartPointer<vtkSelectionNode> selectionNode =
                    vtkSmartPointer<vtkSelectionNode>::New();
            selectionNode->SetFieldType(vtkSelectionNode::CELL);
            selectionNode->SetContentType(vtkSelectionNode::INDICES);
            selectionNode->SetSelectionList(ids);

            vtkSmartPointer<vtkSelection> selection =
                    vtkSmartPointer<vtkSelection>::New();
            selection->AddNode(selectionNode);

            vtkSmartPointer<vtkExtractSelection> extractSelection =
                    vtkSmartPointer<vtkExtractSelection>::New();
            extractSelection->SetInputData(0, this->Data);
            extractSelection->SetInputData(1, selection);
            extractSelection->Update();

            // In selection
            vtkSmartPointer<vtkUnstructuredGrid> selected =
                    vtkSmartPointer<vtkUnstructuredGrid>::New();
            selected->ShallowCopy(extractSelection->GetOutput());

            std::cout << "There are " << selected->GetNumberOfPoints()
                      << " points in the selection." << std::endl;
            std::cout << "There are " << selected->GetNumberOfCells()
                      << " cells in the selection." << std::endl;
            vtkSmartPointer<vtkPropPicker> picker =
                    vtkSmartPointer<vtkPropPicker>::New();
            picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

            selectedMapper->SetInputData(selected);
            selectedActor->SetMapper(selectedMapper);
            selectedActor->GetProperty()->EdgeVisibilityOn();
            selectedActor->GetProperty()->SetEdgeColor(1,0,0);
            selectedActor->GetProperty()->SetLineWidth(3);

            this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(selectedActor);
        }

        // Forward events
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

    vtkSmartPointer<vtkPolyData> Data;
    vtkSmartPointer<vtkDataSetMapper> selectedMapper;
    vtkSmartPointer<vtkActor> selectedActor;
};

vtkStandardNewMacro(MouseInteractorStyle);

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    QWidget::setWindowIcon( QIcon(":/Icons/modelViewer.ico"));
    ui->setupUi(this);


    //Add CTRL+O shortcut for opening files
    QAction *quickOpen = new QAction(this);
    quickOpen->setShortcut(Qt::CTRL | Qt::Key_O);
    connect(quickOpen, SIGNAL(triggered()), this, SLOT(handleFileOpen()));
    this->addAction(quickOpen);

    // Standard call to setup Qt UI (same as previously);
    renderer = vtkSmartPointer<vtkRenderer>::New();

    //connect slots
    connect(ui->actionFileOpen, &QAction::triggered, this, &MainWindow::handleFileOpen);
    /*connect(ui->actionFileSave, &QAction::triggered, this, &MainWindow::handleFileSave);
    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::handleHelp);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::handlePrint);*/

    // Create a VTK render window and link it to the QtVTK widget
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

    // Create a renderer, and render window
    ui->qvtkWidget->GetRenderWindow()->AddRenderer(
            renderer);
    vtkSmartPointer<QVTKInteractor> qvtkInteractor = vtkSmartPointer<QVTKInteractor>::New();
    qvtkInteractor->SetRenderWindow(renderer->GetRenderWindow());
    qvtkInteractor->Initialize();

    // Set the custom stype to use for interaction.
    vtkSmartPointer<MouseInteractorStyle> style =
            vtkSmartPointer<MouseInteractorStyle>::New();
    style->SetDefaultRenderer(renderer);


    qvtkInteractor->SetInteractorStyle(style);

    renderer->SetBackground(colors->GetColor3d("BkgColor").GetData());

    // Setup the renderers's camera
    renderer->ResetCamera();
    }

void MainWindow::handleFileOpen() {
    try {
        std::string filename = QFileDialog::getOpenFileName(this, tr("Open Model"), tr(""), tr("Model Files (*.mod *.stl)")).toStdString();
        if (filename.find_last_of(".") != std::string::npos) {
            std::string ext = filename.substr(filename.find_last_of(".") + 1);
            vtkSmartPointer<vtkActor> actor;
            if (ext == "stl") {
                vtkSmartPointer<vtkSTLReader> stlReader =
                        vtkSmartPointer<vtkSTLReader>::New();
                stlReader->SetFileName(filename.c_str());
                stlReader->Update();

                auto shrink = currentModel.shrinkFilter;
                shrink->SetInputData(stlReader->GetOutput());
                shrink->SetShrinkFactor(1);
                shrink->Update();

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

                vtkSmartPointer<vtkGeometryFilter> extract = vtkSmartPointer<vtkGeometryFilter>::New();
                extract->SetInputData(data);
                extract->Update();

                auto shrink = currentModel.shrinkFilter;
                shrink->SetInputData(extract->GetOutput());
                shrink->SetShrinkFactor(1);
                shrink->Update();

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
            // TODO: Add the actor's data to vtkMergeCells

            auto PickerMerge = vtkSmartPointer<vtkMergeCells>::New();

            auto CopyGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
            CopyGrid->DeepCopy(PickerGrid);
            PickerGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
            PickerMerge->SetUnstructuredGrid(PickerGrid);

            auto ActorData = actor->GetMapper()->GetInput();
            PickerMerge->SetTotalNumberOfDataSets(2);
            TotalCells += ActorData->GetNumberOfCells();
            TotalPoints += ActorData->GetNumberOfPoints();
            PickerMerge->SetTotalNumberOfCells(TotalCells);
            PickerMerge->SetTotalNumberOfPoints(TotalPoints);
            PickerMerge->MergeDataSet(ActorData);
            PickerMerge->MergeDataSet(CopyGrid);
            PickerMerge->Finish();


            renderer->AddActor(actor);
            renderer->ResetCamera();
            renderer->GetRenderWindow()->Render();
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

void MainWindow::handleFileSave() {
    vtkSmartPointer<vtkSTLWriter> stlWriter =
            vtkSmartPointer<vtkSTLWriter>::New();
    std::string filename = QFileDialog::getOpenFileName(this, tr("Save Model"), tr(""), tr("STL Files (*.stl)")).toStdString();
    if (filename.substr(filename.find_last_of(".")) == ".stl") {}
    else{
        filename.append(".stl");
    }

    stlWriter->SetFileName(filename.c_str());
    stlWriter->GetInput();
    stlWriter->Write();
}



QVTKInteractor* MainWindow::GetInteractor(){
    return QVTKInteractor::SafeDownCast(renderer->GetRenderWindow()->GetInteractor());
}

MainWindow::~MainWindow() {
    delete ui;
}


