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

#include <stdlib.h>
#include <string.h>

#include "rutabaga/rutabaga.h"
#include "rutabaga/font-manager.h"
#include "rutabaga/window.h"
#include "rutabaga/shader.h"

#include "shaders/text.glsl.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define ERR(...) fprintf(stderr, "rutabaga: " __VA_ARGS__)
#define FONT "./assets/fonts/open_sans/OpenSans-Regular.ttf"

static const uint8_t lcd_weights[] = {
	0x00,
	0x55,
	0x56,
	0x55,
	0x00
};

/* characters we cache by default in the texture */
/* XXX: this is going to break so hard when we do the windows port */
static const rtb_utf32_t *cache =
	L" abcdefghijklmnopqrstuvwxyz"
	L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	L",.!?;"
	L" [\\]^_@{|}~\"#$%&'()*+-/0123456789:<=>`";

int rtb_font_manager_load_font(rtb_font_manager_t *fm, rtb_font_t *font,
		const char *path, int size)
{
	font->txfont = texture_font_new(fm->atlas, path, size);
	if (!font->txfont) {
		ERR("couldn't load font \"%s\"\n", path);
		return -1;
	}

	font->path = strdup(path);
	font->size = size;
	font->fm   = fm;

	memcpy(font->txfont->lcd_weights, lcd_weights, sizeof(lcd_weights));

	texture_font_load_glyphs(font->txfont, cache);
	return 0;
}

void rtb_font_manager_free_font(rtb_font_t *font)
{
	free(font->path);
	texture_font_delete(font->txfont);
}

int rtb_font_manager_init(rtb_win_t *win)
{
	rtb_font_manager_t *fm;

	if (!(fm = calloc(1, sizeof(*fm)))) {
		ERR("couldn't allocate font manager.\n");
		goto err_calloc;
	}

	if (!rtb_shader_create(RTB_SHADER(&fm->shader),
				TEXT_VERT_SHADER, TEXT_FRAG_SHADER)) {
		ERR("couldn't compile text shader.\n");
		goto err_shader;
	}

#define CACHE_UNIFORM(UNIFORM) \
	fm->shader.UNIFORM = glGetUniformLocation(fm->shader.program, #UNIFORM)

	CACHE_UNIFORM(offset);
	CACHE_UNIFORM(texture);
	CACHE_UNIFORM(atlas_pixel);
	CACHE_UNIFORM(gamma);

#undef CACHE_UNIFORM

#ifdef FT_CONFIG_OPTION_SUBPIXEL_RENDERING
	fm->atlas = texture_atlas_new(512, 512, 3);
#else
	fm->atlas = texture_atlas_new(512, 512, 1);
#endif

	if (rtb_font_manager_load_font(fm, &fm->fonts.main, FONT, 12) < 0)
		goto err_main_font;

	if (rtb_font_manager_load_font(fm, &fm->fonts.big, FONT, 20) < 0)
		goto err_big_font;

	fm->fonts.main.lcd_gamma = 2.2f;
	fm->fonts.big.lcd_gamma  = 2.2f;

	fm->win = win;
	win->font_manager = fm;
	return 0;

err_big_font:
	rtb_font_manager_free_font(&fm->fonts.main);
err_main_font:
err_shader:
	free(fm);
err_calloc:
	return -1;
}

void rtb_font_manager_fini(rtb_win_t *win)
{
	rtb_font_manager_t *fm = win->font_manager;

	if (!fm)
		return;

	rtb_font_manager_free_font(&fm->fonts.main);
	rtb_font_manager_free_font(&fm->fonts.big);
	texture_atlas_delete(fm->atlas);

	rtb_shader_free(RTB_SHADER(&fm->shader));

	free(fm);
}