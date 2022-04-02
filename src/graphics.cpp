#include "graphics.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <time.h>
#include <assert.h>
#include <immintrin.h>

using namespace sf;

float X_0         = 0;
float Y_0         = 0;
__m256i color_value = _mm256_set1_epi8(0);

// TODO: прикрутить регистры xmm
// TODO: запилить цвет
// TODO: понять как работает преобразование координат


// Спросить у Ярика
//----------------------LOCAL-FUNCTIONS-DECLARATION-----------------------//

void draw_fps(RenderWindow* window, uint framerate);
void get_color_id(float* x, float* y, float incr2);
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

    float cur_scale = INIT_SCALE_VALUE;
    float dx        = INIT_DX_VALUE;
    float dy        = INIT_DY_VALUE;

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
                if(Keyboard::isKeyPressed(Keyboard::D)){
                    X_0 += dx * coord_move_ratio * cur_scale;
                }
                if(Keyboard::isKeyPressed(Keyboard::A)){
                    X_0 -= dx * coord_move_ratio * cur_scale;
                }
                if(Keyboard::isKeyPressed(Keyboard::W)){
                    Y_0 -= dy * coord_move_ratio * cur_scale;
                }
                if(Keyboard::isKeyPressed(Keyboard::S)){
                    Y_0 += dy * coord_move_ratio * cur_scale;
                }
                if(Keyboard::isKeyPressed(Keyboard::Q)){
                    cur_scale *= SCALE_RATIO;
                }
                if(Keyboard::isKeyPressed(Keyboard::E)){
                    cur_scale /= SCALE_RATIO;
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

void get_color_id(float* x, float* y, float incr2){

    __m256 _x = _mm256_add_ps(
                _mm256_set_ps(0, incr2, incr2 * 2, incr2 * 3, incr2 * 4, incr2 * 5, incr2 * 6, incr2 * 7),
                _mm256_broadcast_ss(x));

    __m256 _y = _mm256_broadcast_ss(y);
    color_value = _mm256_set1_epi8(0);

    __m256 X = _x;
    __m256 Y = _y;

    __m256 coord_limit = _mm256_set1_ps((float)COORD_LIMIT);

    uint cur_step = 0;
    int ed[8] = {1, 1, 1, 1, 1, 1, 1, 1};

    for(; cur_step < MAX_COLOR_VALUE; cur_step++){

        __m256 xy = _mm256_mul_ps(X, Y);
        __m256 x2 = _mm256_mul_ps(X, X);
        __m256 y2 = _mm256_mul_ps(Y, Y);

        __m256i cmp_res = _mm256_castps_si256(_mm256_cmp_ps(_mm256_add_ps(x2, y2), coord_limit, _CMP_LE_OS));

        int res = _mm256_movemask_epi8(cmp_res);

        if(res == 0) break;

        color_value = _mm256_add_epi64(_mm256_maskload_epi32(ed, cmp_res), color_value);

        X = _mm256_add_ps(_mm256_sub_ps(x2, y2), _x);
        Y = _mm256_add_ps(_mm256_add_ps(xy, xy), _y);
    }

    return;
}
//--------------------------------------------//

void calc_pixels(Vertex* pixels, float dx, float dy, float cur_scale){

    assert(pixels != NULL);

    __m256i color_id = _mm256_set1_epi8(0);
    __m256i nullls   = _mm256_set1_epi8(255);

    float x_incr    = REG_SIZE * dx * cur_scale;
    float x_incr2   = dx * cur_scale;               // rename

    for(uint y_i = 0; y_i < WINDOW_HEIGHT; y_i++){
        // зачем dx, как это работает?, остальное понятно

        float x = ((    0      - (float)HWINDOW_LENGTH) * dx) * cur_scale + ((float)X_0);
        float y = (((float)y_i - (float)HWINDOW_HEIGHT) * dy) * cur_scale + ((float)Y_0);

        for(uint x_i = 0; x_i < WINDOW_LENGTH; x_i+= REG_SIZE, x += x_incr){

            get_color_id(&x, &y, x_incr2);

            int cur_colors[8] = {};
            _mm256_maskstore_epi32(cur_colors, nullls, color_value);


            for(int i = 0; i < REG_SIZE; i++){
                Color cur_color = Color::Black;

                if(cur_colors[i] < MAX_COLOR_VALUE){
                    cur_color.b = 255 - cur_colors[i];
                    cur_color.g = cur_colors[i] % 2 * 64;
                    cur_color.r = cur_colors[i];
                }
                pixels[y_i * WINDOW_LENGTH + x_i + REG_SIZE - 1 -i].color = cur_color;
            }
        }
    }
    return;
}
//--------------------------------------------//
