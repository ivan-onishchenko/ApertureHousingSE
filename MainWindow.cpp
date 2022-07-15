#include "MainWindow.h"

using namespace QtDataVisualization;

Calculation* MainWindow::GetCalc()
{
    return this->_calculation;
}

Q3DSurface* MainWindow::GetSurf()
{
    return this->_surface;
}

QWidget* MainWindow::GetContainer()
{
    return this->_container;
}

void MainWindow::SetCalc(Calculation* calculation)
{
    this->_calculation = calculation;
}

void MainWindow::SetSurf(QtDataVisualization::Q3DSurface* surface)
{
    this->_surface = surface;
}

void MainWindow::SetContainer(QWidget* container)
{
    this->_container = container;
}

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    qApp->setStyleSheet("QPushButton {"
                        "background-color: #e6e6e6;"
                        "border-style: outset;"
                        "border-width: 1px;"
                        "border-radius: 6px;"
                        "border-color: #000000; }"

                        "QPushButton:pressed {"
                        "background-color: #d2d2d2 }"

                        "QPushButton:subcontrol-position {"
                        "background-color: #000000;"
                        "border-style: inset; }"

                        "QPushButton:checked {"
                        "background-color: #d2d2d2 }"

                        "QTextEdit#aValue, QTextEdit#bValue, QTextEdit#dValue, QTextEdit#lValue, QTextEdit#dValue_2,"
                        "QTextEdit#minPValue, QTextEdit#maxPValue, QTextEdit#ptsNumValue {"
                        "border-style: outset;"
                        "border-width: 1px;"
                        "border-color: #000000; }");

    int w, h;
    QPixmap recPic(":/images/resources/rectangle.png");
    w = _ui->recPic->width();
    h = _ui->recPic->height();
    _ui->recPic->setPixmap(recPic.scaled(w, h, Qt::KeepAspectRatio));

    QPixmap cylPic(":/images/resources/cylinder.png");
    w = _ui->cylPic->width();
    h = _ui->cylPic->height();
    _ui->cylPic->setPixmap(cylPic.scaled(w, h, Qt::KeepAspectRatio));

    _ui->freqMultValue->setFixedSize(55, 25);
    _ui->vSpacer_2_3->changeSize(20, 30);
    _ui->vSpacer_3_2->changeSize(20, 10);
    _ui->vSpacer_4_2->changeSize(20, 10);

    _ui->nextButton->setEnabled(false);
    _ui->previousButton->hide();
    _ui->recButton->setCheckable(true);
    _ui->cylButton->setCheckable(true);

    connect(_ui->nextButton, SIGNAL(clicked()), this, SLOT(NextPage()));
    connect(_ui->previousButton, SIGNAL(clicked()), this, SLOT(PreviousPage()));
    connect(_ui->recButton, SIGNAL(clicked()), this, SLOT(RecButtonClicked()));
    connect(_ui->cylButton, SIGNAL(clicked()), this, SLOT(CylButtonClicked()));
    connect(_ui->openFileButton, SIGNAL(clicked()), this, SLOT(OpenFile()));
    connect(_ui->calcButton, SIGNAL(clicked()), this, SLOT(CalculateButtonClicked()));
    connect(_ui->clearButton, SIGNAL(clicked()), this, SLOT(ClearButtonClicked()));
    connect(_ui->saveButton, SIGNAL(clicked()), this, SLOT(SaveButtonClicked()));

    this->SetSurf(new Q3DSurface);
    this->SetContainer(QWidget::createWindowContainer(this->GetSurf()));
    Surface::BuildScene(this->GetSurf());
    _ui->vLayout_5_1->addWidget(this->GetContainer(), 1);
}

MainWindow::~MainWindow()
{
    delete this->_ui;
    delete this->_calculation;
    delete this->_container;
}

