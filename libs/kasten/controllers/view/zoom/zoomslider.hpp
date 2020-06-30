/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ZOOMSLIDER_HPP
#define KASTEN_ZOOMSLIDER_HPP

// Qt
#include <QWidget>

class QSlider;
class QToolButton;

namespace Kasten {

class AbstractModel;
namespace If {
class Zoomable;
}

class ZoomSlider : public QWidget
{
    Q_OBJECT

public:
    explicit ZoomSlider(QWidget* parent);
    ~ZoomSlider() override;

public:
    void setTargetModel(AbstractModel* model);

private:
    void updateToolTip(int sliderValue);

private Q_SLOTS: // action slots
    void zoomIn();
    void zoomOut();
    void onSliderValueChanged(int sliderValue);
    void onSliderMoved(int sliderValue);

private Q_SLOTS:
    void onZoomLevelChange(double level);

private:
    AbstractModel* mModel = nullptr;
    If::Zoomable* mZoomControl = nullptr;

    double mZoomLevel;

    QSlider* mSlider;
    QToolButton* mZoomInButton;
    QToolButton* mZoomOutButton;
};

}

#endif
