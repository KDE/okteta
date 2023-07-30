/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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
// so that instead of this unflexible maintanance mess^WWWunlooped code QItemEditorCreator can be used!

namespace Kasten {

PODDelegate::PODDelegate(PODDecoderTool* tool, QObject* parent)
    : QStyledItemDelegate(parent)
    , mTool(tool)
{
    qRegisterMetaType<Binary8>();
    qRegisterMetaType<Octal8>();
    qRegisterMetaType<Hexadecimal8>();
    qRegisterMetaType<SInt8>();
    qRegisterMetaType<SInt16>();
    qRegisterMetaType<SInt32>();
    qRegisterMetaType<SInt64>();
    qRegisterMetaType<UInt8>();
    qRegisterMetaType<UInt16>();
    qRegisterMetaType<UInt32>();
    qRegisterMetaType<UInt64>();
    qRegisterMetaType<Float32>();
    qRegisterMetaType<Float64>();
    qRegisterMetaType<Char8>();
    qRegisterMetaType<Utf8>();
    qRegisterMetaType<Utf16>();

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
    if (data.canConvert<Binary8>()) {
        auto* editor = new Binary8Editor(parent);
        connect(editor, &Binary8Editor::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Octal8>()) {
        auto* editor = new Octal8Editor(parent);
        connect(editor, &Octal8Editor::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Hexadecimal8>()) {
        auto* editor = new Hexadecimal8Editor(parent);
        connect(editor, &Hexadecimal8Editor::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<SInt8>()) {
        SIntSpinBox* editor = SIntSpinBox::createSInt8Spinbox(parent);
        connect(editor, &SIntSpinBox::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<SInt16>()) {
        SIntSpinBox* editor = SIntSpinBox::createSInt16Spinbox(parent);
        connect(editor, &SIntSpinBox::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<SInt32>()) {
        SIntSpinBox* editor = SIntSpinBox::createSInt32Spinbox(parent);
        connect(editor, &SIntSpinBox::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<SInt64>()) {
        SIntSpinBox* editor = SIntSpinBox::createSInt64Spinbox(parent);
        connect(editor, &SIntSpinBox::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<UInt8>()) {
        UIntSpinBox* editor = UIntSpinBox::createUInt8Spinbox(parent);
        editor->setBase(mTool->isUnsignedAsHex() ? 16 : 10);
        connect(editor, &UIntSpinBox::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<UInt16>()) {
        UIntSpinBox* editor = UIntSpinBox::createUInt16Spinbox(parent);
        editor->setBase(mTool->isUnsignedAsHex() ? 16 : 10);
        connect(editor, &UIntSpinBox::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<UInt32>()) {
        UIntSpinBox* editor = UIntSpinBox::createUInt32Spinbox(parent);
        editor->setBase(mTool->isUnsignedAsHex() ? 16 : 10);
        connect(editor, &UIntSpinBox::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<UInt64>()) {
        UIntSpinBox* editor = UIntSpinBox::createUInt64Spinbox(parent);
        editor->setBase(mTool->isUnsignedAsHex() ? 16 : 10);
        connect(editor, &UIntSpinBox::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Float32>()) {
        auto* editor = new Float32Editor(parent);
        connect(editor, &Float32Editor::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Float64>()) {
        auto* editor = new Float64Editor(parent);
        connect(editor, &Float64Editor::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Char8>()) {
        auto* editor = new Char8Editor(mTool->charCodec(), parent);
        connect(editor, &Char8Editor::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Utf8>()) {
        auto* editor = new Utf8Editor(parent);
        connect(editor, &Utf8Editor::editingFinished,
                this, &PODDelegate::onEditorDone);
        result = editor;
    } else if (data.canConvert<Utf16>()) {
        auto* editor = new Utf16Editor(parent);
        connect(editor, &Utf16Editor::editingFinished,
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

    if (data.canConvert<Binary8>()) {
        Binary8 binary8 = data.value<Binary8>();
        auto* binary8Editor = qobject_cast<Binary8Editor*>(editor);
        binary8Editor->setData(binary8);
    } else if (data.canConvert<Octal8>()) {
        Octal8 octal8 = data.value<Octal8>();
        auto* octal8Editor = qobject_cast<Octal8Editor*>(editor);
        octal8Editor->setData(octal8);
    } else if (data.canConvert<Hexadecimal8>()) {
        Hexadecimal8 hexadecimal8 = data.value<Hexadecimal8>();
        auto* hexadecimal8Editor = qobject_cast<Hexadecimal8Editor*>(editor);
        hexadecimal8Editor->setData(hexadecimal8);
    } else if (data.canConvert<SInt8>()) {
        SInt8 sInt8 = data.value<SInt8>();
        auto* sInt8Editor = qobject_cast<SIntSpinBox*>(editor);
        sInt8Editor->setValue(sInt8.value);
    } else if (data.canConvert<SInt16>()) {
        SInt16 sInt16 = data.value<SInt16>();
        auto* sInt16Editor = qobject_cast<SIntSpinBox*>(editor);
        sInt16Editor->setValue(sInt16.value);
    } else if (data.canConvert<SInt32>()) {
        SInt32 sInt32 = data.value<SInt32>();
        auto* sInt32Editor = qobject_cast<SIntSpinBox*>(editor);
        sInt32Editor->setValue(sInt32.value);
    } else if (data.canConvert<SInt64>()) {
        SInt64 sInt64 = data.value<SInt64>();
        auto* sInt64Editor = qobject_cast<SIntSpinBox*>(editor);
        sInt64Editor->setValue(sInt64.value);
    } else if (data.canConvert<UInt8>()) {
        UInt8 uInt8 = data.value<UInt8>();
        auto* uInt8Editor = qobject_cast<UIntSpinBox*>(editor);
        uInt8Editor->setValue(uInt8.value);
    } else if (data.canConvert<UInt16>()) {
        UInt16 uInt16 = data.value<UInt16>();
        auto* uInt16Editor = qobject_cast<UIntSpinBox*>(editor);
        uInt16Editor->setValue(uInt16.value);
    } else if (data.canConvert<UInt32>()) {
        UInt32 uInt32 = data.value<UInt32>();
        auto* uInt32Editor = qobject_cast<UIntSpinBox*>(editor);
        uInt32Editor->setValue(uInt32.value);
    } else if (data.canConvert<UInt64>()) {
        UInt64 uInt64 = data.value<UInt64>();
        auto* uInt64Editor = qobject_cast<UIntSpinBox*>(editor);
        uInt64Editor->setValue(uInt64.value);
    } else if (data.canConvert<Float32>()) {
        Float32 float32 = data.value<Float32>();
        auto* float32Editor = qobject_cast<Float32Editor*>(editor);
        float32Editor->setData(float32);
    } else if (data.canConvert<Float64>()) {
        Float64 float64 = data.value<Float64>();
        auto* float64Editor = qobject_cast<Float64Editor*>(editor);
        float64Editor->setData(float64);
    } else if (data.canConvert<Char8>()) {
        Char8 char8 = data.value<Char8>();
        auto* char8Editor = qobject_cast<Char8Editor*>(editor);
        char8Editor->setData(char8);
    } else if (data.canConvert<Utf8>()) {
        Utf8 utf8 = data.value<Utf8>();
        auto* utf8Editor = qobject_cast<Utf8Editor*>(editor);
        utf8Editor->setData(utf8);
    } else if (data.canConvert<Utf16>()) {
        Utf16 utf16 = data.value<Utf16>();
        auto* utf16Editor = qobject_cast<Utf16Editor*>(editor);
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

    if (data.canConvert<Binary8>()) {
        auto* binary8Editor = qobject_cast<Binary8Editor*>(editor);
        model->setData(index, QVariant::fromValue(binary8Editor->data()));
    } else if (data.canConvert<Octal8>()) {
        auto* octal8Editor = qobject_cast<Octal8Editor*>(editor);
        model->setData(index, QVariant::fromValue(octal8Editor->data()));
    } else if (data.canConvert<Hexadecimal8>()) {
        auto* hexadecimal8Editor = qobject_cast<Hexadecimal8Editor*>(editor);
        model->setData(index, QVariant::fromValue(hexadecimal8Editor->data()));
    } else if (data.canConvert<SInt8>()) {
        auto* sintEditor = qobject_cast<SIntSpinBox*>(editor);
        model->setData(index, QVariant::fromValue(SInt8(sintEditor->value())));
    } else if (data.canConvert<SInt16>()) {
        auto* sintEditor = qobject_cast<SIntSpinBox*>(editor);
        model->setData(index, QVariant::fromValue(SInt16(sintEditor->value())));
    } else if (data.canConvert<SInt32>()) {
        auto* sintEditor = qobject_cast<SIntSpinBox*>(editor);
        model->setData(index, QVariant::fromValue(SInt32(sintEditor->value())));
    } else if (data.canConvert<SInt64>()) {
        auto* sintEditor = qobject_cast<SIntSpinBox*>(editor);
        model->setData(index, QVariant::fromValue(SInt64(sintEditor->value())));
    } else if (data.canConvert<UInt8>()) {
        auto* uintEditor = qobject_cast<UIntSpinBox*>(editor);
        model->setData(index, QVariant::fromValue(UInt8(uintEditor->value())));
    } else if (data.canConvert<UInt16>()) {
        auto* uintEditor = qobject_cast<UIntSpinBox*>(editor);
        model->setData(index, QVariant::fromValue(UInt16(uintEditor->value())));
    } else if (data.canConvert<UInt32>()) {
        auto* uintEditor = qobject_cast<UIntSpinBox*>(editor);
        model->setData(index, QVariant::fromValue(UInt32(uintEditor->value())));
    } else if (data.canConvert<UInt64>()) {
        auto* uintEditor = qobject_cast<UIntSpinBox*>(editor);
        model->setData(index, QVariant::fromValue(UInt64(uintEditor->value())));
    } else if (data.canConvert<Float32>()) {
        auto* float32Editor = qobject_cast<Float32Editor*>(editor);
        model->setData(index, QVariant::fromValue(float32Editor->data()));
    } else if (data.canConvert<Float64>()) {
        auto* float64Editor = qobject_cast<Float64Editor*>(editor);
        model->setData(index, QVariant::fromValue(float64Editor->data()));
    } else if (data.canConvert<Char8>()) {
        auto* char8Editor = qobject_cast<Char8Editor*>(editor);
        model->setData(index, QVariant::fromValue(char8Editor->data()));
    } else if (data.canConvert<Utf8>()) {
        auto* utf8Editor = qobject_cast<Utf8Editor*>(editor);
        model->setData(index, QVariant::fromValue(utf8Editor->data()));
    } else if (data.canConvert<Utf16>()) {
        auto* utf16Editor = qobject_cast<Utf16Editor*>(editor);
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
    QWidget* editor = qobject_cast<QWidget*>(sender());
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
