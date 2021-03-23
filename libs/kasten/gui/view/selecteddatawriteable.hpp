/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_IF_SELECTEDDATAWRITEABLE_HPP
#define KASTEN_IF_SELECTEDDATAWRITEABLE_HPP

// Qt
#include <QtPlugin>

class QMimeData;

namespace Kasten {

namespace If {
// TODO: this interface is strongly related to the selection interface
// needs further thinking
// for now the mimedata is the write extension to the selection interface
// as "createCopyOfSelectedData()" got moved to selection
// rename both interfaces to Selection and SelectionWriteable?
// TODO: cut does not work on overwrite modus (for me), how to generalize this?
class SelectedDataWriteable
{
public:
    virtual ~SelectedDataWriteable();

public: // operate
    // TODO: inserting works on selection if available
    virtual void insertData(const QMimeData* data) = 0;
    virtual QMimeData* cutSelectedData() = 0;
    virtual void deleteSelectedData() = 0;

public: // get
    virtual bool canReadData(const QMimeData* data) const = 0;
    // reports principle ability to cut selecteed data if there is some, also if currently there is none
    // TODO; does that make sense, or should be changed to also match only if there is selected data?
    virtual bool canCutSelectedData() const = 0;

public: // signal
    virtual void canCutSelectedDataChanged(bool canCutSelectedData) = 0;
};

inline SelectedDataWriteable::~SelectedDataWriteable() = default;

}
}

Q_DECLARE_INTERFACE(Kasten::If::SelectedDataWriteable, "org.kde.kasten.if.selecteddatawriteable/1.0")

#endif
