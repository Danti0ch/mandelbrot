#include "graphics.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <time.h>
#include <assert.h>

using namespace sf;

// TODO: рефактор
// TODO: прикрутить регистры xmm
// TODO: запилить цвет
// TODO: понять как работает преобразование координат

//----------------------LOCAL-FUNCTIONS-DECLARATION-----------------------//

void draw_fps(RenderWindow* window, uint framerate);
uint get_color_id(float x, float y);
void calc_pixels(Vertex* pixels, float dx, float dy, float cur_scale);
//----------------------PUBLIC-FUNCTIONS-DEFINITIONS----------------------//

void RunGraphics(){
    RenderWindow window(VideoMode(WINDOW_LENGTH, WINDOW_HEIGHT), TITLE_STR);
    
    Vertex* pixels = (Vertex*)calloc(WINDOW_HEIGHT * WINDOW_LENGTH, sizeof(Vertex));

    for(int y = 0; y < WINDOW_HEIGHT; y++){
        for(int x = 0; x < WINDOW_LENGTH; x++){

            pixels[y * WINDOW_LENGTH + x] = Vertex(Vector2f(x, y), Color::Red);
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
            if(cur_event.type == Event::Closed)  window.close();
            if(cur_event.type == Event::KeyPressed){

                    switch(cur_event.key.code){
                        case Keyboard::D:{
                            X_0 += dx * coord_move_ratio * cur_scale;
                            break;
                        }
                        case Keyboard::A:{
                            X_0 -= dx * coord_move_ratio * cur_scale;
                            break;
                        }
                        case Keyboard::W:{
                            Y_0 += dy * coord_move_ratio * cur_scale;
                            break;
                        }
                        case Keyboard::D:{
                            Y_0 += dy * coord_move_ratio * cur_scale;
                            break;
                        }
                        case Keyboard::D:{
                            X_0 += dx * coord_move_ratio * cur_scale;
                            break;
                        }
                        case Keyboard::D:{
                            X_0 += dx * coord_move_ratio * cur_scale;
                            break;
                        }
                        
                    }
                    if (cur_event.key.code == Keyboard::D)
                    {
                        X_0 += dx * coord_move_ratio * cur_scale;
                    }
                    if (cur_event.key.code == Keyboard::A)
                    {
                        X_0 -= dx * coord_move_ratio * cur_scale;
                    }
                    if (cur_event.key.code == Keyboard::W)
                    {
                        Y_0 -= dy * coord_move_ratio * cur_scale;
                    }
                    if (cur_event.key.code == Keyboard::S)
                    {
                        Y_0 += dy * coord_move_ratio * cur_scale;
                    }
                    if (cur_event.key.code == Keyboard::Q)
                    {
                        cur_scale *= 1.2;
                    }
                    if (cur_event.key.code == Keyboard::E)
                    {
                        cur_scale /= 1.2;
                    }
            }
        }

        window.clear();
        
        calc_pixels(pixels, dx, dy, cur_scale);

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

uint get_color_id(float x, float y){

    uint color_value = 0;

    float X = x;
    float Y = y;

    for(; color_value < MAX_COLOR_VALUE; color_value++){

        float xy = X * Y;
        float x2 = X * X;
        float y2 = Y * Y;

        if(x2 + y2 >= (float)COORD_LIMIT){
            break;
        }

        X = x2 - y2 + x;
        Y = xy + xy + y;
    }

    return color_value;
}
//--------------------------------------------//

void calc_pixels(Vertex* pixels, float dx, float dy, float cur_scale){

    assert(pixels != NULL);

    for(uint y_i = 0; y_i < WINDOW_HEIGHT; y_i++){
        // зачем dx, как это работает?, остальное понятно
        float x = ((    0      - (float)HWINDOW_LENGTH) * dx) * cur_scale + ((float)X_0);
        float y = (((float)y_i - (float)HWINDOW_HEIGHT) * dy) * cur_scale + ((float)Y_0);

        for(uint x_i = 0; x_i < WINDOW_LENGTH; x_i++, x += dx * cur_scale){

            uint color_id = get_color_id(x, y);

            Color cur_color = Color::Black;

            if(color_id < MAX_COLOR_VALUE){
                cur_color = Color::White;
                // printf("- %f, %f\n", (float)x_i - (float)HWINDOW_LENGTH, (float)y_i - (float)HWINDOW_HEIGHT);
            }
            else{
                //printf("+ %f, %f\n", (float)x_i - (float)HWINDOW_LENGTH, (float)y_i - (float)HWINDOW_HEIGHT);
            }
            pixels[y_i * WINDOW_LENGTH + x_i].color = cur_color;
        }
    }
        return;
}
//--------------------------------------------//
