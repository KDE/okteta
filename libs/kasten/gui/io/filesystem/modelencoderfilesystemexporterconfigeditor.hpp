/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODELENCODERFILESYSTEMEXPORTERCONFIGEDITOR_HPP
#define KASTEN_MODELENCODERFILESYSTEMEXPORTERCONFIGEDITOR_HPP

// lib
#include "abstractmodelexporterconfigeditor.hpp"
// Std
#include <memory>

namespace Kasten {

class AbstractModelStreamEncoderConfigEditor;

class KASTENGUI_EXPORT ModelEncoderFileSystemExporterConfigEditor : public AbstractModelExporterConfigEditor
{
    Q_OBJECT

public:
    explicit ModelEncoderFileSystemExporterConfigEditor(std::unique_ptr<AbstractModelStreamEncoderConfigEditor>&& encoderConfigEditor);
    ~ModelEncoderFileSystemExporterConfigEditor() override;

public: // AbstractModelExporterConfigEditor API
    [[nodiscard]]
    bool isValid() const override;
    [[nodiscard]]
    std::unique_ptr<AbstractSelectionView> createPreviewView() const override;

private:
    AbstractModelStreamEncoderConfigEditor* const mEncoderConfigEditor;
};

}

#endif
