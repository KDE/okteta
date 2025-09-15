/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYSTREAMENCODERCONFIGEDITORFACTORYFACTORY_HPP
#define KASTEN_BYTEARRAYSTREAMENCODERCONFIGEDITORFACTORYFACTORY_HPP

// lib
#include "oktetakastengui_export.hpp"
// Std
#include <memory>
#include <vector>

namespace Kasten {
class AbstractModelStreamEncoderConfigEditorFactory;

class OKTETAKASTENGUI_EXPORT ByteArrayStreamEncoderConfigEditorFactoryFactory
{
public:
    ByteArrayStreamEncoderConfigEditorFactoryFactory() = delete;
    ByteArrayStreamEncoderConfigEditorFactoryFactory(const ByteArrayStreamEncoderConfigEditorFactoryFactory&) = delete;
    ByteArrayStreamEncoderConfigEditorFactoryFactory(ByteArrayStreamEncoderConfigEditorFactoryFactory&&) = delete;

    ByteArrayStreamEncoderConfigEditorFactoryFactory& operator=(const ByteArrayStreamEncoderConfigEditorFactoryFactory&) = delete;
    ByteArrayStreamEncoderConfigEditorFactoryFactory& operator=(ByteArrayStreamEncoderConfigEditorFactoryFactory&&) = delete;

public:
    [[nodiscard]]
    static std::vector<std::unique_ptr<AbstractModelStreamEncoderConfigEditorFactory>> createFactorys();

};

}

#endif
