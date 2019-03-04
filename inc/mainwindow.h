#pragma once

#include <QMainWindow>
#include <vtkLight.h>
#include <vtkLightActor.h>

#include "LegacyLoader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
