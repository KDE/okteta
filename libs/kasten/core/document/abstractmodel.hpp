/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTMODEL_HPP
#define KASTEN_ABSTRACTMODEL_HPP

// lib
#include "kastencore_export.hpp"
// Qt
#include <QObject>

class QString;

namespace Kasten {
class AbstractModelPrivate;

// TODO: reasons not to name it AbstractObjectModel, but just as it is
class KASTENCORE_EXPORT AbstractModel : public QObject
{
    Q_OBJECT

protected:
    explicit AbstractModel(AbstractModel* baseModel = nullptr);
    explicit AbstractModel(AbstractModelPrivate* d);

public:
    ~AbstractModel() override;

public:
// TODO: just one baseModel, or can there be multiple? Better name?
    AbstractModel* baseModel() const;
    /**
     * returns the first baseModel which is of type T, or null if none is found.
     * The search is started with the model itself
     */
    template <typename T>
    T findBaseModel() const;
    /**
     * returns the first baseModel which is of type T, or null if none is found.
     * The search is started with the model itself
     */
    template <typename T>
    AbstractModel* findBaseModelWithInterface() const;

public: // API to be implemented
    virtual QString title() const = 0;

    /** Default returns false */
    virtual bool isModifiable() const;
    /** default returns true */
    virtual bool isReadOnly() const;
    /** default does nothing */
    virtual void setReadOnly(bool isReadOnly);

Q_SIGNALS:
    // TODO: readonly and modifiable should be turned into flags, also get/set methods
    void readOnlyChanged(bool isReadOnly);
    void modifiableChanged(bool isModifiable);
    void titleChanged(const QString& newTitle);

protected:
    void setBaseModel(AbstractModel* baseModel);

protected:
    const QScopedPointer<AbstractModelPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractModel)
};

template <typename T>
T AbstractModel::findBaseModel() const
{
    auto* model = const_cast<AbstractModel*>(this);
    do {
        T castedModel = qobject_cast<T>(model);
        if (castedModel) {
            return castedModel;
        }
        model = model->baseModel();
    } while (model);

    return nullptr;
}

template <typename T>
AbstractModel* AbstractModel::findBaseModelWithInterface() const
{
    auto* model = const_cast<AbstractModel*>(this);
    do {
        T interface = qobject_cast<T>(model);
        if (interface) {
            return model;
        }
        model = model->baseModel();
    } while (model);

    return nullptr;
}

}

#endif
