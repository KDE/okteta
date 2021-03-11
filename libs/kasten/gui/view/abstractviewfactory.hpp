/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTVIEWFACTORY_HPP
#define KASTEN_ABSTRACTVIEWFACTORY_HPP

// lib
#include "abstractview.hpp"

namespace Kasten {

class KASTENGUI_EXPORT AbstractViewFactory
{
public:
    virtual ~AbstractViewFactory();

public:
    // TODO: there can be views not only on documents
    virtual AbstractView* createViewFor(AbstractDocument* document) = 0;
    // TODO: is alignment best done here? needs view to be stable on creation of view copy
    // doesn't work if the new view is not next to the old, but are there usecases for this?
    /**
     * @param alignment on which side the new view is placed to show a continuous whole view
     */
    virtual AbstractView* createCopyOfView(AbstractView* view, Qt::Alignment alignment = {});
};

inline AbstractViewFactory::~AbstractViewFactory() = default;

// TODO: is this default implementation useful? Like, if the base is not a document, but a subdocument/model?
inline AbstractView* AbstractViewFactory::createCopyOfView(AbstractView* view, Qt::Alignment alignment)
{
    Q_UNUSED(alignment)

    AbstractView * viewCopy = createViewFor(view->findBaseModel<AbstractDocument*>());
    if (viewCopy) {
        viewCopy->setReadOnly(view->isReadOnly());
    }

    return viewCopy;
}

}

#endif