void MainWindow::RecButtonClicked()
{
    if (_ui->recButton->isChecked())
    {
        _ui->nextButton->setEnabled(true);
    }
    else
    {
        _ui->nextButton->setEnabled(false);
    }

    if (_ui->cylButton->isChecked())
    {
        _ui->cylButton->setChecked(false);
    }
}

void MainWindow::CylButtonClicked()
{
    if (_ui->cylButton->isChecked())
    {
        _ui->nextButton->setEnabled(true);
    }
    else
    {
        _ui->nextButton->setEnabled(false);
    }

    if (_ui->recButton->isChecked())
    {
        _ui->recButton->setChecked(false);
    }
}

void MainWindow::NextPage()
{
    if (_ui->stackedWidget->currentIndex() == 0)
    {
        _ui->previousButton->show();
        _ui->nextButton->setEnabled(false);
        if (MainWindow::_fullName != 0)
        {
            _ui->nextButton->setEnabled(true);
        }

    }

    if (_ui->stackedWidget->currentIndex() == 1 && _ui->cylButton->isChecked())
    {
        _ui->stackedWidget->setCurrentIndex(_ui->stackedWidget->currentIndex() + 1);
    }

    if (_ui->stackedWidget->currentIndex() == 2 && _ui->recButton->isChecked())
    {
        if (_ui->aValue->toPlainText().isEmpty() || _ui->bValue->toPlainText().isEmpty() || _ui->dValue->toPlainText().isEmpty())
        {
            if (_ui->aValue->toPlainText().isEmpty())
            {
                _ui->aValue->setStyleSheet("* { border-color: #ff0000; }");
            }
            if (_ui->bValue->toPlainText().isEmpty())
            {
                _ui->bValue->setStyleSheet("* { border-color: #ff0000; }");
            }
            if (_ui->dValue->toPlainText().isEmpty())
            {
                _ui->dValue->setStyleSheet("* { border-color: #ff0000; }");
            }
            return;
        }
        _ui->aValue->setStyleSheet("QTextEdit { border-color: #000000 }");
        _ui->bValue->setStyleSheet("QTextEdit { border-color: #000000 }");
        _ui->dValue->setStyleSheet("QTextEdit { border-color: #000000 }");
        _ui->stackedWidget->setCurrentIndex(_ui->stackedWidget->currentIndex() + 1);
    }

    if (_ui->stackedWidget->currentIndex() == 3 && _ui->cylButton->isChecked())
    {
        if (_ui->lValue->toPlainText().isEmpty() || _ui->dValue_2->toPlainText().isEmpty())
        {
            if (_ui->lValue->toPlainText().isEmpty())
            {
                _ui->lValue->setStyleSheet("* { border-color: #ff0000; }");
            }
            if (_ui->dValue_2->toPlainText().isEmpty())
            {
                _ui->dValue_2->setStyleSheet("* { border-color: #ff0000; }");
            }
            return;
        }
        _ui->lValue->setStyleSheet("QTextEdit { border-color: #000000 }");
        _ui->dValue_2->setStyleSheet("QTextEdit { border-color: #000000 }");
    }

    if (_ui->stackedWidget->currentIndex() == 4)
    {
        if (_ui->minPValue->toPlainText().isEmpty() || _ui->maxPValue->toPlainText().isEmpty() || _ui->ptsNumValue->toPlainText().isEmpty())
        {
            if (_ui->minPValue->toPlainText().isEmpty())
            {
                _ui->minPValue->setStyleSheet("* { border-color: #ff0000; }");
            }
            if (_ui->maxPValue->toPlainText().isEmpty())
            {
                _ui->maxPValue->setStyleSheet("* { border-color: #ff0000; }");
            }
            if (_ui->ptsNumValue->toPlainText().isEmpty())
            {
                _ui->ptsNumValue->setStyleSheet("* { border-color: #ff0000; }");
            }
            return;
        }
        _ui->minPValue->setStyleSheet("QTextEdit { border-color: #000000 }");
        _ui->maxPValue->setStyleSheet("QTextEdit { border-color: #000000 }");
        _ui->ptsNumValue->setStyleSheet("QTextEdit { border-color: #000000 }");
        _ui->nextButton->setText(("Restart"));
    }

    if (_ui->stackedWidget->currentIndex() == 5)
    {
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }

    _ui->stackedWidget->setCurrentIndex(_ui->stackedWidget->currentIndex() + 1);
}

