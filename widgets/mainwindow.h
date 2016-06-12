#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*
 *  This file is a part of KNOSSOS.
 *
 *  (C) Copyright 2007-2013
 *  Max-Planck-Gesellschaft zur Foerderung der Wissenschaften e.V.
 *
 *  KNOSSOS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 of
 *  the License as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * For further information, visit http://www.knossostool.org or contact
 *     Joergen.Kornfeld@mpimf-heidelberg.mpg.de or
 *     Fabian.Svara@mpimf-heidelberg.mpg.de
 */
#include "scriptengine/proxies/skeletonproxy.h"
#include "session.h"
#include "viewport.h"
#include "widgetcontainer.h"

#define FILE_DIALOG_HISTORY_MAX_ENTRIES 10

#include <QComboBox>
#include <QDropEvent>
#include <QList>
#include <QMenu>
#include <QMainWindow>
#include <QToolBar>
#include <QUndoStack>

#include <array>
#include <memory>

class QLabel;
class QToolButton;
class QPushButton;
class QSpinBox;
class QCheckBox;
class QMessageBox;
class QGridLayout;
class QFile;

enum class SynapseState {
    Off, SynapticCleft, PostSynapse
};

enum class SegmentState {
    On, Off, Off_Once
};

class WorkModeModel : public QAbstractListModel {
    Q_OBJECT
    std::vector<std::pair<AnnotationMode, QString>> workModes;
public:
    virtual int rowCount(const QModelIndex &) const override {
        return workModes.size();
    }
    virtual QVariant data(const QModelIndex & index, int role) const override {
        if (role == Qt::DisplayRole) {
            return workModes[index.row()].second;
        }
        return QVariant();
    }
    void recreate(const std::map<AnnotationMode, QString> & modes) {
        beginResetModel();
        workModes.clear();
        for (const auto & mode : modes) {
            workModes.emplace_back(mode);
        }
        std::sort(std::begin(workModes), std::end(workModes), [](const std::pair<AnnotationMode, QString> elemA, const std::pair<AnnotationMode, QString> elemB) {
            return elemA.second < elemB.second;
        });
        for (uint i = 1; i <= modes.size(); ++i) {
            workModes[i - 1].second = tr("%1. ").arg(i) + workModes[i -1].second;
        }
        endResetModel();
    }

    std::pair<AnnotationMode, QString> at(const int index) const {
        return workModes[index];
    }

    int indexOf(const AnnotationMode & mode) const {
        for (uint i = 0; i < workModes.size(); ++i) {
            if (workModes[i].first == mode) {
                return i;
            }
        }
        return -1;
    }
};

class MainWindow : public QMainWindow {
    Q_OBJECT
    friend class TaskManagementWidget;
    friend class SkeletonProxy;

    QToolBar basicToolbar{"Basic Functionality"};
    QToolBar defaultToolbar{"Tools"};

    std::map<AnnotationMode, QString> workModes{ {AnnotationMode::Mode_MergeTracing, tr("Merge Tracing")},
                                                 {AnnotationMode::Mode_Tracing, tr("Tracing")},
                                                 {AnnotationMode::Mode_TracingAdvanced, tr("Tracing Advanced")},
                                                 {AnnotationMode::Mode_Merge, tr("Segmentation Merge")},
                                                 {AnnotationMode::Mode_Paint, tr("Segmentation Paint")},
                                               };
    WorkModeModel workModeModel;
    QComboBox modeCombo;
    QAction *toggleSegmentsAction;
    QAction *newTreeAction;
    QAction *pushBranchAction;
    QAction *popBranchAction;
    QAction *clearSkeletonAction;
    QAction *clearMergelistAction;

