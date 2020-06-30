/*
    This file is part of the Okteta Designer plugin, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef BYTEARRAYROWVIEWDESIGNERFACTORY_HPP
#define BYTEARRAYROWVIEWDESIGNERFACTORY_HPP

// Qt
#include <QDesignerCustomWidgetInterface>
#include <QObject>

class ByteArrayRowViewDesignerFactory : public QObject
                                      , public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(
        QDesignerCustomWidgetInterface
    )

public:
    explicit ByteArrayRowViewDesignerFactory(QObject* parent = nullptr);

public: // QDesignerCustomWidgetInterface API
    QWidget* createWidget(QWidget* parent) override;
    QString group() const override;
    QIcon icon() const override;
    QString includeFile() const override;
    bool isContainer() const override;
    QString name() const override;
    QString toolTip() const override;
    QString whatsThis() const override;
};

#endif
