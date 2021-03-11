/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTMODELDATAGENERATORCONFIGEDITOR_HPP
#define KASTEN_ABSTRACTMODELDATAGENERATORCONFIGEDITOR_HPP

// lib
#include "kastengui_export.hpp"
// Qt
#include <QWidget>

namespace Kasten {

class KASTENGUI_EXPORT AbstractModelDataGeneratorConfigEditor : public QWidget
{
    Q_OBJECT

protected:
    explicit AbstractModelDataGeneratorConfigEditor(QWidget* parent = nullptr);

public:
    ~AbstractModelDataGeneratorConfigEditor() override;

public: // API to be implemented
    /// default returns true
    virtual bool isValid() const;

    virtual QString name() const = 0;

Q_SIGNALS:
    void validityChanged(bool isValid);

private:
    const QScopedPointer<class AbstractModelDataGeneratorConfigEditorPrivate> d_ptr;
};

}

#endif