    void resizeEvent(QResizeEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void resizeToFitViewports(int width, int height);

    QSpinBox *xField, *yField, *zField;
    QMenu fileMenu{"File"};
    QMenu *segEditMenu;
    QMenu *skelEditMenu;
    QMenu actionMenu{"Action"};
    QMenu *pluginMenu;
    QString openFileDirectory;
    QString saveFileDirectory;

    std::vector<QAction*> commentActions;

    int loaderLastProgress;
    QLabel *loaderProgress;

    // segmentation job mode
    QToolBar segJobModeToolbar{"Job Navigation"};
    QLabel todosLeftLabel{"<font color='green'>  0 more left</font>"};
    void updateTodosLeft();

    void placeComment(const int index);
    void toggleSegments();
public:
    std::unique_ptr<ViewportOrtho> viewportXY;
    std::unique_ptr<ViewportOrtho> viewportXZ;
    std::unique_ptr<ViewportOrtho> viewportZY;
    std::unique_ptr<ViewportArb> viewportArb;
    std::unique_ptr<Viewport3D> viewport3D;

    // contains all widgets
    WidgetContainer widgetContainer;

    std::array<QAction*, FILE_DIALOG_HISTORY_MAX_ENTRIES> historyEntryActions;

    // convenience mode switch actions for proof reading mode
    QAction *modeSwitchSeparator{nullptr};
    QAction *setMergeModeAction{nullptr};
    QAction *setPaintModeAction{nullptr};

    QAction *segEditSegModeAction;
    QAction *segEditSkelModeAction;
    QAction *skelEditSegModeAction;
    QAction *skelEditSkelModeAction;
    QAction *addNodeAction;
    QAction *linkWithActiveNodeAction;
    QAction *dropNodesAction;

    QList<QString> skeletonFileHistory;
    QFile *loadedFile;

    SkeletonProxy *skeletonProxy;

    QLabel GUIModeLabel{""};
    QLabel cursorPositionLabel;
    QLabel nodeLockingLabel;
    QLabel synapseStateLabel;
    QLabel segmentStateLabel;
    QLabel unsavedChangesLabel;
    QLabel annotationTimeLabel;

    void createViewports();

    // for creating action, menus and the toolbar
    void createMenus();
    void createToolbars();

    // for save, load and clear settings
    void saveSettings();
    void loadSettings();
    void clearSettings();

    explicit MainWindow(QWidget *parent = 0);
    template<typename Function> void forEachVPDo(Function func) {
        for (auto * vp : { static_cast<ViewportBase *>(viewportXY.get()), static_cast<ViewportBase *>(viewportXZ.get()), static_cast<ViewportBase *>(viewportZY.get()), static_cast<ViewportBase *>(viewportArb.get()), static_cast<ViewportBase *>(viewport3D.get()) }) {
            func(*vp);
        }
    }
    template<typename Function> void forEachOrthoVPDo(Function func) {
        for (auto * vp : { viewportXY.get(), viewportXZ.get(), viewportZY.get(), static_cast<ViewportOrtho *>(viewportArb.get()) }) {
            func(*vp);
        }
    }
    void resetTextureProperties();
    ViewportBase *viewport(const ViewportType vpType);
    ViewportOrtho *viewportOrtho(const ViewportType vpType);
    void closeEvent(QCloseEvent *event);
    void notifyUnsavedChanges();
    void updateTitlebar();

    SegmentState segmentState{SegmentState::On};
    void setSegmentState(const SegmentState newState);
    SynapseState synapseState{SynapseState::Off};
    void setSynapseState(const SynapseState newState);
    void toggleSynapses();
public slots:
    void refreshPluginMenu();
    void setProofReadingUI(const bool on);
    void setJobModeUI(bool enabled);
    void updateLoaderProgress(int refCount);
    void updateCursorLabel(const Coordinate & position, const ViewportType vpType);
    // for the recent file menu
    bool openFileDispatch(QStringList fileNames, const bool mergeAll = false, const bool silent = false);
    void updateRecentFile(const QString &fileName);

    /* skeleton menu */
    bool newAnnotationSlot();
    void openSlot();
    void saveSlot();
    void saveAsSlot();
    void save(QString filename = Session::singleton().annotationFilename, const bool silent = false, const bool allocIncrement = true);
    void exportToNml();

    /* edit skeleton menu*/
    void setWorkMode(AnnotationMode workMode);
    void clearSkeletonSlot();

    /* preferences menu */
    void loadCustomPreferencesSlot();
    void saveCustomPreferencesSlot();
    void defaultPreferencesSlot();

    /* toolbar slots */
    void copyClipboardCoordinates();
    void pasteClipboardCoordinates();
    void coordinateEditingFinished();

    void updateCoordinateBar(int x, int y, int z);
    // viewports
    void resetViewports();
    void showVPDecorationClicked();

    // from the event handler
    void newTreeSlot();
    void pushBranchNodeSlot();
    void popBranchNodeSlot();
    void pythonPropertiesSlot();
    void pythonFileSlot();
    void pythonInterpreterSlot();
    void pythonPluginMgrSlot();
};

#endif // MAINWINDOW_H
