/*
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
    ByteArrayViewFactory(const ByteArrayViewFactory&) = delete;
    ByteArrayViewFactory(ByteArrayViewFactory&&) = delete;

    ~ByteArrayViewFactory() override = default;

    ByteArrayViewFactory& operator=(const ByteArrayViewFactory&) = delete;
    ByteArrayViewFactory& operator=(ByteArrayViewFactory&&) = delete;

public:
    [[nodiscard]]
    std::unique_ptr<AbstractView> createViewFor(AbstractDocument* document) override;
    [[nodiscard]]
    std::unique_ptr<AbstractView> createCopyOfView(AbstractView* view, Qt::Alignment alignment) override;

private:
    ByteArrayViewProfileManager* const mByteArrayViewProfileManager;
};

}

#endif
