/*
 *  This file is a part of KNOSSOS.
 *
 *  (C) Copyright 2007-2016
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
 *
 *
 *  For further information, visit http://www.knossostool.org
 *  or contact knossos-team@mpimf-heidelberg.mpg.de
 */

#ifndef VIEWPORTTAB_H
#define VIEWPORTTAB_H

#include <QButtonGroup>
#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSettings>
#include <QVBoxLayout>
#include <QWidget>

class ViewportTab : public QWidget {
    friend class PreferencesWidget;
    friend class ViewportBase;
    friend class MainWindow;
    Q_OBJECT
    QHBoxLayout mainLayout;
    QGroupBox generalBox{tr("General")};
    QVBoxLayout generalLayout;
    QCheckBox showScalebarCheckBox{"Show scalebar"};
    QCheckBox showVPDecorationCheckBox{"Show viewport decorations"};
    QCheckBox drawIntersectionsCrossHairCheckBox{"Draw intersections crosshairs"};
    QCheckBox addArbVPCheckBox{"Add viewport with arbitrary view"};
    // 3D viewport
    QGroupBox viewport3DBox{tr("3D Viewport")};
    QVBoxLayout viewport3DLayout;
    QGroupBox planesBox{tr("Viewport planes")};
    QVBoxLayout planesLayout;
    QCheckBox showXYPlaneCheckBox{"Show XY Plane"};
    QCheckBox showXZPlaneCheckBox{"Show XZ Plane"};
    QCheckBox showZYPlaneCheckBox{"Show ZY Plane"};
    QCheckBox showArbPlaneCheckBox{"Show arbitrary Plane"};
    QGroupBox boundaryBox{tr("Boundaries")};
    QVBoxLayout boundaryLayout;
    QButtonGroup boundaryGroup;
    QRadioButton boundariesPixelRadioBtn{"Display dataset boundaries in pixels"};
    QRadioButton boundariesPhysicalRadioBtn{"Display dataset boundaries in µm"};
    QGroupBox rotationBox{tr("Rotation")};
    QVBoxLayout rotationLayout;
    QButtonGroup rotationCenterGroup;
    QRadioButton rotateAroundDatasetCenterRadioBtn{"Rotate around dataset center"};
    QRadioButton rotateAroundActiveNodeRadioBtn{"Rotate around active Node"};
    QRadioButton rotateAroundCurrentPositionRadioBtn{"Rotate around current position"};

    void saveSettings(QSettings & settings) const;
    void loadSettings(const QSettings & settings);
public:
    explicit ViewportTab(QWidget *parent = 0);

signals:
    void setViewportDecorations(const bool);
};

#endif // VIEWPORTTAB_H
