/*
    This file is part of the Okteta Designer plugin, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETAWIDGETDESIGNERFACTORYCOLLECTION_HPP
#define OKTETAWIDGETDESIGNERFACTORYCOLLECTION_HPP

// Qt
#include <QDesignerCustomWidgetCollectionInterface>
#include <QObject>

class OktetaWidgetDesignerFactoryCollection : public QObject
                                            , public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_INTERFACES(
        QDesignerCustomWidgetCollectionInterface
    )

    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface")

public:
    explicit OktetaWidgetDesignerFactoryCollection(QObject* parent = nullptr);

public: // QDesignerCustomWidgetCollectionInterface API
    QList<QDesignerCustomWidgetInterface*> customWidgets() const override;

private:
    QList<QDesignerCustomWidgetInterface*> mWidgetFactories;
};

#endif
