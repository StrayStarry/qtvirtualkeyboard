/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://www.qt.io
**
** This file is part of the Qt Virtual Keyboard add-on for Qt Enterprise.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** contact form at http://www.qt.io
**
****************************************************************************/

#include "abstractinputmethod.h"

AbstractInputMethodPrivate::AbstractInputMethodPrivate() :
    QObjectPrivate(),
    inputEngine(0)
{
}

/*!
    \class AbstractInputMethod

    \inmodule qtvirtualkeyboard

    \brief The base class for input methods.

    Use this class if you want to implement a custom input
    method using C/C++ language.
*/

/*!
    Constructs an input method with \a dd as the private data
    from the derived class and \a parent as the parent.
*/
AbstractInputMethod::AbstractInputMethod(AbstractInputMethodPrivate &dd, QObject *parent) :
    QObject(dd, parent)
{
}

/*!
    Constructs an input method with \a parent.
*/
AbstractInputMethod::AbstractInputMethod(QObject *parent) :
    QObject(*new AbstractInputMethodPrivate(), parent)
{
}

/*!
    Destroys the input method and frees all allocated resources.
*/
AbstractInputMethod::~AbstractInputMethod()
{
}

/*!
    Returns the input context associated with the input method.
    This method returns \c NULL if the input method is not active.
*/
DeclarativeInputContext *AbstractInputMethod::inputContext() const
{
    Q_D(const AbstractInputMethod);
    return d->inputEngine ? d->inputEngine->inputContext() : 0;
}

/*!
    Returns the input engine associated with the input method.
    This method returns \c NULL if the input method is not active.
*/
DeclarativeInputEngine *AbstractInputMethod::inputEngine() const
{
    Q_D(const AbstractInputMethod);
    return d->inputEngine;
}

/*!
    This method is called by the input engine when the input method needs
    to be reset. The input method must reset its internal state only. The main
    difference to the update() method is that reset() modifies only
    the input method state, i.e. it must not modify the input context.
*/
void AbstractInputMethod::reset()
{
}

/*!
    This method is called by the input engine when the input method needs to be
    updated. The input method must close the current pre-edit text and
    restore its internal state to the default.
*/
void AbstractInputMethod::update()
{
}

/*!
    \internal
    Called by the input engine when the input method is activated and
    deactivated.
*/
void AbstractInputMethod::setInputEngine(DeclarativeInputEngine *inputEngine)
{
    Q_D(AbstractInputMethod);
    if (d->inputEngine) {
        d->inputEngine->disconnect(this, SLOT(reset()));
        d->inputEngine->disconnect(this, SLOT(update()));
    }
    d->inputEngine = inputEngine;
    if (d->inputEngine) {
        connect(d->inputEngine, SIGNAL(inputMethodReset()), SLOT(reset()));
        connect(d->inputEngine, SIGNAL(inputMethodUpdate()), SLOT(update()));
    }
}

QList<DeclarativeSelectionListModel::Type> AbstractInputMethod::selectionLists()
{
    return QList<DeclarativeSelectionListModel::Type>();
}

int AbstractInputMethod::selectionListItemCount(DeclarativeSelectionListModel::Type type)
{
    Q_UNUSED(type)
    return 0;
}

QVariant AbstractInputMethod::selectionListData(DeclarativeSelectionListModel::Type type, int index, int role)
{
    Q_UNUSED(type)
    Q_UNUSED(index)
    switch (role) {
    case DeclarativeSelectionListModel::DisplayRole:
        return QVariant("");
    case DeclarativeSelectionListModel::WordCompletionLengthRole:
        return QVariant(0);
    }
    return QVariant();
}

void AbstractInputMethod::selectionListItemSelected(DeclarativeSelectionListModel::Type type, int index)
{
    Q_UNUSED(type)
    Q_UNUSED(index)
}

/*!
    \since QtQuick.Enterprise.VirtualKeyboard 2.0

    Returns list of supported pattern recognition modes.

    This method is called by the input engine to query the list of
    supported pattern recognition modes.
*/
QList<DeclarativeInputEngine::PatternRecognitionMode> AbstractInputMethod::patternRecognitionModes() const
{
    return QList<DeclarativeInputEngine::PatternRecognitionMode>();
}

