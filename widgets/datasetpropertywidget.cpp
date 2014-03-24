#include "datasetpropertywidget.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include <QLabel>
#include <QSettings>
#include "knossos-global.h"
#include "knossos.h"
#include "GuiConstants.h"
#include "ftp.h"
#include "viewer.h"
#include "mainwindow.h"

extern  stateInfo *state;

DatasetPropertyWidget::DatasetPropertyWidget(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGridLayout *localLayout = new QGridLayout();

    localGroup = new QGroupBox("Local Dataset");

    datasetfileDialog = new QPushButton("Select Dataset Path");
    this->path = new QComboBox();
    this->path->setInsertPolicy(QComboBox::NoInsert);
    this->path->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    this->path->setEditable(true);
    supercubeEdgeSpin = new QSpinBox;
    supercubeEdgeSpin->setRange(3, 7);
    supercubeEdgeSpin->setSingleStep(2);
    supercubeEdgeSpin->setValue(state->M);
    cancelButton = new QPushButton("Cancel");
    processButton = new QPushButton("Use");

    QHBoxLayout * hLayout = new QHBoxLayout;
    hLayout->addWidget(path);
    hLayout->addWidget(datasetfileDialog);

    localLayout->addLayout(hLayout, 0, 0, 1, 2);
    localLayout->addWidget(new QLabel("Data cubes per dimension"), 2, 0);
    localLayout->addWidget(supercubeEdgeSpin, 2, 1);
    localLayout->addWidget(processButton, 3, 0);
    localLayout->addWidget(cancelButton, 3, 1);

    localGroup->setLayout(localLayout);
    mainLayout->addWidget(localGroup);

    setLayout(mainLayout);

    connect(this->datasetfileDialog, SIGNAL(clicked()), this, SLOT(datasetfileDialogClicked()));
    connect(this->cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
    connect(this->processButton, SIGNAL(clicked()), this, SLOT(processButtonClicked()));

    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
}

QStringList DatasetPropertyWidget::getRecentDirsItems() {
    QStringList recentDirs;
    int dirCount = this->path->count();
    for (int i = 0; i < dirCount; i++) {
        recentDirs.append(this->path->itemText(i));
    }
    return recentDirs;
}

void DatasetPropertyWidget::saveSettings()
{
    QSettings settings;
    settings.beginGroup(DATASET_WIDGET);
    settings.setValue(DATASET_MRU, getRecentDirsItems());
    settings.endGroup();
}

void DatasetPropertyWidget::loadSettings()
{
    QSettings settings;
    settings.beginGroup(DATASET_WIDGET);
    this->path->clear();
    this->path->insertItems(0, settings.value(DATASET_MRU).toStringList());
    settings.endGroup();
}

void DatasetPropertyWidget::datasetfileDialogClicked() {
    state->viewerState->renderInterval = SLOW;
    QApplication::processEvents();
    QString selectDir = QFileDialog::getExistingDirectory(this, "Select a knossos.conf", QDir::homePath());
    if(!selectDir.isNull()) {
        path->setEditText(selectDir);
    }
    state->viewerState->renderInterval = FAST;
}

void DatasetPropertyWidget::closeEvent(QCloseEvent *) {
    this->hide();
}

void DatasetPropertyWidget::waitForLoader() {
    emit startLoaderSignal();
    state->protectLoadSignal->lock();
    while (state->loaderBusy) {
        state->conditionLoadFinished->wait(state->protectLoadSignal);
    }
    state->protectLoadSignal->unlock();
}

void DatasetPropertyWidget::cancelButtonClicked() {
    this->hide();
}

void DatasetPropertyWidget::processButtonClicked() {
    changeDataSet(true);
}

#include <QApplication>
#include <QProcess>

void DatasetPropertyWidget::changeDataSet(bool isGUI) {
    QString dir = this->path->currentText();
    if(dir.isNull() || dir.isEmpty()) {
        if (isGUI) {
            QMessageBox info;
            info.setWindowFlags(Qt::WindowStaysOnTopHint);
            info.setIcon(QMessageBox::Information);
            info.setWindowTitle("Information");
            info.setText("No directory specified!");
            info.addButton(QMessageBox::Ok);
            info.exec();
        }
        return;
    }

    QString conf = QString(dir).append("/knossos.conf");
    QFile confFile(conf);
    if(!confFile.exists()) {
        if (isGUI) {
            QMessageBox info;
            info.setWindowFlags(Qt::WindowStaysOnTopHint);
            info.setIcon(QMessageBox::Information);
            info.setWindowTitle("Information");
            info.setText("There is no knossos.conf");
            info.addButton(QMessageBox::Ok);
            info.exec();
        }
        return;
    }

    int dirRecentIndex = this->getRecentDirsItems().indexOf(dir);
    if (-1 != dirRecentIndex) {
        this->path->removeItem(dirRecentIndex);
    }
    this->path->insertItem(0, dir);
    this->path->setCurrentIndex(0);

    // Note:
    // We clear the skeleton *before* reading the new config. In case we fail later, the skeleton would be nevertheless be gone.
    // This is a gamble we take, in order to not have possible bugs where the skeleton depends on old configuration values.
    if (isGUI) {
        emit clearSkeletonSignalGUI();
    } else {
        emit clearSkeletonSignalNoGUI();
    }

    // BUG BUG BUG
    // The following code, combined with the way loader::run in currently implemented
    // (revision 966) contains a minor timing issue that may result in a crash, namely
    // since loader::loadCubes begins executing in LM_LOCAL mode and ends in LM_FTP,
    // if at this point in the code we're in LM_LOCAL, and are about an FTP dataset
    // BUG BUG BUG

    state->loaderDummy = true;

    // Stupid userMove hack-around. In order to move somewhere, you have to currently be at another supercube.
    state->viewerState->currentPosition.x =
            state->viewerState->currentPosition.y =
            state->viewerState->currentPosition.z = 0;
    emit userMoveSignal(state->cubeEdgeLength, state->cubeEdgeLength, state->cubeEdgeLength, TELL_COORDINATE_CHANGE);

    this->waitForLoader();

    strcpy(state->path, dir.toStdString().c_str());

    if(false == Knossos::readConfigFile(conf.toStdString().c_str())) {
        QMessageBox info;
        info.setWindowFlags(Qt::WindowStaysOnTopHint);
        info.setIcon(QMessageBox::Information);
        info.setWindowTitle("Information");
        info.setText(QString("Failed to read config from %s").arg(conf));
        info.addButton(QMessageBox::Ok);
        info.exec();
        return;
    }

    knossos->commonInitStates();

    this->waitForLoader();

    emit changeDatasetMagSignal(DATA_SET);

    // Back to usual...
    state->loaderDummy = false;

    // ...beginning with loading the middle of dataset, as upon startup
    SET_COORDINATE(state->viewerState->currentPosition,
                   state->boundary.x / 2 - state->cubeEdgeLength,
                   state->boundary.y / 2 - state->cubeEdgeLength,
                   state->boundary.z / 2 - state->cubeEdgeLength);
    emit userMoveSignal(
                state->cubeEdgeLength,
                state->cubeEdgeLength,
                state->cubeEdgeLength,
                TELL_COORDINATE_CHANGE);
    // reset skeleton viewport
    if(state->skeletonState->rotationcounter == 0) {
        state->skeletonState->definedSkeletonVpView = SKELVP_RESET;
    }

    //Viewer::changeDatasetMag cannot be used when ther’re no other mags available
    //sets viewport settings according to current mag
    for(size_t i = 0; i < state->viewerState->numberViewports; i++) {
        if(state->viewerState->vpConfigs[i].type != VIEWPORT_SKELETON) {
            state->viewerState->vpConfigs[i].texture.zoomLevel = VPZOOMMIN;
            state->viewerState->vpConfigs[i].texture.texUnitsPerDataPx = 1. / TEXTURE_EDGE_LEN / state->magnification;
        }
    }

    emit datasetSwitchZoomDefaults();

    this->hide();
    if (isGUI) {
        //ideally one would use qApp->quit(), but the cleanup steps are not connected to this
        static_cast<MainWindow*>(parent())->close();//call Knossos cleanup func
        auto args = qApp->arguments();
        args.append(QString("--supercube-edge=%0").arg(supercubeEdgeSpin->value()));
        qDebug() << args;
        QProcess::startDetached(qApp->arguments()[0], args);
    }
}
