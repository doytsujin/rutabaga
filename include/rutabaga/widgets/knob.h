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

#pragma once

#include "rutabaga/rutabaga.h"
#include "rutabaga/event.h"
#include "rutabaga/mat4.h"

typedef struct rtb_knob rtb_knob_t;
typedef struct rtb_event_knob rtb_ev_knob_t;

#define RTB_KNOB(x) (&(x)->_rtb_knob)

typedef enum {
	KNOB_VALUE_CHANGE = 1
} rtb_ev_knob_type_t;

struct rtb_event_knob {
	RTB_INHERIT(rtb_event);

	float value;
};

struct rtb_knob {
	RTB_INHERIT(rtb_object);

	/* public *********************************/
	float origin;
	float min;
	float max;

	/* private ********************************/
	mat4 modelview;
	float value;
	GLuint vbo[2];
};

void rtb_knob_set_value(rtb_knob_t *self, float new_value);

rtb_knob_t *rtb_knob_new(void);
void rtb_knob_free(rtb_knob_t *self);
