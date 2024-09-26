/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "zoomslider.hpp"

// Kasten gui
#include <Kasten/Zoomable>
// Kasten core
#include <Kasten/AbstractModel>
// KF
#include <KLocalizedString>
// Qt
#include <QSlider>
#include <QToolButton>
#include <QHBoxLayout>
#include <QApplication>
#include <QHelpEvent>

namespace Kasten {

static constexpr int ZoomSliderWidth = 150;

// TODO: look at Dolphin/Krita/KOffice zoom tool

// TODO: different zoom strategies: fixed step size, relative step size
// where to put this, behind interface? or better into a zoomtool?

ZoomSlider::ZoomSlider(QWidget* parent)
    : QWidget(parent)
{
    mZoomNormalButton = new QToolButton(this);
    mZoomNormalButton->setIcon(QIcon::fromTheme(QStringLiteral("zoom-original")));
    mZoomNormalButton->setAutoRaise(true);

    mZoomOutButton = new QToolButton(this);
    mZoomOutButton->setIcon(QIcon::fromTheme(QStringLiteral("zoom-out")));
    mZoomOutButton->setAutoRaise(true);

    mSlider = new QSlider(Qt::Horizontal, this);

    mZoomInButton = new QToolButton(this);
    mZoomInButton->setIcon(QIcon::fromTheme(QStringLiteral("zoom-in")));
    mZoomInButton->setAutoRaise(true);

    auto* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(mZoomNormalButton);
    layout->addWidget(mZoomOutButton);
    layout->addWidget(mSlider);
    layout->addWidget(mZoomInButton);

    connect(mZoomNormalButton, &QAbstractButton::clicked,
            this, &ZoomSlider::zoomNormal);
    connect(mZoomOutButton, &QAbstractButton::clicked,
            this, &ZoomSlider::zoomOut);
    connect(mZoomInButton, &QAbstractButton::clicked,
            this, &ZoomSlider::zoomIn);
    connect(mSlider, &QSlider::valueChanged,
            this, &ZoomSlider::onSliderValueChanged);
    connect(mSlider, &QSlider::sliderMoved,
            this, &ZoomSlider::onSliderMoved);

    setFixedWidth(ZoomSliderWidth);

    setTargetModel(nullptr);
}

ZoomSlider::~ZoomSlider() = default;

void ZoomSlider::setTargetModel(AbstractModel* model)
{
    if (mModel) {
        mModel->disconnect(this);
    }

    mModel = model ? model->findBaseModelWithInterface<If::Zoomable*>() : nullptr;
    mZoomControl = mModel ? qobject_cast<If::Zoomable*>(mModel) : nullptr;

    const bool hasView = (mZoomControl != nullptr);
    if (hasView) {
        mSlider->setSingleStep(1);   // mZoomControl->zoomLevelSingleStep()?
        mSlider->setPageStep(5);   // mZoomControl->zoomLevelPageStep()?

        const int min = 0; // mZoomControl->minimumZoomLevel();
        const int max = 99; // mZoomControl->maximumZoomLevel();
        mSlider->setRange(min, max);

        onZoomLevelChange(mZoomControl->zoomLevel());
        const int sliderValue = mSlider->value();
        const bool isZoomed = (sliderValue != 50);
        mZoomNormalButton->setEnabled(isZoomed);
        mZoomOutButton->setEnabled(sliderValue > mSlider->minimum());
        mZoomInButton->setEnabled(sliderValue < mSlider->maximum());
        connect(mModel, SIGNAL(zoomLevelChanged(double)), SLOT(onZoomLevelChange(double)));
    } else {
        mZoomNormalButton->setEnabled(false);
        mZoomOutButton->setEnabled(false);
        mZoomInButton->setEnabled(false);
        // put slider in the middle
        mSlider->setRange(0, 99);
        mSlider->setValue(50);
    }

    mSlider->setEnabled(hasView);
}

void ZoomSlider::updateToolTip(int sliderValue)
{
    if (m_toolTipSliderValue == sliderValue) {
        return;
    }

    m_toolTipSliderValue = sliderValue;

    const float zoomLevel = 50.0 / (100 - m_toolTipSliderValue);
    const int zoomPercent = static_cast<int>(zoomLevel * 100 + 0.5);
    mSlider->setToolTip(i18nc("@info:tooltip", "Zoom: %1%", zoomPercent));
// TODO: get the text by a signal toolTipNeeded( int zoomLevel, QString* toolTipText ); ?
}

void ZoomSlider::zoomOut()
{
    const int newValue = mSlider->value() - mSlider->singleStep();
    mSlider->setValue(newValue);
}

void ZoomSlider::zoomIn()
{
    const int newValue = mSlider->value() + mSlider->singleStep();
    mSlider->setValue(newValue);
}

void ZoomSlider::zoomNormal()
{
    mSlider->setValue(50);
}

void ZoomSlider::onSliderValueChanged(int sliderValue)
{
    updateToolTip(sliderValue);
    const bool isZoomed = (sliderValue != 50);
    mZoomNormalButton->setEnabled(isZoomed);
    mZoomOutButton->setEnabled(sliderValue > mSlider->minimum());
    mZoomInButton->setEnabled(sliderValue < mSlider->maximum());

    if (m_isUpdatingSlider) {
        return;
    }

    if (mZoomControl) {
        mZoomControl->setZoomLevel(50.0 / (100 - sliderValue));
    }
}

// ensure correct calculation of zoomLevel, best by model
// but can be timeconsuming?
// use timer to delay resize, so that sliding is not delayed by resizing
void ZoomSlider::onSliderMoved(int sliderValue)
{
    // QSlider::sliderMoved gets emitted before valueChanged
    // so we have to make sure the tooltip text is updated
    // before sending synchronously the tooltip event next
    updateToolTip(sliderValue);

    QPoint toolTipPoint = mSlider->rect().topLeft();
    toolTipPoint.ry() += mSlider->height() / 2;
    toolTipPoint = mSlider->mapToGlobal(toolTipPoint);

    QHelpEvent toolTipEvent(QEvent::ToolTip, QPoint(0, 0), toolTipPoint);
    QApplication::sendEvent(mSlider, &toolTipEvent);
}

void ZoomSlider::onZoomLevelChange(double level)
{
    m_isUpdatingSlider = true;

    const int newSliderValue = 100 - static_cast<int>(50.0 / level + 0.5);
    mSlider->setValue(newSliderValue);

    m_isUpdatingSlider = false;
}

}

#include "moc_zoomslider.cpp"
