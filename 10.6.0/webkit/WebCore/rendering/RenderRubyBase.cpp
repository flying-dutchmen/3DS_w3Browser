/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "RenderRubyBase.h"

namespace WebCore {

RenderRubyBase::RenderRubyBase(Node* node)
    : RenderBlock(node)
{
    setInline(false);
}

RenderRubyBase::~RenderRubyBase()
{
}

bool RenderRubyBase::isChildAllowed(RenderObject* child, RenderStyle*) const
{
    return child->isInline();
}

#if 1
// modified at webkit.org trunk r53525
void RenderRubyBase::moveChildren(RenderRubyBase* toBase, RenderObject* fromBeforeChild)
{
    // This function removes all children that are before (!) beforeChild
    // and appends them to toBase.
    ASSERT_ARG(toBase, toBase);
    
    // First make sure that beforeChild (if set) is indeed a direct child of this.
    // Inline children might be wrapped in an anonymous block if there's a continuation.
    // Theoretically, in ruby bases, this can happen with only the first such a child,
    // so it should be OK to just climb the tree.
    while (fromBeforeChild && fromBeforeChild->parent() != this)
        fromBeforeChild = fromBeforeChild->parent();

    if (childrenInline())
        moveInlineChildren(toBase, fromBeforeChild);
    else
        moveBlockChildren(toBase, fromBeforeChild);

    setNeedsLayoutAndPrefWidthsRecalc();
    toBase->setNeedsLayoutAndPrefWidthsRecalc();
}

void RenderRubyBase::moveInlineChildren(RenderRubyBase* toBase, RenderObject* fromBeforeChild)
{
    ASSERT(childrenInline());
    ASSERT_ARG(toBase, toBase);

    if (!firstChild())
        return;

    RenderBlock* toBlock;
    if (toBase->childrenInline()) {
        // The standard and easy case: move the children into the target base
        toBlock = toBase;
    } else {
        // We need to wrap the inline objects into an anonymous block.
        // If toBase has a suitable block, we re-use it, otherwise create a new one.
        RenderObject* lastChild = toBase->lastChild();
        if (lastChild && lastChild->isAnonymousBlock() && lastChild->childrenInline())
            toBlock = toRenderBlock(lastChild);
        else {
            toBlock = toBase->createAnonymousBlock();
            toBase->children()->appendChildNode(toBase, toBlock);
        }
    }
    // Move our inline children into the target block we determined above.
    for (RenderObject* child = firstChild(); child != fromBeforeChild; child = firstChild())
        moveChildTo(toBlock, toBlock->children(), child);
}

void RenderRubyBase::moveBlockChildren(RenderRubyBase* toBase, RenderObject* fromBeforeChild)
{
    ASSERT(!childrenInline());
    ASSERT_ARG(toBase, toBase);

    if (!firstChild())
        return;

    if (toBase->childrenInline())
        toBase->makeChildrenNonInline();

    // If an anonymous block would be put next to another such block, then merge those.
    RenderObject* firstChildHere = firstChild();
    RenderObject* lastChildThere = toBase->lastChild();
    if (firstChildHere->isAnonymousBlock() && firstChildHere->childrenInline() 
            && lastChildThere && lastChildThere->isAnonymousBlock() && lastChildThere->childrenInline()) {            
        RenderBlock* anonBlockHere = toRenderBlock(firstChildHere);
        RenderBlock* anonBlockThere = toRenderBlock(lastChildThere);
        anonBlockHere->moveAllChildrenTo(anonBlockThere, anonBlockThere->children());
        anonBlockHere->deleteLineBoxTree();
        anonBlockHere->destroy();
    }
    // Move all remaining children normally.
    for (RenderObject* child = firstChild(); child != fromBeforeChild; child = firstChild())
        moveChildTo(toBase, toBase->children(), child);
}
#else
void RenderRubyBase::splitToLeft(RenderBlock* leftBase, RenderObject* beforeChild)
{
    // This function removes all children that are before (!) beforeChild
    // and appends them to leftBase.
    ASSERT(leftBase);

    // First make sure that beforeChild (if set) is indeed a direct child of this.
    // Fallback: climb up the tree to make sure. This may result in somewhat incorrect rendering.
    // FIXME: Can this happen? Is there a better/more correct way to solve this?
    ASSERT(!beforeChild || beforeChild->parent() == this);
    while (beforeChild && beforeChild->parent() != this)
        beforeChild = beforeChild->parent();

    RenderObject* child = firstChild();
    while (child != beforeChild) {
        RenderObject* nextChild = child->nextSibling();
        moveChildTo(leftBase, leftBase->children(), child);
        child = nextChild;
    }
}

void RenderRubyBase::mergeWithRight(RenderBlock* rightBase)
{
    // This function removes all children and prepends (!) them to rightBase.
    ASSERT(rightBase);

    RenderObject* firstPos = rightBase->firstChild();
    RenderObject* child = lastChild();
    while (child) {
        moveChildTo(rightBase, rightBase->children(), firstPos, child);
        firstPos = child;
        child = lastChild();
    }
}
#endif

} // namespace WebCore