/*!
    \since QtQuick.Enterprise.VirtualKeyboard 2.0

    This method is called when a trace interaction starts with the specified \a patternRecognitionMode.
    The trace is uniquely identified by the \a traceId.
    The \a traceCaptureDeviceInfo provides information about the source device and the
    \a traceScreenInfo provides information about the screen context.

    If the input method accepts the event and wants to capture the trace input, it must return
    a new Trace object. This object must remain valid until the traceEnd() method is called. If the
    Trace is rendered on screen, it remains there until the Trace object is destroyed.
*/
DeclarativeTrace *AbstractInputMethod::traceBegin(int traceId, DeclarativeInputEngine::PatternRecognitionMode patternRecognitionMode,
                                                  const QVariantMap &traceCaptureDeviceInfo, const QVariantMap &traceScreenInfo)
{
    Q_UNUSED(traceId)
    Q_UNUSED(patternRecognitionMode)
    Q_UNUSED(traceCaptureDeviceInfo)
    Q_UNUSED(traceScreenInfo)
    return 0;
}

/*!
    \since QtQuick.Enterprise.VirtualKeyboard 2.0

    This method is called when the trace interaction ends. The input method should destroy the \a trace object
    at some point after this function is called. See the \l Trace API how to access the gathered
    data.

    The method returns \c true if the trace interaction was accepted.
*/
bool AbstractInputMethod::traceEnd(DeclarativeTrace *trace)
{
    Q_UNUSED(trace)
    return false;
}

/*!
    \fn QList<DeclarativeInputEngine::InputMode> AbstractInputMethod::inputModes(const QString& locale)

    Returns the list of input modes for \a locale.
*/

/*!
    \fn bool AbstractInputMethod::setInputMode(const QString& locale, DeclarativeInputEngine::InputMode inputMode)

    Sets the \a inputMode and \a locale for this input method. Returns \c true
    if successful.
*/

/*!
    \fn bool AbstractInputMethod::setTextCase(DeclarativeInputEngine::TextCase textCase)

    Updates the \a textCase for this input method. The method returns \c true
    if successful.
*/

/*!
    \fn bool AbstractInputMethod::keyEvent(Qt::Key key, const QString& text, Qt::KeyboardModifiers modifiers)

    The purpose of this method is to handle the key events generated by the the
    input engine.

    The \a key parameter specifies the code of the key to handle. The key code
    does not distinguish between capital and non-capital letters. The \a
    text parameter contains the Unicode text for the key. The \a modifiers
    parameter contains the key modifiers that apply to key.

    This method returns \c true if the key event was successfully handled.
    If the return value is \c false, the key event is redirected to the default
    input method for futher processing.
*/

/*!
    \fn QList<DeclarativeSelectionListModel::Type> AbstractInputMethod::selectionLists()

    Returns the list of selection lists used by this input method.

    This method is called by input engine when the input method is being
    activated. The input method can reserve the selection lists for its use
    by returning a list of selection list types required.
*/

/*!
    \fn int AbstractInputMethod::selectionListItemCount(DeclarativeSelectionListModel::Type type)

    Returns the number of items in the selection list identified by \a type.
*/

/*!
    \fn QVariant AbstractInputMethod::selectionListData(DeclarativeSelectionListModel::Type type, int index, int role)

    Returns item data for the selection list identified by \a type. The \a role
    parameter specifies which data is requested. The \a index parameter is a
    zero based index into the list.
*/

/*!
    \fn void AbstractInputMethod::selectionListItemSelected(DeclarativeSelectionListModel::Type type, int index)

    This method is called when an item at \a index has been selected by the
    user. The selection list is identified by the \a type parameter.
*/

/*!
    \fn void AbstractInputMethod::selectionListChanged(int type)

    The input method emits this signal when the contents of the selection list
    has changed. The \a type parameter specifies which selection list has
    changed.
*/

/*!
    \fn void AbstractInputMethod::selectionListActiveItemChanged(int type, int index)

    The input method emits this signal when the current \a index has changed
    in the selection list identified by \a type.
*/
