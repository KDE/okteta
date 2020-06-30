/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_IF_DATASELECTABLE_HPP
#define KASTEN_IF_DATASELECTABLE_HPP

// Qt
#include <QtPlugin>

class QMimeData;

namespace Kasten {

class AbstractModelSelection;

namespace If {

// TODO: this interface is strongly related to the selecteddatawriteable interface
class DataSelectable
{
public:
    virtual ~DataSelectable();

public: // set
    virtual void selectAllData(bool selectAll) = 0;

public: // get
    virtual bool hasSelectedData() const = 0;
    virtual QMimeData* copySelectedData() const = 0; // TODO: move into AbstractModelSelection
    virtual const AbstractModelSelection* modelSelection() const = 0;

public: // signal
    /// emitted if there is a change in whether selected data is available or not
    virtual void hasSelectedDataChanged(bool hasSelectedData) = 0;
    /// emitted if the selection changes, to other data or none
    virtual void selectedDataChanged(const AbstractModelSelection* modelSelection) = 0;
};

inline DataSelectable::~DataSelectable() = default;

}
}

Q_DECLARE_INTERFACE(Kasten::If::DataSelectable, "org.kde.kasten.if.dataselectable/1.0")

#endif
