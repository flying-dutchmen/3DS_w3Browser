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

#ifndef _WKC_HELPERS_PRIVATE_QUALIFIEDNAME_H_
#define _WKC_HELPERS_PRIVATE_QUALIFIEDNAME_H_

#ifdef WKC_ADD_WKC_HELPERS

#include "PlatformString.h"

namespace WebCore {
class QualifiedName;
} // namespace

namespace WebCore {
class AtomicString;
}

namespace WKC {
class QualifiedName;
class AtomicString;
} // namespace

namespace WKC {

class QualifiedNamePrivate {
public:
    QualifiedNamePrivate(const WKC::AtomicString&, const String&, const WKC::AtomicString&);
    ~QualifiedNamePrivate();

    WebCore::QualifiedName* webcore() const { return m_webcore; }

private:
    WebCore::QualifiedName* m_webcore;
    WebCore::String m_webcore_local_name;
    WebCore::AtomicString* m_webcore_prefix;
    WebCore::AtomicString* m_webcore_namespace;
};
} // namespace

#endif // WKC_ADD_WKC_HELPERS

#endif //_WKC_HELPERS_PRIVATE_QUALIFIEDNAME_H_