/*
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All Rights Reserved.
 *  Copyright (c) 2010 ACCESS CO., LTD. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"
#include "JSLazyEventListener.h"

#include "Frame.h"
#include "JSNode.h"
#include <runtime/FunctionConstructor.h>
#include <runtime/JSFunction.h>
#include <runtime/JSLock.h>
#include <wtf/RefCountedLeakCounter.h>

using namespace JSC;

namespace WebCore {

#ifndef NDEBUG
#if PLATFORM(WKC)
static WTF::RefCountedLeakCounter* gEventListenerCounter = 0;
#else
static WTF::RefCountedLeakCounter eventListenerCounter("JSLazyEventListener");
#endif
#endif

#ifdef WKC_FIX_EVENT_HANDLER_REMOVAL // webkit.org trunk r54400
JSLazyEventListener::JSLazyEventListener(const String& functionName, const String& eventParameterName, const String& code, Node* node, const String& sourceURL, int lineNumber, JSObject* wrapper, DOMWrapperWorld* isolatedWorld)
    : JSEventListener(0, wrapper, true, isolatedWorld)
#else
JSLazyEventListener::JSLazyEventListener(const String& functionName, const String& eventParameterName, const String& code, Node* node, const String& sourceURL, int lineNumber, DOMWrapperWorld* isolatedWorld)
    : JSEventListener(0, true, isolatedWorld)
#endif
    , m_functionName(functionName)
    , m_eventParameterName(eventParameterName)
    , m_code(code)
#ifndef WKC_FIX_EVENT_HANDLER_REMOVAL // webkit.org trunk r54400
    , m_parsed(false)
#endif
    , m_sourceURL(sourceURL)
    , m_lineNumber(lineNumber)
    , m_originalNode(node)
{
    // We don't retain the original node because we assume it
    // will stay alive as long as this handler object is around
    // and we need to avoid a reference cycle. If JS transfers
#ifdef WKC_FIX_EVENT_HANDLER_REMOVAL // webkit.org trunk r54400
    // this handler to another node, initializeJSFunction will
    // be called and then originalNode is no longer needed.
#else
    // this handler to another node, parseCode will be called and
    // then originalNode is no longer needed.
#endif

    // A JSLazyEventListener can be created with a line number of zero when it is created with
    // a setAttribute call from JavaScript, so make the line number 1 in that case.
    if (m_lineNumber == 0)
        m_lineNumber = 1;

#ifndef NDEBUG
#if PLATFORM(WKC)
    if (!gEventListenerCounter) {
        gEventListenerCounter = new WTF::RefCountedLeakCounter("JSLazyEventListener");
    }
    WTF::RefCountedLeakCounter& eventListenerCounter = *gEventListenerCounter;
#endif
    eventListenerCounter.increment();
#endif
}

JSLazyEventListener::~JSLazyEventListener()
{
#ifndef NDEBUG
#if PLATFORM(WKC)
    WTF::RefCountedLeakCounter& eventListenerCounter = *gEventListenerCounter;
#endif
    eventListenerCounter.decrement();
#endif
}

#ifndef WKC_FIX_EVENT_HANDLER_REMOVAL // webkit.org trunk r54400
JSObject* JSLazyEventListener::jsFunction(ScriptExecutionContext* executionContext) const
{
    parseCode(executionContext);
    return m_jsFunction;
}
#endif

#ifdef WKC_FIX_EVENT_HANDLER_REMOVAL // webkit.org trunk r54400
JSObject* JSLazyEventListener::initializeJSFunction(ScriptExecutionContext* executionContext) const
{
    ASSERT(executionContext);
    ASSERT(executionContext->isDocument());
    if (!executionContext)
        return 0;

    Frame* frame = static_cast<Document*>(executionContext)->frame();
    if (!frame)
        return 0;

    ScriptController* scriptController = frame->script();
    if (!scriptController->isEnabled()) // reverted r53046 to build successfully
        return 0;

    JSDOMGlobalObject* globalObject = toJSDOMGlobalObject(executionContext, isolatedWorld());
    if (!globalObject)
        return 0;

    if (executionContext->isDocument()) {
        JSDOMWindow* window = static_cast<JSDOMWindow*>(globalObject);
        Frame* frame = window->impl()->frame();
        if (!frame)
            return 0;
        // FIXME: Is this check needed for non-Document contexts?
        ScriptController* script = frame->script();
        if (!script->isEnabled() || script->isPaused()) // reverted r53046 to build successfully
            return 0;
    }

    ExecState* exec = globalObject->globalExec();

    MarkedArgumentBuffer args;
    args.append(jsNontrivialString(exec, m_eventParameterName));
    args.append(jsString(exec, m_code));

    JSObject* jsFunction = constructFunction(exec, args, Identifier(exec, m_functionName), m_sourceURL, m_lineNumber); // FIXME: is globalExec ok?
    if (exec->hadException()) {
        exec->clearException();
        return 0;
    }

    JSFunction* listenerAsFunction = static_cast<JSFunction*>(jsFunction);
    if (m_originalNode) {
        if (!wrapper()) {
            // Ensure that 'node' has a JavaScript wrapper to mark the event listener we're creating.
            JSLock lock(SilenceAssertionsOnly);
            // FIXME: Should pass the global object associated with the node
            setWrapper(asObject(toJS(globalObject->globalExec(), globalObject, m_originalNode)));
        }

        // Add the event's home element to the scope
        // (and the document, and the form - see JSHTMLElement::eventHandlerScope)
        ScopeChain scope = listenerAsFunction->scope();
        static_cast<JSNode*>(wrapper())->pushEventHandlerScope(exec, scope);
        listenerAsFunction->setScope(scope);
    }

    // Since we only parse once, there's no need to keep data used for parsing around anymore.
    m_functionName = String();
    m_code = String();
    m_eventParameterName = String();
    m_sourceURL = String();
    return jsFunction;
}
#else
void JSLazyEventListener::parseCode(ScriptExecutionContext* executionContext) const
{
    ASSERT(executionContext);
    ASSERT(executionContext->isDocument());
    if (!executionContext)
        return;

    if (m_parsed)
        return;

    Frame* frame = static_cast<Document*>(executionContext)->frame();
    if (!frame)
        return;

    ScriptController* scriptController = frame->script();
    if (!scriptController->isEnabled())
        return;

    JSDOMGlobalObject* globalObject = toJSDOMGlobalObject(executionContext, m_isolatedWorld.get());
    if (!globalObject)
        return;

    // Ensure that 'node' has a JavaScript wrapper to mark the event listener we're creating.
    if (m_originalNode) {
        JSLock lock(SilenceAssertionsOnly);
        // FIXME: Should pass the global object associated with the node
        toJS(globalObject->globalExec(), globalObject, m_originalNode);
    }

    if (executionContext->isDocument()) {
        JSDOMWindow* window = static_cast<JSDOMWindow*>(globalObject);
        Frame* frame = window->impl()->frame();
        if (!frame)
            return;
        // FIXME: Is this check needed for non-Document contexts?
        ScriptController* script = frame->script();
        if (!script->isEnabled() || script->isPaused())
            return;
    }

    m_parsed = true;

    ExecState* exec = globalObject->globalExec();

    MarkedArgumentBuffer args;
    args.append(jsNontrivialString(exec, m_eventParameterName));
    args.append(jsString(exec, m_code));

    m_jsFunction = constructFunction(exec, args, Identifier(exec, m_functionName), m_sourceURL, m_lineNumber); // FIXME: is globalExec ok?

    JSFunction* listenerAsFunction = static_cast<JSFunction*>(m_jsFunction);

    if (exec->hadException()) {
        exec->clearException();

        // failed to parse, so let's just make this listener a no-op
        m_jsFunction = 0;
    } else if (m_originalNode) {
        // Add the event's home element to the scope
        // (and the document, and the form - see JSHTMLElement::eventHandlerScope)
        ScopeChain scope = listenerAsFunction->scope();

        JSValue thisObj = toJS(exec, globalObject, m_originalNode);
        if (thisObj.isObject()) {
            static_cast<JSNode*>(asObject(thisObj))->pushEventHandlerScope(exec, scope);
            listenerAsFunction->setScope(scope);
        }
    }

    // Since we only parse once, there's no need to keep data used for parsing around anymore.
    m_functionName = String();
    m_code = String();
    m_eventParameterName = String();
    m_sourceURL = String();
}
#endif

#if PLATFORM(WKC)
void JSLazyEventListener::deleteSharedInstance()
{
#ifndef NDEBUG
    delete gEventListenerCounter;
#endif
}

void JSLazyEventListener::resetVariables()
{
#ifndef NDEBUG
    gEventListenerCounter = 0;
#endif
}
#endif

} // namespace WebCore
