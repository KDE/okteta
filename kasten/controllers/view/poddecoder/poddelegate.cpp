/*
    SPDX-FileCopyrightText: 2009, 2ß23 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "poddelegate.hpp"

// tool
#include "poddecodertool.hpp"
// libdatatypeeditors
#include <binary8editor.hpp>
#include <octal8editor.hpp>
#include <hexadecimal8editor.hpp>
#include <float32editor.hpp>
#include <float64editor.hpp>
#include <char8editor.hpp>
#include <utf8editor.hpp>
#include <utf16editor.hpp>
#include <sintspinbox.hpp>
#include <uintspinbox.hpp>
// libdatatypes
#include <sint8.hpp>
#include <sint16.hpp>
#include <sint32.hpp>
#include <sint64.hpp>
#include <uint8.hpp>
#include <uint16.hpp>
#include <uint32.hpp>
#include <uint64.hpp>

// TODO: Stranger that you are reading this: please help out and show how to add QVariant::Types for custom datatypes,
// so that instead of this inflexible maintenance mess^WWWunlooped code QItemEditorCreator can be used!

namespace Kasten {

PODDelegate::PODDelegate(PODDecoderTool* tool, QObject* parent)
    : QStyledItemDelegate(parent)
    , mTool(tool)
{
    qRegisterMetaType<Okteta::Binary8>();
    qRegisterMetaType<Okteta::Octal8>();
    qRegisterMetaType<Okteta::Hexadecimal8>();
    qRegisterMetaType<Okteta::SInt8>();
    qRegisterMetaType<Okteta::SInt16>();
    qRegisterMetaType<Okteta::SInt32>();
    qRegisterMetaType<Okteta::SInt64>();
    qRegisterMetaType<Okteta::UInt8>();
    qRegisterMetaType<Okteta::UInt16>();
    qRegisterMetaType<Okteta::UInt32>();
    qRegisterMetaType<Okteta::UInt64>();
    qRegisterMetaType<Okteta::Float32>();
    qRegisterMetaType<Okteta::Float64>();
    qRegisterMetaType<Okteta::Char8>();
    qRegisterMetaType<Okteta::Utf8>();
    qRegisterMetaType<Okteta::Utf16>();

    connect(mTool, &PODDecoderTool::readOnlyChanged, this, &PODDelegate::onReadOnlyChanged);
}

PODDelegate::~PODDelegate() = default;

// make sure only editors are created which have a readOnly property
// also beware that for subclasses of QLineEdit (all float editors) the signal
// editingFinished() is only emitted if validator() returns QValidator::Acceptable
// so onEditorDone() is not reached if QValidator::Intermediate
QWidget* PODDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QWidget* result;

    QVariant data = index.data(Qt::EditRole);
    if (data.canConvert<Okteta::Binary8>()) {
        auto* const editor = new Okteta::Binary8Editor(parent);
        connect(editor, &Okteta::Binary8Editor::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Okteta::Octal8>()) {
        auto* const editor = new Okteta::Octal8Editor(parent);
        connect(editor, &Okteta::Octal8Editor::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Okteta::Hexadecimal8>()) {
        auto* const editor = new Okteta::Hexadecimal8Editor(parent);
        connect(editor, &Okteta::Hexadecimal8Editor::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Okteta::SInt8>()) {
        Okteta::SIntSpinBox* const editor = Okteta::SIntSpinBox::createSInt8Spinbox(parent);
        connect(editor, &Okteta::SIntSpinBox::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Okteta::SInt16>()) {
        Okteta::SIntSpinBox* const editor = Okteta::SIntSpinBox::createSInt16Spinbox(parent);
        connect(editor, &Okteta::SIntSpinBox::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Okteta::SInt32>()) {
        Okteta::SIntSpinBox* const editor = Okteta::SIntSpinBox::createSInt32Spinbox(parent);
        connect(editor, &Okteta::SIntSpinBox::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Okteta::SInt64>()) {
        Okteta::SIntSpinBox* const editor = Okteta::SIntSpinBox::createSInt64Spinbox(parent);
        connect(editor, &Okteta::SIntSpinBox::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Okteta::UInt8>()) {
        Okteta::UIntSpinBox* const editor = Okteta::UIntSpinBox::createUInt8Spinbox(parent);
        editor->setBase(mTool->isUnsignedAsHex() ? 16 : 10);
        connect(editor, &Okteta::UIntSpinBox::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Okteta::UInt16>()) {
        Okteta::UIntSpinBox* const editor = Okteta::UIntSpinBox::createUInt16Spinbox(parent);
        editor->setBase(mTool->isUnsignedAsHex() ? 16 : 10);
        connect(editor, &Okteta::UIntSpinBox::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Okteta::UInt32>()) {
        Okteta::UIntSpinBox* const editor = Okteta::UIntSpinBox::createUInt32Spinbox(parent);
        editor->setBase(mTool->isUnsignedAsHex() ? 16 : 10);
        connect(editor, &Okteta::UIntSpinBox::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Okteta::UInt64>()) {
        Okteta::UIntSpinBox* const editor = Okteta::UIntSpinBox::createUInt64Spinbox(parent);
        editor->setBase(mTool->isUnsignedAsHex() ? 16 : 10);
        connect(editor, &Okteta::UIntSpinBox::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Okteta::Float32>()) {
        auto* const editor = new Okteta::Float32Editor(parent);
        connect(editor, &Okteta::Float32Editor::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Okteta::Float64>()) {
        auto* const editor = new Okteta::Float64Editor(parent);
        connect(editor, &Okteta::Float64Editor::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Okteta::Char8>()) {
        auto* const editor = new Okteta::Char8Editor(mTool->charCodec(), parent);
        connect(editor, &Okteta::Char8Editor::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Okteta::Utf8>()) {
        auto* const editor = new Okteta::Utf8Editor(parent);
        connect(editor, &Okteta::Utf8Editor::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Okteta::Utf16>()) {
        auto* const editor = new Okteta::Utf16Editor(parent);
        connect(editor, &Okteta::Utf16Editor::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else {
        result = QStyledItemDelegate::createEditor(parent, option, index);
    }

    mEditor = result;
    onReadOnlyChanged(mTool->isReadOnly());

    return result;
}

void PODDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QVariant data = index.data(Qt::EditRole);

    if (data.canConvert<Okteta::Binary8>()) {
        const auto binary8 = data.value<Okteta::Binary8>();
        auto* const binary8Editor = qobject_cast<Okteta::Binary8Editor*>(editor);
        binary8Editor->setData(binary8);
    } else if (data.canConvert<Okteta::Octal8>()) {
        const auto octal8 = data.value<Okteta::Octal8>();
        auto* const octal8Editor = qobject_cast<Okteta::Octal8Editor*>(editor);
        octal8Editor->setData(octal8);
    } else if (data.canConvert<Okteta::Hexadecimal8>()) {
        const auto hexadecimal8 = data.value<Okteta::Hexadecimal8>();
        auto* const hexadecimal8Editor = qobject_cast<Okteta::Hexadecimal8Editor*>(editor);
        hexadecimal8Editor->setData(hexadecimal8);
    } else if (data.canConvert<Okteta::SInt8>()) {
        const auto sInt8 = data.value<Okteta::SInt8>();
        auto* const sInt8Editor = qobject_cast<Okteta::SIntSpinBox*>(editor);
        sInt8Editor->setValue(sInt8.value);
    } else if (data.canConvert<Okteta::SInt16>()) {
        const auto sInt16 = data.value<Okteta::SInt16>();
        auto* const sInt16Editor = qobject_cast<Okteta::SIntSpinBox*>(editor);
        sInt16Editor->setValue(sInt16.value);
    } else if (data.canConvert<Okteta::SInt32>()) {
        const auto sInt32 = data.value<Okteta::SInt32>();
        auto* const sInt32Editor = qobject_cast<Okteta::SIntSpinBox*>(editor);
        sInt32Editor->setValue(sInt32.value);
    } else if (data.canConvert<Okteta::SInt64>()) {
        const auto sInt64 = data.value<Okteta::SInt64>();
        auto* const sInt64Editor = qobject_cast<Okteta::SIntSpinBox*>(editor);
        sInt64Editor->setValue(sInt64.value);
    } else if (data.canConvert<Okteta::UInt8>()) {
        const auto uInt8 = data.value<Okteta::UInt8>();
        auto* const uInt8Editor = qobject_cast<Okteta::UIntSpinBox*>(editor);
        uInt8Editor->setValue(uInt8.value);
    } else if (data.canConvert<Okteta::UInt16>()) {
        const auto uInt16 = data.value<Okteta::UInt16>();
        auto* const uInt16Editor = qobject_cast<Okteta::UIntSpinBox*>(editor);
        uInt16Editor->setValue(uInt16.value);
    } else if (data.canConvert<Okteta::UInt32>()) {
        const auto uInt32 = data.value<Okteta::UInt32>();
        auto* const uInt32Editor = qobject_cast<Okteta::UIntSpinBox*>(editor);
        uInt32Editor->setValue(uInt32.value);
    } else if (data.canConvert<Okteta::UInt64>()) {
        const auto uInt64 = data.value<Okteta::UInt64>();
        auto* const uInt64Editor = qobject_cast<Okteta::UIntSpinBox*>(editor);
        uInt64Editor->setValue(uInt64.value);
    } else if (data.canConvert<Okteta::Float32>()) {
        const auto float32 = data.value<Okteta::Float32>();
        auto* const float32Editor = qobject_cast<Okteta::Float32Editor*>(editor);
        float32Editor->setData(float32);
    } else if (data.canConvert<Okteta::Float64>()) {
        const auto float64 = data.value<Okteta::Float64>();
        auto* const float64Editor = qobject_cast<Okteta::Float64Editor*>(editor);
        float64Editor->setData(float64);
    } else if (data.canConvert<Okteta::Char8>()) {
        const auto char8 = data.value<Okteta::Char8>();
        auto* const char8Editor = qobject_cast<Okteta::Char8Editor*>(editor);
        char8Editor->setData(char8);
    } else if (data.canConvert<Okteta::Utf8>()) {
        const auto utf8 = data.value<Okteta::Utf8>();
        auto* const utf8Editor = qobject_cast<Okteta::Utf8Editor*>(editor);
        utf8Editor->setData(utf8);
    } else if (data.canConvert<Okteta::Utf16>()) {
        const auto utf16 = data.value<Okteta::Utf16>();
        auto* const utf16Editor = qobject_cast<Okteta::Utf16Editor*>(editor);
        utf16Editor->setData(utf16);
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void PODDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    if (mTool->isReadOnly()) {
        return;
    }

    QVariant data = index.data(Qt::EditRole);

    if (data.canConvert<Okteta::Binary8>()) {
        auto* const binary8Editor = qobject_cast<Okteta::Binary8Editor*>(editor);
        model->setData(index, QVariant::fromValue(binary8Editor->data()));
    } else if (data.canConvert<Okteta::Octal8>()) {
        auto* const octal8Editor = qobject_cast<Okteta::Octal8Editor*>(editor);
        model->setData(index, QVariant::fromValue(octal8Editor->data()));
    } else if (data.canConvert<Okteta::Hexadecimal8>()) {
        auto* const hexadecimal8Editor = qobject_cast<Okteta::Hexadecimal8Editor*>(editor);
        model->setData(index, QVariant::fromValue(hexadecimal8Editor->data()));
    } else if (data.canConvert<Okteta::SInt8>()) {
        auto* const sintEditor = qobject_cast<Okteta::SIntSpinBox*>(editor);
        model->setData(index, QVariant::fromValue(Okteta::SInt8(sintEditor->value())));
    } else if (data.canConvert<Okteta::SInt16>()) {
        auto* const sintEditor = qobject_cast<Okteta::SIntSpinBox*>(editor);
        model->setData(index, QVariant::fromValue(Okteta::SInt16(sintEditor->value())));
    } else if (data.canConvert<Okteta::SInt32>()) {
        auto* const sintEditor = qobject_cast<Okteta::SIntSpinBox*>(editor);
        model->setData(index, QVariant::fromValue(Okteta::SInt32(sintEditor->value())));
    } else if (data.canConvert<Okteta::SInt64>()) {
        auto* const sintEditor = qobject_cast<Okteta::SIntSpinBox*>(editor);
        model->setData(index, QVariant::fromValue(Okteta::SInt64(sintEditor->value())));
    } else if (data.canConvert<Okteta::UInt8>()) {
        auto* const uintEditor = qobject_cast<Okteta::UIntSpinBox*>(editor);
        model->setData(index, QVariant::fromValue(Okteta::UInt8(uintEditor->value())));
    } else if (data.canConvert<Okteta::UInt16>()) {
        auto* const uintEditor = qobject_cast<Okteta::UIntSpinBox*>(editor);
        model->setData(index, QVariant::fromValue(Okteta::UInt16(uintEditor->value())));
    } else if (data.canConvert<Okteta::UInt32>()) {
        auto* const uintEditor = qobject_cast<Okteta::UIntSpinBox*>(editor);
        model->setData(index, QVariant::fromValue(Okteta::UInt32(uintEditor->value())));
    } else if (data.canConvert<Okteta::UInt64>()) {
        auto* const uintEditor = qobject_cast<Okteta::UIntSpinBox*>(editor);
        model->setData(index, QVariant::fromValue(Okteta::UInt64(uintEditor->value())));
    } else if (data.canConvert<Okteta::Float32>()) {
        auto* const float32Editor = qobject_cast<Okteta::Float32Editor*>(editor);
        model->setData(index, QVariant::fromValue(float32Editor->data()));
    } else if (data.canConvert<Okteta::Float64>()) {
        auto* const float64Editor = qobject_cast<Okteta::Float64Editor*>(editor);
        model->setData(index, QVariant::fromValue(float64Editor->data()));
    } else if (data.canConvert<Okteta::Char8>()) {
        auto* const char8Editor = qobject_cast<Okteta::Char8Editor*>(editor);
        model->setData(index, QVariant::fromValue(char8Editor->data()));
    } else if (data.canConvert<Okteta::Utf8>()) {
        auto* const utf8Editor = qobject_cast<Okteta::Utf8Editor*>(editor);
        model->setData(index, QVariant::fromValue(utf8Editor->data()));
    } else if (data.canConvert<Okteta::Utf16>()) {
        auto* const utf16Editor = qobject_cast<Okteta::Utf16Editor*>(editor);
        model->setData(index, QVariant::fromValue(utf16Editor->data()));
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

// QSize PODDelegate::updateEditorGeometry( QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index ) const
// {
//      editor->setGeometry(option.rect);
// }

void PODDelegate::onEditorDone()
{
    auto* const editor = qobject_cast<QWidget*>(sender());
    mEditor = nullptr;
    Q_EMIT commitData(editor);
    Q_EMIT closeEditor(editor);
}

void PODDelegate::onReadOnlyChanged(bool isReadOnly) const
{
    if (mEditor) {
        // going by property is slower, but saves writing this call for every widget
        mEditor->setProperty("readOnly", isReadOnly);
    }
}

}

#include "moc_poddelegate.cpp"
