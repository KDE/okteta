/*
    SPDX-FileCopyrightText: 2008, 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTMODELEXPORTERCONFIGEDITOR_HPP
#define KASTEN_ABSTRACTMODELEXPORTERCONFIGEDITOR_HPP

// lib
#include "kastengui_export.hpp"
// Qt
#include <QWidget>
// Std
#include <memory>

namespace Kasten {

class AbstractSelectionView;

class KASTENGUI_EXPORT AbstractModelExporterConfigEditor : public QWidget
{
    Q_OBJECT

protected:
    explicit AbstractModelExporterConfigEditor(QWidget* parent = nullptr);

public:
    ~AbstractModelExporterConfigEditor() override;

public: // API to be implemented
    /// default returns true
    [[nodiscard]]
    virtual bool isValid() const;
    /// default returns none
    [[nodiscard]]
    virtual std::unique_ptr<AbstractSelectionView> createPreviewView() const;

Q_SIGNALS:
    void validityChanged(bool isValid);

private:
    const std::unique_ptr<class AbstractModelExporterConfigEditorPrivate> d_ptr;
};

}

#endif
