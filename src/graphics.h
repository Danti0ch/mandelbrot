#ifndef GRAPHICS_H
#define GRAPHICS_H

typedef unsigned int uint;

const uint WINDOW_HEIGHT  = 400;
const uint WINDOW_LENGTH  = 300;
const uint HWINDOW_HEIGHT = WINDOW_HEIGHT >> 1;
const uint HWINDOW_LENGTH = WINDOW_LENGTH >> 1;


const char FONT_NAME[]  = "arial.ttf";
const uint BUF_SIZE     = 1 << 8;
const uint FONT_SIZE    = 30;
const char TITLE_STR[] = "BU11ERBRO11";

const float X_0         = 0;
const float Y_0         = 0;
const float INIT_SCALE_RATIO = 1;
const float INIT_DX_RATIO    = 1 / ((float)WINDOW_LENGTH);
const float INIT_DY_RATIO    = 1 / ((float)WINDOW_HEIGHT);

const uint MAX_COLOR_VALUE = 256;
const uint COORD_LIMIT  = 70;

void RunGraphics();

#endif // GRAPHICS_H
