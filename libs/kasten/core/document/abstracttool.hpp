/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTTOOL_HPP
#define KASTEN_ABSTRACTTOOL_HPP

// lib
#include "kastencore_export.hpp"
// Qt
#include <QObject>

class QString;

namespace Kasten {

class AbstractModel;

// TODO: what is the difference to a plain dependent model?
//
class KASTENCORE_EXPORT AbstractTool : public QObject
{
    Q_OBJECT

protected:
    AbstractTool();

public:
    ~AbstractTool() override;

public: // API to be implemented
//     virtual AbstractModel* targetModel() const = 0;
    virtual QString title() const = 0;

    virtual void setTargetModel(AbstractModel* model) = 0;

Q_SIGNALS:
    void titleChanged(const QString& newTitle);

private:
    const QScopedPointer<class AbstractToolPrivate> d_ptr;
};

}

#endif
