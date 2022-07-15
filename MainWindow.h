#pragma once

#include "ui_MainWindow.h"
#include "Calculation.h"
#include "Surface.h"

#include <QMainWindow>
#include <QtDataVisualization/Q3DSurface>
#include <QStackedWidget>
#include <QFileDialog>
#include <QProcess>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Calculation* GetCalc();
    QtDataVisualization::Q3DSurface* GetSurf();
    QWidget* GetContainer();

    void SetCalc(Calculation* calculation);
    void SetSurf(QtDataVisualization::Q3DSurface* surface);
    void SetContainer(QWidget* container);

    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
	Ui::MainWindow* _ui;
	QString _fullName;
	Calculation* _calculation = nullptr;
	QtDataVisualization::Q3DSurface* _surface = nullptr;
	QWidget* _container = nullptr;

private slots:
    void NextPage();
    void PreviousPage();
    void RecButtonClicked();
    void CylButtonClicked();
    void OpenFile();
    void CalculateButtonClicked();
    void SaveButtonClicked();
    void ClearButtonClicked();
};
