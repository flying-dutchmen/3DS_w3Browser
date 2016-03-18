/*
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2003, 2008, 2009 Apple Inc. All rights reserved.
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

#ifndef JSLazyEventListener_h
#define JSLazyEventListener_h

#include "JSEventListener.h"
#include "PlatformString.h"

namespace WebCore {

    class Node;

    class JSLazyEventListener : public JSEventListener {
    public:
#ifdef WKC_FIX_EVENT_HANDLER_REMOVAL // webkit.org trunk r54400
        static PassRefPtr<JSLazyEventListener> create(const String& functionName, const String& eventParameterName, const String& code, Node* node, const String& sourceURL, int lineNumber, JSC::JSObject* wrapper, DOMWrapperWorld* isolatedWorld)
#else
        static PassRefPtr<JSLazyEventListener> create(const String& functionName, const String& eventParameterName, const String& code, Node* node, const String& sourceURL, int lineNumber, DOMWrapperWorld* isolatedWorld)
#endif
        {
#ifdef WKC_FIX_EVENT_HANDLER_REMOVAL // webkit.org trunk r54400
            return adoptRef(new JSLazyEventListener(functionName, eventParameterName, code, node, sourceURL, lineNumber, wrapper, isolatedWorld));
#else
            return adoptRef(new JSLazyEventListener(functionName, eventParameterName, code, node, sourceURL, lineNumber, isolatedWorld));
#endif
        }
        virtual ~JSLazyEventListener();
#if PLATFORM(WKC)
        static void deleteSharedInstance();
        static void resetVariables();
#endif

    private:
#ifdef WKC_FIX_EVENT_HANDLER_REMOVAL // webkit.org trunk r54400
        JSLazyEventListener(const String& functionName, const String& eventParameterName, const String& code, Node*, const String& sourceURL, int lineNumber, JSC::JSObject* wrapper, DOMWrapperWorld* isolatedWorld);
#else
        JSLazyEventListener(const String& functionName, const String& eventParameterName, const String& code, Node*, const String& sourceURL, int lineNumber, DOMWrapperWorld* isolatedWorld);
#endif

#ifdef WKC_FIX_EVENT_HANDLER_REMOVAL // webkit.org trunk r54400
        virtual JSC::JSObject* initializeJSFunction(ScriptExecutionContext*) const;
#else
        virtual JSC::JSObject* jsFunction(ScriptExecutionContext*) const;
#endif
        virtual bool wasCreatedFromMarkup() const { return true; }

#ifndef WKC_FIX_EVENT_HANDLER_REMOVAL // webkit.org trunk r54400
        void parseCode(ScriptExecutionContext*) const;
#endif

        mutable String m_functionName;
        mutable String m_eventParameterName;
        mutable String m_code;
#ifndef WKC_FIX_EVENT_HANDLER_REMOVAL // webkit.org trunk r54400
        mutable bool m_parsed;
#endif
        mutable String m_sourceURL;
        int m_lineNumber;
        Node* m_originalNode;
    };

} // namespace WebCore

#endif // JSEventListener_h
