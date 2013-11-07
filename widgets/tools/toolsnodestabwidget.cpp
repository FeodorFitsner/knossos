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

#include "toolsnodestabwidget.h"
#include <QLabel>
#include <QSpinBox>
#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include "knossos.h"

extern struct stateInfo *state;

ToolsNodesTabWidget::ToolsNodesTabWidget(ToolsWidget *parent) :
    QWidget(parent), reference(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    activeNodeIdLabel = new QLabel("Active Node ID");
    activeNodeIdSpinBox = new QSpinBox();
    activeNodeIdSpinBox->setMaximum(0);
    activeNodeIdSpinBox->setMinimum(0);

    jumpToNodeButton = new QPushButton("Jump to node(s)");
    deleteNodeButton = new QPushButton("Delete Node(Del)");
    linkNodeWithButton = new QPushButton("Link Node with(Shift + Click)");

    idLabel = new QLabel("ID:");
    idSpinBox = new QSpinBox();
    idSpinBox->setMaximum(0); // no nodes on start-up
    idSpinBox->setMinimum(0);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    commentLabel = new QLabel("Comment:");
    searchForLabel = new QLabel("Search For:");
    commentField = new QLineEdit();
    searchForField = new QLineEdit();

    QFrame *line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);

    findNextButton = new QPushButton("Find (n)ext");
    findPreviousButton = new QPushButton("Find (p)revious");

    QFrame *line3 = new QFrame();
    line3->setFrameShape(QFrame::HLine);
    line3->setFrameShadow(QFrame::Sunken);

    QFrame *line4 = new QFrame();
    line4->setFrameShape(QFrame::HLine);
    line4->setFrameShadow(QFrame::Sunken);

    useLastRadiusBox = new QCheckBox("Use Last Radius as Default");
    activeNodeRadiusLabel = new QLabel("Active Node Radius(SHIFT + wheel):");
    activeNodeRadiusSpinBox = new QDoubleSpinBox();
    activeNodeRadiusSpinBox->setMaximum(100000);
    defaultNodeRadiusLabel = new QLabel("Default Node Radius:");
    defaultNodeRadiusSpinBox = new QDoubleSpinBox();
    defaultNodeRadiusSpinBox->setMaximum(100000);

    enableCommentLockingCheckBox = new QCheckBox("Enable comment locking");
    lockingRadiusLabel = new QLabel("Locking Radius:");
    lockToNodesWithCommentLabel = new QLabel("Lock To Nodes With Comment:");
    lockingRadiusSpinBox = new QSpinBox();
    lockingRadiusSpinBox->setMaximum(100000);
    lockingToNodesWithCommentField = new QLineEdit("seed");
    lockToActiveNodeButton = new QPushButton("Lock to Active Node");
    disableLockingButton = new QPushButton("Disable Locking");

    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow(activeNodeIdLabel, activeNodeIdSpinBox);

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(jumpToNodeButton, 1, 1);
    gridLayout->addWidget(deleteNodeButton, 1, 2);
    gridLayout->addWidget(linkNodeWithButton, 2, 1);
    gridLayout->addWidget(idSpinBox, 2,2);

    //QFormLayout *formLayout2 = new QFormLayout();
    //formLayout2->addRow(idLabel, idSpinBox);

    QFormLayout *formLayout3 = new QFormLayout();
    formLayout3->addRow(commentLabel, commentField);
    formLayout3->addRow(searchForLabel, searchForField);

    QGridLayout *gridLayout2 = new QGridLayout();
    gridLayout2->addWidget(findNextButton, 1 ,1);
    gridLayout2->addWidget(findPreviousButton, 1, 2);

    QGridLayout *gridLayout3 = new QGridLayout();
    gridLayout3->addWidget(useLastRadiusBox, 1, 1);
    gridLayout3->addWidget(activeNodeRadiusLabel, 2, 1);
    gridLayout3->addWidget(activeNodeRadiusSpinBox, 2, 2);
    gridLayout3->addWidget(defaultNodeRadiusLabel, 3, 1);
    gridLayout3->addWidget(defaultNodeRadiusSpinBox, 3, 2);

    QGridLayout *gridLayout4 = new QGridLayout();
    gridLayout4->addWidget(enableCommentLockingCheckBox, 1, 1);
    gridLayout4->addWidget(lockingRadiusLabel, 2, 1);
    gridLayout4->addWidget(lockingRadiusSpinBox, 2, 2);
    gridLayout4->addWidget(lockToNodesWithCommentLabel, 3, 1);
    gridLayout4->addWidget(lockingToNodesWithCommentField, 4, 1);
    gridLayout4->addWidget(lockToActiveNodeButton, 5, 1);
    gridLayout4->addWidget(disableLockingButton, 5, 2);

    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(gridLayout);
    //mainLayout->addLayout(formLayout2);
    mainLayout->addWidget(line);
    mainLayout->addLayout(formLayout3);
    mainLayout->addWidget(line2);
    mainLayout->addLayout(gridLayout2);
    mainLayout->addWidget(line3);
    mainLayout->addLayout(gridLayout3);
    mainLayout->addWidget(line4);
    mainLayout->addLayout(gridLayout4);

    setLayout(mainLayout);    
    connect(jumpToNodeButton, SIGNAL(clicked()), this, SLOT(jumpToNodeButtonClicked()));
    connect(deleteNodeButton, SIGNAL(clicked()), this, SLOT(deleteNodeButtonClicked()));
    connect(linkNodeWithButton, SIGNAL(clicked()), this, SLOT(linkNodeWithButtonClicked()));
    connect(idSpinBox, SIGNAL(valueChanged(int)), this, SLOT(idChanged(int)));

    connect(useLastRadiusBox, SIGNAL(clicked(bool)), this, SLOT(useLastRadiusChecked(bool)));
    connect(activeNodeRadiusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(activeNodeRadiusChanged(double)));
    connect(defaultNodeRadiusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(defaultNodeRadiusChanged(double)));
    connect(enableCommentLockingCheckBox, SIGNAL(clicked(bool)), this, SLOT(enableCommentLockingChecked(bool)));
    connect(lockingRadiusSpinBox, SIGNAL(valueChanged(int)), this, SLOT(lockingRadiusChanged(int)));
    connect(lockingToNodesWithCommentField, SIGNAL(textChanged(QString)), this, SLOT(lockToNodesWithCommentChanged(QString)));
    connect(lockToActiveNodeButton, SIGNAL(clicked()), this, SLOT(lockToActiveNodeButtonClicked()));
    connect(disableLockingButton, SIGNAL(clicked()), this, SLOT(disableLockingButtonClicked()));

}