void MainWindow::PreviousPage()
{
    if (_ui->stackedWidget->currentIndex() == 1)
    {
        _ui->nextButton->setEnabled(true);
        _ui->previousButton->hide();
    }

    if (_ui->stackedWidget->currentIndex() == 3)
    {
        _ui->stackedWidget->setCurrentIndex(_ui->stackedWidget->currentIndex() - 1);
    }


    if (_ui->stackedWidget->currentIndex() == 4 && _ui->recButton->isChecked())
    {
        _ui->stackedWidget->setCurrentIndex(_ui->stackedWidget->currentIndex() - 1);
    }

    if (_ui->stackedWidget->currentIndex() == 5)
    {
        _ui->nextButton->setText(("Next >"));
    }

    _ui->stackedWidget->setCurrentIndex(_ui->stackedWidget->currentIndex() - 1);
}

void MainWindow::OpenFile()
{
    MainWindow::_fullName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt)"));
    if (MainWindow::_fullName != "")
    {
        QString fileName, fileDirectory;
        int counter = 0;
        for (int i = MainWindow::_fullName.length() - 1; i >= 0; i--)
        {
            if (MainWindow::_fullName[i] == '/')
				break;
            else
				counter++;
        }
        fileName = MainWindow::_fullName.right(counter);
        fileDirectory = MainWindow::_fullName.left(MainWindow::_fullName.length() - counter);
        _ui->fileDirectory->setText(fileDirectory + "...");
        _ui->fileName->setText(fileName);
        _ui->nextButton->setEnabled(true);
    }
}

void MainWindow::CalculateButtonClicked()
{
    this->SetCalc(new Calculation);
    if(_ui->recButton->isChecked())
    {
        this->GetCalc()->RecCalc(MainWindow::_fullName, _ui->freqMultValue,
                                 _ui->aValue->toPlainText(), _ui->bValue->toPlainText(), _ui->dValue->toPlainText(),
                                 _ui->minPValue->toPlainText(), _ui->maxPValue->toPlainText(), _ui->ptsNumValue->toPlainText());
    }
    else if(_ui->cylButton->isChecked())
    {
        this->GetCalc()->CylCalc(MainWindow::_fullName, _ui->freqMultValue,
                                 _ui->dValue_2->toPlainText(), _ui->lValue->toPlainText(),
                                 _ui->minPValue->toPlainText(), _ui->maxPValue->toPlainText(), _ui->ptsNumValue->toPlainText());
    }
    Surface::BuildSurface(this->GetSurf(), this->GetCalc()->GetPtsNum(), this->GetCalc()->GetStrNum(),
                          this->GetCalc()->GetF(), this->GetCalc()->GetP(), this->GetCalc()->GetSE());
    _ui->vLayout_5_1->update();
    _ui->calcButton->setEnabled(false);
}

void MainWindow::SaveButtonClicked()
{
    MainWindow::_fullName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text Files (*.txt)"));
    this->GetCalc()->SaveCalc(_fullName);
}

void MainWindow::ClearButtonClicked()
{
    _ui->vLayout_5_1->removeWidget(this->GetContainer());
    this->SetSurf(new Q3DSurface);
    this->SetContainer(QWidget::createWindowContainer(this->GetSurf()));
    Surface::BuildScene(this->GetSurf());
    _ui->vLayout_5_1->addWidget(this->GetContainer(), 1);
    _ui->calcButton->setEnabled(true);
}
