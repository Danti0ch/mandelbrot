#ifndef GRAPHICS_H
#define GRAPHICS_H

typedef unsigned int uint;

const uint WINDOW_HEIGHT  = 300;

// must be 4 * x
const uint WINDOW_LENGTH  = 400;
const uint HWINDOW_HEIGHT = WINDOW_HEIGHT >> 1;
const uint HWINDOW_LENGTH = WINDOW_LENGTH >> 1;

const char FONT_NAME[]  = "arial.ttf";
const uint BUF_SIZE     = 1 << 8;
const uint FONT_SIZE    = 30;
const char TITLE_STR[]  = "BU11ERBRO11";

float X_0         = 0;
float Y_0         = 0;

const float INIT_SCALE_VALUE = 1;
const float INIT_DX_VALUE    = 1 / float(WINDOW_LENGTH);
const float INIT_DY_VALUE    = 1 / float(WINDOW_HEIGHT);

const float coord_move_ratio = 4;
const float SCALE_RATIO      = 1.32;

const uint MAX_COLOR_VALUE = 256;
const uint COORD_LIMIT     = 100;

const uint REG_SIZE = 8;
void RunGraphics();

#endif // GRAPHICS_H
