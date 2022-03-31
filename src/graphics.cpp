#include "graphics.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <time.h>
#include <assert.h>

using namespace sf;

//----------------------LOCAL-FUNCTIONS-DECLARATION-----------------------//

void draw_fps(RenderWindow* window, uint framerate);
uint get_color_id(uint x, uint y);
//----------------------PUBLIC-FUNCTIONS-DEFINITIONS----------------------//

void RunGraphics(){
    RenderWindow window(VideoMode(WINDOW_LENGTH, WINDOW_HEIGHT), TITLE_STR);
    
    Vertex pixels[WINDOW_HEIGHT * WINDOW_LENGTH];

    for(int y = 0; y < WINDOW_HEIGHT; y++){
        for(int x = 0; x < WINDOW_LENGTH; x++){

            pixels[y * WINDOW_HEIGHT + x] = Vertex(Vector2f(x, y), Color::Red);
        }
    }

    float cur_scale = INIT_SCALE_RATIO;
    float dx        = INIT_DX_RATIO;
    float dy        = INIT_DY_RATIO;

    uint framerate_counter  = 0;
    uint cur_framerate      = 0;
    
    clock_t init_time = clock();

    while (window.isOpen())
    {
        Event cur_event;

        while (window.pollEvent(cur_event))
        {
            if (cur_event.type == Event::Closed)  window.close();
        }

        window.clear();

        for(uint y_i = 0; y_i < WINDOW_HEIGHT; y_i++){

            for(uint x_i = 0; x_i < WINDOW_LENGTH; x_i++){

                float x = ((x_i - HWINDOW_LENGTH) * dx + X_0) * cur_scale;
                float y = ((y_i - HWINDOW_HEIGHT) * dy + Y_0) * cur_scale;

                uint color_id = get_color_id(x_i, y_i);

                Color cur_color(0, 0, 0);

                if(color_id < MAX_COLOR_VALUE){
                    cur_color.b = 255;
                    cur_color.g = 255;
                    cur_color.r = 255;
                }
                
                pixels[y_i * WINDOW_HEIGHT + x_i].color = cur_color;
            }
        }

        window.draw(pixels, WINDOW_LENGTH * WINDOW_HEIGHT, Points);

        clock_t end_time = clock();
        framerate_counter++;

        if(end_time - init_time >= CLOCKS_PER_SEC){
            cur_framerate = framerate_counter;
            framerate_counter = 0;
            init_time = clock();
        }
        
        draw_fps(&window, cur_framerate);

        window.display();
    }

    return;
}
//--------------------------------------------//

//----------------------LOCAL-FUNCTIONS-DEFINITIONS----------------------//

void draw_fps(RenderWindow* window, uint framerate){
        
    char buffer[BUF_SIZE] = "";
    snprintf(buffer, sizeof(buffer), "%u\n", framerate);
    Font font;
    
    font.loadFromFile(FONT_NAME);

    Text text(buffer, font);
    text.setCharacterSize(FONT_SIZE);
    text.setStyle(Text::Bold);
    text.setFillColor(Color::Red);

    window->draw(text);

    return;
}
//--------------------------------------------//

uint get_color_id(uint x, uint y){

    assert(x < WINDOW_LENGTH);
    assert(y < WINDOW_HEIGHT);

    uint color_value = 0;

    for(; color_value < MAX_COLOR_VALUE; color_value++){

        uint xy = x * y;
        uint x2 = x * x;
        uint y2 = y * y;

        if(x2 + y2 > COORD_LIMIT){
            break;
        }

        x = x2 - y2 + X_0;
        y = xy + xy + Y_0;
    }

    return color_value;
}
//--------------------------------------------//
