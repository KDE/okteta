/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTSELECTIONVIEW_HPP
#define KASTEN_ABSTRACTSELECTIONVIEW_HPP

// lib
#include "kastengui_export.hpp"
// Qt
#include <QObject>

class QWidget;

namespace Kasten {

class AbstractModel;
class AbstractModelSelection;

class KASTENGUI_EXPORT AbstractSelectionView : public QObject
{
    Q_OBJECT

protected:
    AbstractSelectionView();

public:
    ~AbstractSelectionView() override;

public: // API to be implemented
    virtual QWidget* widget() const = 0;
    virtual void setData(AbstractModel* model, const AbstractModelSelection* selection) = 0;

private:
    const QScopedPointer<class AbstractSelectionViewPrivate> d_ptr;
};

}

#endif
