/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BORDERCOLUMNTEXTRENDERER_HPP
#define KASTEN_BORDERCOLUMNTEXTRENDERER_HPP

// lib
#include "abstractcolumntextrenderer.hpp"

namespace Kasten {

class BorderColumnTextRenderer : public AbstractColumnTextRenderer
{
public:
    ~BorderColumnTextRenderer() override = default;

public: // AbstractColumnTextRenderer API
    void renderFirstLine(QTextStream* stream, int lineIndex) const override;
    void renderNextLine(QTextStream* stream, bool isSubline) const override;

private:
    void render(QTextStream* stream) const;
};

}

#endif
