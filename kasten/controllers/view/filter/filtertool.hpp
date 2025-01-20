/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FILTERTOOL_HPP
#define KASTEN_FILTERTOOL_HPP

// Kasten core
#include <Kasten/AbstractTool>
// Std
#include <memory>
#include <vector>

class AbstractByteArrayFilterParameterSet;
class AbstractByteArrayFilter;

namespace Okteta {
class AbstractByteArrayModel;
}
class QString;

namespace Kasten {

class ByteArrayView;
class AbstractUserMessagesHandler;

class FilterTool : public AbstractTool
{
    Q_OBJECT

private:
    static inline constexpr char ConfigGroupId[] = "FilterTool";
    static inline constexpr char OperationConfigKey[] = "Operation";

public:
    explicit FilterTool(AbstractUserMessagesHandler* userMessagesHandler);
    ~FilterTool() override;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    [[nodiscard]]
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

public:
    [[nodiscard]]
    int filterId() const;
    [[nodiscard]]
    bool hasWriteable() const;
    [[nodiscard]]
    const std::vector<std::unique_ptr<AbstractByteArrayFilter>>& filterList() const;

public:
    [[nodiscard]]
    QString charCodecName() const;
    [[nodiscard]]
    AbstractByteArrayFilterParameterSet* parameterSet(int filterId);

    void saveParameterSet(int filterId);

public Q_SLOTS:
    void filter() const;
    void setFilter(int filterId);

Q_SIGNALS:
    void filterChanged(int filterId);
    void hasWriteableChanged(bool hasWriteable);
    void charCodecChanged(const QString& charCodecName);

private Q_SLOTS:
    void onApplyableChanged();

private:
    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;
    bool mHasWritable = false;

    AbstractUserMessagesHandler* const m_userMessagesHandler;

    std::vector<std::unique_ptr<AbstractByteArrayFilter>> mFilterList;

    int mFilterId = 0;
};

inline int FilterTool::filterId() const { return mFilterId; }

}

#endif