void ToolsNodesTabWidget::idChanged(int value) {
    state->viewerState->gui->activeNodeID = value;
}


void ToolsNodesTabWidget::jumpToNodeButtonClicked() {
    if(state->skeletonState->activeNode) {
        emit setRemoteStateTypeSignal(REMOTE_RECENTERING);
        emit setRecenteringPositionSignal(state->skeletonState->activeNode->position.x / state->magnification,
                                       state->skeletonState->activeNode->position.y / state->magnification,
                                       state->skeletonState->activeNode->position.z / state->magnification);
        emit Knossos::sendRemoteSignal();

    }
}

void ToolsNodesTabWidget::deleteNodeButtonClicked() {
    emit deleteActiveNodeSignal();
    reference->updateToolsSlot();
}

void ToolsNodesTabWidget::linkNodeWithButtonClicked() {
    if((state->skeletonState->activeNode) && (findNodeByNodeIDSignal(this->idSpinBox->value()))) {
         emit addSegmentSignal(CHANGE_MANUAL, state->skeletonState->activeNode->nodeID, this->idSpinBox->value(), true);
    }
}


void ToolsNodesTabWidget::findNextButtonClicked() {
    char *searchStr = const_cast<char *>(this->searchForField->text().toStdString().c_str());
    qDebug() << searchStr;
    emit nextCommentSignal(searchStr);
}

void ToolsNodesTabWidget::findPreviousButtonClicked() {
    char *searchStr = const_cast<char *>(this->searchForField->text().toStdString().c_str());
    qDebug() << searchStr;
    emit previousCommentSignal(searchStr);
}

void ToolsNodesTabWidget::useLastRadiusChecked(bool on) {    
    if(on) {
        state->skeletonState->defaultNodeRadius = activeNodeIdSpinBox->value();
    } else {
        state->skeletonState->defaultNodeRadius = defaultNodeRadiusSpinBox->value();
    }
}

void ToolsNodesTabWidget::activeNodeRadiusChanged(double value) {
    if(state->skeletonState->activeNode) {
        state->skeletonState->defaultNodeRadius = value;
        state->skeletonState->activeNode->radius = value;
    }

}

void ToolsNodesTabWidget::defaultNodeRadiusChanged(double value) {
    state->skeletonState->defaultNodeRadius = value;
}

void ToolsNodesTabWidget::enableCommentLockingChecked(bool on) {
    state->skeletonState->positionLocked = on;
    if(on and lockingToNodesWithCommentField->text().isEmpty())
        state->viewerState->gui->lockComment = lockingToNodesWithCommentField->text();
       // state->skeletonState->commentBuffer = const_cast<char *>(lockingToNodesWithCommentField->text().toStdString().c_str());
    //
}

void ToolsNodesTabWidget::lockingRadiusChanged(int value) {
    state->skeletonState->lockRadius = value;
}

void ToolsNodesTabWidget::lockToNodesWithCommentChanged(QString comment) {
    state->viewerState->gui->lockComment = comment;
}

void ToolsNodesTabWidget::lockToActiveNodeButtonClicked() {
    Coordinate activeNodePosition;

    if(state->skeletonState->activeNode) {
        qDebug("Locking to active node");

        activeNodePosition.x = state->skeletonState->activeNode->position.x;
        activeNodePosition.y = state->skeletonState->activeNode->position.y;
        activeNodePosition.z = state->skeletonState->activeNode->position.z;

        emit lockPositionSignal(activeNodePosition);

    } else {
        qDebug("There is not active node to lock");
    }
}

void ToolsNodesTabWidget::disableLockingButtonClicked() {
    emit unlockPositionSignal();
}
