/**
 * rutabaga: an OpenGL widget toolkit
 * Copyright (c) 2013 William Light.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "rutabaga/rutabaga.h"
#include "rutabaga/object.h"
#include "rutabaga/window.h"
#include "rutabaga/style.h"

#include "private/default_style.h"

static rtb_style_t no_style = {
	.for_type = "couldn't find a style for this object",
	.available_styles = RTB_STYLE_NORMAL,

	.states = {
		[RTB_DRAW_NORMAL] = {
			.fg = {RGB(0x00FF00), 1.f},
			.bg = {RGB(0xFF0000), 1.f}}
	}
};


static int style_resolve(rtb_win_t *window, rtb_style_t *style)
{
	style->resolved_type = rtb_type_lookup(window, style->for_type);
	if (style->resolved_type)
		return 0;
	return -1;
}

static rtb_style_t *style_for_type(rtb_type_atom_t *atom,
		rtb_style_t *style_list)
{
	for (; style_list->for_type; style_list++) {
		if (style_list->resolved_type &&
				rtb_is_type(style_list->resolved_type, atom))
			return style_list;
	}

	return &no_style;
}

/**
 * public API
 */

int rtb_style_resolve_list(rtb_win_t *win, rtb_style_t *style_list)
{
	int unresolved_styles = 0;

	for (; style_list->for_type; style_list++) {
		if (style_resolve(win, style_list))
			unresolved_styles++;
	}

	return unresolved_styles;
}

void rtb_style_apply_to_tree(rtb_obj_t *root, rtb_style_t *style_list)
{
	rtb_obj_t *iter;

	if (!root->style)
		root->style = style_for_type(RTB_TYPE_ATOM(root), style_list);

	TAILQ_FOREACH(iter, &root->children, child)
		rtb_style_apply_to_tree(iter, style_list);
}

rtb_style_t *rtb_style_for_object(rtb_obj_t *obj, rtb_style_t *style_list)
{
	return style_for_type(RTB_TYPE_ATOM(obj), style_list);
}

rtb_style_t *rtb_style_get_defaults(void)
{
	return default_style;
}
