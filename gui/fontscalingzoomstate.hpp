/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2024-2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_FONTSCALINGZOOMSTATE_HPP
#define OKTETA_FONTSCALINGZOOMSTATE_HPP

// Qt
#include <QFont>

namespace Okteta {

class FontScalingZoomState
{
private:
    static constexpr int MinFontPointSize = 2;
    static constexpr int MaxFontPointSize = 96;

public:
    FontScalingZoomState() = default;
    ~FontScalingZoomState() = default;

public: // setters/actions
    void initFont(const QFont& font);
    void setFont(const QFont& font);
    /// @returns @c true if scale changed
    bool setScale(double scale);

    void zoomIn(int pointInc);
    void zoomOut(int pointDec);
    /// @returns @c true if zoom changed
    bool zoomTo(int pointSize);

public: // query
    double scale() const;
    int defaultFontSize() const;
    int minimumFontSize() const;
    int maximumFontSize() const;
    int inLevelsSize() const;
    int outLevelsSize() const;

    double scaleForLevel(int zoomLevel) const;
    int levelForScale(double scale) const;

public:
    const QFont& font() const;

private:
    QFont m_font;
    int m_defaultFontSize;
    double m_scale = 1.0;
    int m_inLevelsSize = 0;
    int m_outLevelsSize = 0;
};

// FontScalingZoomState::FontScalingZoomState()
// , m_defaultFontSize(m_font.pointSize() ) crashes in font()

inline const QFont& FontScalingZoomState::font() const { return m_font; }
inline int FontScalingZoomState::defaultFontSize() const { return m_defaultFontSize; }
inline int FontScalingZoomState::minimumFontSize() const { return MinFontPointSize; }
inline int FontScalingZoomState::maximumFontSize() const { return MaxFontPointSize; }
inline double FontScalingZoomState::scale() const { return m_scale; }
inline int FontScalingZoomState::inLevelsSize() const { return m_inLevelsSize; }
inline int FontScalingZoomState::outLevelsSize() const { return m_outLevelsSize; }

}

#endif
