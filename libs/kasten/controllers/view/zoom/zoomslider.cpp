/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "zoomslider.hpp"

// Kasten gui
#include <Kasten/Zoomable>
#include <Kasten/ZoomLevelsQueryable>
// Kasten core
#include <Kasten/AbstractModel>
// KF
#include <KLocalizedString>
#include <KStandardAction>
// Qt
#include <QSlider>
#include <QToolButton>
#include <QHBoxLayout>
#include <QApplication>
#include <QHelpEvent>

namespace Kasten {

static constexpr int ZoomSliderWidth = 150;

static constexpr double DefaultZoomSliderZoomOutLevelsSize = 49;
static constexpr double DefaultZoomSliderZoomInLevelsSize = 50;

// TODO: look at Dolphin/Krita/KOffice zoom tool

// TODO: different zoom strategies: fixed step size, relative step size
// where to put this, behind interface? or better into a zoomtool?

ZoomSlider::ZoomSlider(QWidget* parent)
    : QWidget(parent)
{
    auto* zoomNormalButton = new QToolButton(this);
    m_zoomNormalAction = KStandardAction::actualSize(this, &ZoomSlider::zoomNormal, this);
    m_zoomNormalAction->setShortcut(QKeySequence()); // unset shortcut, here no integration into main menu
    zoomNormalButton->setDefaultAction(m_zoomNormalAction);
    zoomNormalButton->setAutoRaise(true);

    auto* zoomOutButton = new QToolButton(this);
    m_zoomOutAction = KStandardAction::zoomOut(this, &ZoomSlider::zoomOut, this);
    m_zoomOutAction->setShortcut(QKeySequence()); // unset shortcut, here no integration into main menu
    zoomOutButton->setDefaultAction(m_zoomOutAction);
    zoomOutButton->setAutoRaise(true);

    // slider: use 0 as 100 % value, negative values for zoom-out levels, positive for zoom-in levels
    mSlider = new QSlider(Qt::Horizontal, this);
    mSlider->setSingleStep(1);   // mZoomControl->zoomLevelSingleStep()?
    mSlider->setPageStep(5);   // mZoomControl->zoomLevelPageStep()?

    auto* zoomInButton = new QToolButton(this);
    m_zoomInAction = KStandardAction::zoomIn(this, &ZoomSlider::zoomIn,  this);
    m_zoomInAction->setShortcut(QKeySequence()); // unset shortcut, here no integration into main menu
    zoomInButton->setDefaultAction(m_zoomInAction);
    zoomInButton->setAutoRaise(true);

    auto* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(zoomNormalButton);
    layout->addWidget(zoomOutButton);
    layout->addWidget(mSlider);
    layout->addWidget(zoomInButton);

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
    m_zoomLevelsControl = nullptr;

    const bool hasView = (mZoomControl != nullptr);
    if (hasView) {
        m_zoomLevelsControl = qobject_cast<If::ZoomLevelsQueryable*>(mModel);
        if (m_zoomLevelsControl) {
            connect(mModel, SIGNAL(zoomLevelsChanged()), this, SLOT(onZoomLevelsChanged()));
        }

        onZoomLevelsChanged();

        onZoomScaleChange(mZoomControl->zoomScale());
        const int sliderValue = mSlider->value();
        const bool isZoomed = (sliderValue != 0);
        m_zoomNormalAction->setEnabled(isZoomed);
        m_zoomOutAction->setEnabled(sliderValue > mSlider->minimum());
        m_zoomInAction->setEnabled(sliderValue < mSlider->maximum());
        connect(mModel, SIGNAL(zoomScaleChanged(double)), SLOT(onZoomScaleChange(double)));
    } else {
        m_zoomNormalAction->setEnabled(false);
        m_zoomOutAction->setEnabled(false);
        m_zoomInAction->setEnabled(false);
        // put slider in the middle
        mSlider->setRange(-DefaultZoomSliderZoomInLevelsSize, DefaultZoomSliderZoomOutLevelsSize);
        mSlider->setValue(0);
    }

    mSlider->setEnabled(hasView);
}

void ZoomSlider::updateToolTip(int sliderValue)
{
    if (m_toolTipSliderValue == sliderValue) {
        return;
    }

    m_toolTipSliderValue = sliderValue;

    const float zoomScale = m_zoomLevelsControl ? m_zoomLevelsControl->zoomScaleForLevel(m_toolTipSliderValue) : 50.0 / (50 - m_toolTipSliderValue);
    const int zoomPercent = static_cast<int>(zoomScale * 100 + 0.5);
    mSlider->setToolTip(i18nc("@info:tooltip", "Zoom: %1%", zoomPercent));
// TODO: get the text by a signal toolTipNeeded( int zoomScale, QString* toolTipText ); ?
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
    mSlider->setValue(0);
}

void ZoomSlider::onSliderValueChanged(int sliderValue)
{
    updateToolTip(sliderValue);
    const bool isZoomed = (sliderValue != 0);
    m_zoomNormalAction->setEnabled(isZoomed);
    m_zoomOutAction->setEnabled(sliderValue > mSlider->minimum());
    m_zoomInAction->setEnabled(sliderValue < mSlider->maximum());

    if (m_isUpdatingSlider) {
        return;
    }

    if (mZoomControl) {
        const double zoomScale = m_zoomLevelsControl ? m_zoomLevelsControl->zoomScaleForLevel(sliderValue) : 50.0 / (50 - sliderValue);

        mZoomControl->setZoomScale(zoomScale);
    }
}

// ensure correct calculation of zoom scale, best by model
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

void ZoomSlider::onZoomScaleChange(double zoomScale)
{
    m_isUpdatingSlider = true;

    const int newSliderValue = m_zoomLevelsControl ? m_zoomLevelsControl->zoomLevelForScale(zoomScale) : 50 - static_cast<int>(50.0 / zoomScale + 0.5);
    mSlider->setValue(newSliderValue);

    m_isUpdatingSlider = false;
}

void ZoomSlider::onZoomLevelsChanged()
{
    const int zoomOutLevelsSize = m_zoomLevelsControl ? m_zoomLevelsControl->zoomOutLevelsSize() : DefaultZoomSliderZoomOutLevelsSize;
    const int zoomInLevelsSize = m_zoomLevelsControl ? m_zoomLevelsControl->zoomInLevelsSize() : DefaultZoomSliderZoomInLevelsSize;
    mSlider->setRange(-zoomOutLevelsSize, zoomInLevelsSize);
}

}

#include "moc_zoomslider.cpp"
