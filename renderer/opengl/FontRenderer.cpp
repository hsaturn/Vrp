/* 
 * File:   FontRenderer.cpp
 * Author: francois
 * 
 * Created on 7 juillet 2015, 17:42
 */

#include "Color.h"
#include <GL/freeglut_std.h>

#include "FontRenderer.h"
#include "FontRendererGlfw.h"

FontRenderer* FontRenderer::factory(string& data)
{
	return FontRendererGlfw::factory(data);
}

