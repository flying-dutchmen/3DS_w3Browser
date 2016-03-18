/*
 *  Copyright (c) 2011-2013 ACCESS CO., LTD. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * 
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */

#ifndef _WKC_HELPERS_PRIVATE_ATOMICSTRING_H_
#define _WKC_HELPERS_PRIVATE_ATOMICSTRING_H_

#ifdef WKC_ADD_WKC_HELPERS

#include "helpers/WKCAtomicString.h"
#include "helpers/WKCString.h"

namespace WebCore {
class String;
class AtomicString;
} // namespace

namespace WKC {

class WKC_API AtomicStringPrivate {
public:
    AtomicStringPrivate();
    AtomicStringPrivate(WebCore::AtomicString* value);
    ~AtomicStringPrivate();

    String& string();
    WebCore::AtomicString* webcore(){ return m_webcore; }
    AtomicString& wkc() const { return *m_wkc; }

private:
    WebCore::AtomicString* m_webcore;
    bool m_webcoreowner;
    AtomicString* m_wkc;
    String m_wkc_string;
};

} // namespace

#endif // WKC_ADD_WKC_HELPERS

#endif // _WKC_HELPERS_PRIVATE_ATOMICSTRING_H_
