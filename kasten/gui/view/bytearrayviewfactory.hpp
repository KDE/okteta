/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYVIEWFACTORY_HPP
#define KASTEN_BYTEARRAYVIEWFACTORY_HPP

// lib
#include "oktetakastengui_export.hpp"
// Kasten gui
#include <Kasten/AbstractViewFactory>

namespace Kasten {
class ByteArrayViewProfileManager;

class OKTETAKASTENGUI_EXPORT ByteArrayViewFactory : public AbstractViewFactory
{
public:
    explicit ByteArrayViewFactory(ByteArrayViewProfileManager* byteArrayViewProfileManager);

public:
    AbstractView* createViewFor(AbstractDocument* document) override;
    AbstractView* createCopyOfView(AbstractView* view, Qt::Alignment alignment) override;

private:
    ByteArrayViewProfileManager* const mByteArrayViewProfileManager;
};

}

#endif
