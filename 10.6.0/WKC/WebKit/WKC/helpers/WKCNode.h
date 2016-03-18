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

#ifndef _WKC_HELPERS_WKC_NODE_H_
#define _WKC_HELPERS_WKC_NODE_H_

namespace WKC {
class Document;
class Element;
class RenderObject;
class String;
#ifdef WKC_ADD_WKC_HELPERS
class NamedNodeMap;
#endif // WKC_ADD_WKC_HELPERS

class NodePrivate;

enum {
    HTMLNames_inputTag,
    HTMLNames_textareaTag,
    HTMLNames_selectTag,
    HTMLNames_frameTag,
    HTMLNames_iframeTag,
};

enum {
    eventNames_clickEvent,
    eventNames_mousedownEvent,
    eventNames_mousemoveEvent,
    eventNames_dragEvent,
    eventNames_dragstartEvent,
    eventNames_dragendEvent,
};

class Node {
public:
    Node(NodePrivate&);
    Node(Node*, bool needsRef = false);
    ~Node();

    bool compare(const Node*) const;

    bool hasTagName(int) const;
    bool isFocusable() const;
    bool isHTMLElement() const;
    bool isElementNode() const;
    bool isFrameOwnerElement() const;
    bool inDocument() const;
    String nodeName() const;

    bool hasEventListeners(int);

    Document* document() const;
    RenderObject* renderer() const;
    Element* parentElement() const;

    Node* parent() const;
    Node* parentNode() const;
    Node* firstChild() const;
    Node* traverseNextNode() const;
    Node* traverseNextSibling() const;
    Node* shadowAncestorNode() const;

#ifdef WKC_ADD_WKC_HELPERS
    NamedNodeMap* attributes() const;
#endif // WKC_ADD_WKC_HELPERS

    NodePrivate& priv() const { return m_private; }

protected:
    NodePrivate* m_ownedPrivate;
    NodePrivate& m_private;
    bool m_needsRef;
};
}

#endif // _WKC_HELPERS_WKC_NODE_H_
