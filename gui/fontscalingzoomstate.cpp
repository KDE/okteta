/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2024-2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "fontscalingzoomstate.hpp"

// Std
#include <cmath>

namespace Okteta {

void FontScalingZoomState::initFont(const QFont& font)
{
    m_font = font;
    m_defaultFontSize = m_font.pointSize();
}

void FontScalingZoomState::setFont(const QFont& font)
{
    m_font = font;
    m_defaultFontSize = m_font.pointSize();

    m_outLevelsSize = (minimumFontSize() < m_defaultFontSize) ? m_defaultFontSize - minimumFontSize() : 0;
    m_inLevelsSize = (m_defaultFontSize < maximumFontSize()) ? maximumFontSize() - m_defaultFontSize : 0;

    int newPointSize = static_cast<int>(std::round(m_scale * m_defaultFontSize));
    if (newPointSize < minimumFontSize()) {
        newPointSize = minimumFontSize();
    } else if (newPointSize > maximumFontSize()) {
        newPointSize = maximumFontSize();
    }

    m_font.setPointSize(newPointSize);
    m_scale = static_cast<double>(newPointSize) / m_defaultFontSize;
}

double FontScalingZoomState::scaleForLevel(int level)  const
{
    if (m_outLevelsSize < -level) {
        level = -m_outLevelsSize;
    } else if (m_inLevelsSize < level) {
        level = m_inLevelsSize;
    }

    const int newPointSize = m_defaultFontSize + level;
    return static_cast<double>(newPointSize) / m_defaultFontSize;
}

int FontScalingZoomState::levelForScale(double zoomScale) const
{
    const int pointSize = static_cast<int>(std::round(zoomScale * m_defaultFontSize));
    int level = pointSize - m_defaultFontSize;
    if (m_outLevelsSize < -level) {
        level = -m_outLevelsSize;
    } else if (m_inLevelsSize < level) {
        level = m_inLevelsSize;
    }
    return level;
}

void FontScalingZoomState::zoomIn(int pointIncrement)
{
    int newPointSize = m_font.pointSize() + pointIncrement;
    if (newPointSize > maximumFontSize()) {
        newPointSize = maximumFontSize();
    }

    m_font.setPointSize(newPointSize);
    m_scale = static_cast<double>(newPointSize) / m_defaultFontSize;
}

void FontScalingZoomState::zoomOut(int pointDecrement)
{
    int newPointSize = m_font.pointSize() - pointDecrement;
    if (newPointSize < minimumFontSize()) {
        newPointSize = minimumFontSize();
    }

    m_font.setPointSize(newPointSize);
    m_scale = static_cast<double>(newPointSize) / m_defaultFontSize;
}

bool FontScalingZoomState::zoomTo(int newPointSize)
{
    if (newPointSize < minimumFontSize()) {
        newPointSize = minimumFontSize();
    } else if (newPointSize > maximumFontSize()) {
        newPointSize = maximumFontSize();
    }

    if (m_font.pointSize() == newPointSize) {
        return false;
    }

    m_font.setPointSize(newPointSize);
    m_scale = static_cast<double>(newPointSize) / m_defaultFontSize;

    return true;
}

bool FontScalingZoomState::setScale(double zoomScale)
{
    const int currentPointSize = m_font.pointSize();

    // TODO: here we catch any new zoom scales which are out of bounds and the zoom already at that bound
    if ((currentPointSize <= minimumFontSize() &&
         zoomScale < (static_cast<double>(minimumFontSize()) / m_defaultFontSize)) ||
        (maximumFontSize() <= currentPointSize &&
         (static_cast<double>(maximumFontSize()) / m_defaultFontSize) < zoomScale)) {
        return false;
    }

    int newPointSize = static_cast<int>(std::round(zoomScale * m_defaultFontSize));
    if (newPointSize < minimumFontSize()) {
        newPointSize = minimumFontSize();
    } else if (newPointSize > maximumFontSize()) {
        newPointSize = maximumFontSize();
    }

    // other than in zoomTo(), where the new zoom scale is calculated from the integers, here
    // use the passed zoom scale value, to avoid getting trapped inside a small integer value,
    // if the zoom tool operates relatively
    // think about, if this is the right approach
    m_font.setPointSize(newPointSize);
    m_scale = zoomScale;

    return true;
}

}
