#include "mainwindow.h"
#include "ui_mainwindow.h"

/* Disabled due to non-functionality
MouseInteractorStyle::MouseInteractorStyle() {
    selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    selectedActor = vtkSmartPointer<vtkActor>::New();
}

void MouseInteractorStyle::OnLeftButtonDown() {
    // Get the location of the click (in window coordinates)
    int *pos = this->GetInteractor()->GetEventPosition();

    vtkSmartPointer<vtkCellPicker> picker =
            vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.001);

    // Pick from this location.
    picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

    double *worldPosition = picker->GetPickPosition();
    std::cout << "Cell id is: " << picker->GetCellId() << std::endl;
    std::cout << "Actor is: " << picker->GetActor() << std::endl;

    if (picker->GetCellId() != -1) {
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
        selectedActor->GetProperty()->SetEdgeColor(1, 0, 0);
        selectedActor->GetProperty()->SetLineWidth(3);

        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(selectedActor);
    }

    // Forward events
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}*/

//vtkStandardNewMacro(MouseInteractorStyle);

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    QWidget::setWindowIcon(QIcon(":/Icons/modelViewer.ico"));
    ui->setupUi(this);


    //Add CTRL+O shortcut for opening files
    auto quickOpen = new QAction(this);
    quickOpen->setShortcut(Qt::CTRL | Qt::Key_O);
    connect(quickOpen, SIGNAL(triggered()), this, SLOT(handleFileOpen()));
    this->addAction(quickOpen);

    //Add CTRL+O shortcut for opening files
    auto quickSave = new QAction(this);
    quickSave->setShortcut(Qt::CTRL | Qt::Key_S);
    connect(quickSave, SIGNAL(triggered()), this, SLOT(handleFileSave()));
    this->addAction(quickSave);

    // Standard call to setup Qt UI (same as previously);
    renderer = vtkSmartPointer<vtkRenderer>::New();

    //connect slots
    connect(ui->actionFileOpen, &QAction::triggered, this, &MainWindow::handleFileOpen);
    connect(ui->actionFileSave, &QAction::triggered, this, &MainWindow::handleFileSave);
    connect(ui->actionChangeColour, &QAction::triggered, this, &MainWindow::handleChangeColour);
    connect(ui->actionChangeBkg, &QAction::triggered, this, &MainWindow::handleChangeBkg);
    connect(ui->actionResetCamera, &QAction::triggered, this, &MainWindow::handleCameraReset);
    connect(ui->actionResetFilters, &QAction::triggered, this, &MainWindow::handleFilterReset);

    connect(ui->shrinkSlider, &QSlider::valueChanged, this, &MainWindow::handleShrinkActor);
    connect(ui->modelColour, &QPushButton::clicked, this, &MainWindow::handleChangeColour);
    connect(ui->bkgColour, &QPushButton::clicked, this, &MainWindow::handleChangeBkg);

    // Create a VTK render window and link it to the QtVTK widget
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    ui->qvtkWidget->SetRenderWindow(
            renderWindow);            // note that vtkWidget is the name I gave to my QtVTKOpenGLWidget in Qt creator

    // Set background colour
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    std::array<unsigned char, 4> bkg{{51, 77, 102, 255}};
    colors->SetColor("BkgColor", bkg.data());

    // Create a renderer, and render window
    ui->qvtkWidget->GetRenderWindow()->AddRenderer(
            renderer);
    /*
    vtkSmartPointer<QVTKInteractor> qvtkInteractor = vtkSmartPointer<QVTKInteractor>::New();
    qvtkInteractor->SetRenderWindow(renderer->GetRenderWindow());
    qvtkInteractor->Initialize();
    // Set the custom stype to use for interaction.
    vtkSmartPointer<MouseInteractorStyle> style =
            vtkSmartPointer<MouseInteractorStyle>::New();
    style->Data = currentModel.currentData;
    style->SetDefaultRenderer(renderer);
    qvtkInteractor->SetInteractorStyle(style);*/

    renderer->SetBackground(colors->GetColor3d("BkgColor").GetData());

    // Setup the renderers's camera
    renderer->ResetCamera();
}

