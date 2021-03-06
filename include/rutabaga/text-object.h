/**
 * rutabaga: an OpenGL widget toolkit
 * Copyright (c) 2013-2018 William Light.
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

#pragma once

#include <rutabaga/types.h>
#include <rutabaga/window.h>
#include <rutabaga/geometry.h>
#include <rutabaga/font-manager.h>
#include <rutabaga/style.h>
#include <rutabaga/render.h>

#include "freetype-gl/vertex-buffer.h"

struct rtb_text_object {
	GLfloat w, h;

	vertex_buffer_t *vertices;
	struct rtb_font_manager *fm;
	const struct rtb_font *font;
};

int rtb_text_object_get_glyph_rect(struct rtb_text_object *, int idx,
		struct rtb_rect *rect);
int rtb_text_object_count_glyphs(struct rtb_text_object *);

int rtb_text_object_update(struct rtb_text_object *,
		struct rtb_font *rfont, struct rtb_window *,
		const rtb_utf8_t *text, float line_height_multiplier);
void rtb_text_object_render(struct rtb_text_object *,
		struct rtb_render_context *ctx, float x, float y,
		const struct rtb_rgb_color *color);

struct rtb_text_object *rtb_text_object_new(struct rtb_font_manager *fm);
void rtb_text_object_free(struct rtb_text_object *self);
