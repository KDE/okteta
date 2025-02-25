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
    FontScalingZoomState(const FontScalingZoomState&) = delete;

    ~FontScalingZoomState() = default;

    FontScalingZoomState& operator=(const FontScalingZoomState&) = delete;

public: // setters/actions
    void initFont(const QFont& font);
    void setFont(const QFont& font);
    /// @returns @c true if scale changed
    [[nodiscard]]
    bool setScale(double scale);

    void zoomIn(int pointInc);
    void zoomOut(int pointDec);
    /// @returns @c true if zoom changed
    [[nodiscard]]
    bool zoomTo(int pointSize);

public: // query
    [[nodiscard]]
    double scale() const;
    [[nodiscard]]
    int fontSize() const;
    [[nodiscard]]
    int defaultFontSize() const;
    [[nodiscard]]
    int minimumFontSize() const;
    [[nodiscard]]
    int maximumFontSize() const;
    [[nodiscard]]
    int inLevelsSize() const;
    [[nodiscard]]
    int outLevelsSize() const;

    [[nodiscard]]
    double scaleForLevel(int zoomLevel) const;
    [[nodiscard]]
    int levelForScale(double scale) const;

    [[nodiscard]]
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
inline int FontScalingZoomState::fontSize() const { return m_font.pointSize(); }
inline int FontScalingZoomState::defaultFontSize() const { return m_defaultFontSize; }
inline int FontScalingZoomState::minimumFontSize() const { return MinFontPointSize; }
inline int FontScalingZoomState::maximumFontSize() const { return MaxFontPointSize; }
inline double FontScalingZoomState::scale() const { return m_scale; }
inline int FontScalingZoomState::inLevelsSize() const { return m_inLevelsSize; }
inline int FontScalingZoomState::outLevelsSize() const { return m_outLevelsSize; }

}

#endif
