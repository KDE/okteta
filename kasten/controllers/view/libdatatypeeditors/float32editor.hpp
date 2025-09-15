/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_FLOAT32EDITOR_HPP
#define OKTETA_FLOAT32EDITOR_HPP

// libdatatypes
#include <float32.hpp>
// Qt
#include <QLineEdit>

namespace Okteta {

class Float32Editor : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(Float32 data READ data WRITE setData USER true)

public:
    explicit Float32Editor(QWidget* parent);

    ~Float32Editor() override;

public:
    void setData(Float32 data);
    [[nodiscard]]
    Float32 data() const;
};

}

#endif
