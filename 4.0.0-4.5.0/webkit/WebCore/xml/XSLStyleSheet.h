/*
 * This file is part of the XSL implementation.
 *
 * Copyright (C) 2004, 2006, 2008 Apple Inc. All rights reserved.
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
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef XSLStyleSheet_h
#define XSLStyleSheet_h

#if ENABLE(XSLT)

#include "StyleSheet.h"

#if !USE(QXMLQUERY)
#include <libxml/parser.h>
#include <libxslt/transform.h>
#endif

#include <wtf/PassRefPtr.h>

namespace WebCore {

class DocLoader;
class Document;
class XSLImportRule;
    
class XSLStyleSheet : public StyleSheet {
public:
#if !USE(QXMLQUERY)
#if 1
    // modified at webkit.org trunk r53607 and r54645
    static PassRefPtr<XSLStyleSheet> create(XSLImportRule* parentImport, const String& originalURL, const KURL& finalURL)
#else
    static PassRefPtr<XSLStyleSheet> create(XSLImportRule* parentImport, const String& href)
#endif
    {
#if 1
        // modified at webkit.org trunk r53607 and r54645
        return adoptRef(new XSLStyleSheet(parentImport, originalURL, finalURL));
#else
        return adoptRef(new XSLStyleSheet(parentImport, href));
#endif
    }
#endif
#if 1
    // modified at webkit.org trunk r53607 and r54645
    static PassRefPtr<XSLStyleSheet> create(Node* parentNode, const String& originalURL, const KURL& finalURL)
#else
    static PassRefPtr<XSLStyleSheet> create(Node* parentNode, const String& href)
#endif
    {
#if 1
        // modified at webkit.org trunk r53607 and r54645
        return adoptRef(new XSLStyleSheet(parentNode, originalURL, finalURL, false));
#else
        return adoptRef(new XSLStyleSheet(parentNode, href, false));
#endif
    }
#if 1
    // modified at webkit.org trunk r53607 and r54645
    static PassRefPtr<XSLStyleSheet> createInline(Node* parentNode, const KURL& finalURL)
#else
    static PassRefPtr<XSLStyleSheet> createEmbedded(Node* parentNode, const String& href)
#endif
    {
#if 1
        // modified at webkit.org trunk r53607 and r54645
        return adoptRef(new XSLStyleSheet(parentNode, finalURL.string(), finalURL, true));
#else
        return adoptRef(new XSLStyleSheet(parentNode, href, true));
#endif
    }

    virtual ~XSLStyleSheet();
    
    virtual bool isXSLStyleSheet() const { return true; }

    virtual String type() const { return "text/xml"; }

    virtual bool parseString(const String &string, bool strict = true);
    
    virtual bool isLoading();
    virtual void checkLoaded();

    void loadChildSheets();
    void loadChildSheet(const String& href);

    DocLoader* docLoader();

    Document* ownerDocument() { return m_ownerDocument; }
    void setParentStyleSheet(XSLStyleSheet* parent);

#if USE(QXMLQUERY)
    String sheetString() const { return m_sheetString; }
#else
    xmlDocPtr document();
    xsltStylesheetPtr compileStyleSheet();
    xmlDocPtr locateStylesheetSubResource(xmlDocPtr parentDoc, const xmlChar* uri);
#endif

    void clearDocuments();

    void markAsProcessed();
    bool processed() const { return m_processed; }

private:
#if 1
    // modified at webkit.org trunk r53607 and r54645
    XSLStyleSheet(Node* parentNode, const String& originalURL, const KURL& finalURL, bool embedded);
#else
    XSLStyleSheet(Node* parentNode, const String& href, bool embedded);
#endif
#if !USE(QXMLQUERY)
#if 1
    // modified at webkit.org trunk r53607 and r54645
    XSLStyleSheet(XSLImportRule* parentImport, const String& originalURL, const KURL& finalURL);
#else
    XSLStyleSheet(XSLImportRule* parentImport, const String& href);
#endif
#endif

    Document* m_ownerDocument;
    bool m_embedded;
    bool m_processed;

#if USE(QXMLQUERY)
    String m_sheetString;
#else
    xmlDocPtr m_stylesheetDoc;
    bool m_stylesheetDocTaken;
#endif
    
    XSLStyleSheet* m_parentStyleSheet;
};

} // namespace WebCore

#endif // ENABLE(XSLT)

#endif // XSLStyleSheet_h