void MainWindow::handleFileOpen() {
    try {
        std::string filename = QFileDialog::getOpenFileName(this, tr("Open Model"), tr(""),
                                                            tr("Model Files (*.mod *.stl)")).toStdString();
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
                currentModel.currentData = shrink->GetOutput();
                currentModel.currentActor = stlActor;
            } else if (ext == "mod") {
                auto data = loader.loadModel(filename);
                // Visualize.

                vtkSmartPointer<vtkGeometryFilter> extract =
                        vtkSmartPointer<vtkGeometryFilter>::New();
                extract->SetInputData(data.loadedModel);
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
                currentModel.currentData = shrink->GetOutput();
                currentModel.currentActor = actor;
                currentModel.currentMetadata = data.modelData;
            }
            ui->actionFileSave->setEnabled(!currentModel.isSTL);
            ui->actionChangeColour->setEnabled(currentModel.isSTL);
            ui->modelColour->setEnabled(currentModel.isSTL);
            ui->shrinkSlider->setEnabled(true);
            updateActorStats();

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
    auto actor = currentModel.currentActor;
    auto currColor = actor->GetProperty()->GetColor();
    currentModel.color = QColorDialog::getColor(QColor((int) currColor[0] * 255, (int) currColor[1] * 255, (int) currColor[2] * 255));
    actor->GetProperty()->SetColor(currentModel.color.red() / 255.0, currentModel.color.green() / 255.0, currentModel.color.blue() / 255.0);
    renderer->GetRenderWindow()->Render();
    updateActorStats();
}

void MainWindow::handleShrinkActor(int value) {
    auto sf = currentModel.shrinkFilter;
    sf->SetShrinkFactor(((float) value) / 100.0);
    sf->Update();
    renderer->GetRenderWindow()->Render();
}

void MainWindow::handleChangeBkg() {
    auto currColor = renderer->GetBackground();
    auto color = QColorDialog::getColor(QColor((int) (currColor[0] * 255), (int) (currColor[1] * 255), (int) (currColor[2] * 255)));
    renderer->SetBackground(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0);
    renderer->GetRenderWindow()->Render();
}

void MainWindow::handleFileSave() {
    vtkSmartPointer<vtkSTLWriter> stlWriter =
            vtkSmartPointer<vtkSTLWriter>::New();
    std::string filename = QFileDialog::getSaveFileName(this, tr("Save Model"), tr(""),
                                                        tr("STL Files (*.stl)")).toStdString();
    if (".stl" != filename.substr((filename.find_last_of('.') == std::string::npos) ? 0 : filename.find_last_of('.'))) {
        filename.append(".stl");
    }

    stlWriter->SetFileName(filename.c_str());
    stlWriter->SetInputData(currentModel.currentData);
    stlWriter->Write();
}

void MainWindow::handleCameraReset () {
    renderer->ResetCamera();
}

void MainWindow::handleFilterReset () {
    currentModel.shrinkFilter->SetShrinkFactor(1);
    currentModel.shrinkFilter->Update();
    renderer->GetRenderWindow()->Render();
}

/*
QVTKInteractor *MainWindow::GetInteractor() {
    return QVTKInteractor::SafeDownCast(renderer->GetRenderWindow()->GetInteractor());
}*/

void MainWindow::updateActorStats() {
    auto table = ui->statsTable;
    if (currentModel.isSTL) {
        table->setRowCount(1);
        table->setItem(0, 0, new QTableWidgetItem(tr("Colour")));
        table->setItem(0, 1, new QTableWidgetItem(currentModel.color.name()));
    } else {
        auto cog = currentModel.currentMetadata.calcCentreOfGravity();
        table->setRowCount(6);
        table->setColumnCount(2);
        table->setItem(0, 0, new QTableWidgetItem(tr("Cells")));
        table->setItem(0, 1, new QTableWidgetItem(QString::number(currentModel.currentMetadata.getCells().size())));
        table->setItem(1, 0, new QTableWidgetItem(tr("Centre of Gravity")));
        table->setItem(1, 1, new QTableWidgetItem(QString::asprintf("[%.4f, %.4f, %.4f]", cog.getX(), cog.getY(), cog.getZ())));
        table->setItem(2, 0, new QTableWidgetItem(tr("Colour")));
        table->setItem(2, 1, new QTableWidgetItem(tr("Custom")));
        table->setItem(3, 0, new QTableWidgetItem(tr("Vectors")));
        table->setItem(3, 1, new QTableWidgetItem(QString::number(currentModel.currentMetadata.getVertices().size())));
        table->setItem(4, 0, new QTableWidgetItem(tr("Volume")));
        table->setItem(4, 1, new QTableWidgetItem(QString::number(currentModel.currentMetadata.calcVolume())));
        table->setItem(5, 0, new QTableWidgetItem(tr("Weight")));
        table->setItem(5, 1, new QTableWidgetItem(QString::number(currentModel.currentMetadata.calcWeight())));
    }
}

MainWindow::~MainWindow() {
    delete ui;
}


