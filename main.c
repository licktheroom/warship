/**
    Warship, a simple, textureless game.
    Copyright (C) 2022  licktheroom

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/gpl-3.0.txt>.
 *
 *
 * Version 0.1.6
 *
 * Version scheme:
 *      0.1 = Most basic systems are in place and functional.
 *      0.2 = Everything to do with ships and bullets are finished.
 *      0.3 = Islands, home bases, and gamemodes are finished.
 *      0.4 = Island bases and special events are finished.
 *      0.5 = Dialog, settings, and menus are finished.
 *      0.6 = Both team's A.Is are finished.
 *      0.7 = Multi-threading and multi-platform support.
 *      0.8 = 1/3 of main missions finished.
 *      0.9 = 2/3 of main missions finished.
 *      1.0 = All missions are finished.
 *
 *      0.0.n
 *      n = The number of weeks spent on this version.
 *      0.4.17 = 17 weeks have been spent on version 0.4
 *
 * Dates are in day/month/year
 *
 * TODO 25/9/2022: Realize I'm a bad programmer.
 *
 * TODO 25/9/2022: Rewrite comments in main.c, gotta make them look nice like in settings.c
**/

/* Include non-standerd headers */
#include "include/glad/gl.h"
#include <GLFW/glfw3.h>

#include "include/ship_offsets.h"
#include "include/level_data.h"

#include "include/shaders/vertex.h"
#include "include/shaders/fragment.h"

/* Standerd headers */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

/* Static values */

static FILE * log_file;

static uint VAO, VBO, EBO, s_program;
static uint vertex_uniform_size;
static uint g_phy_tick;

static int g_state;
static vec2 g_scn;

static double g_pixel_scale;

static vec2 g_mouse_pos;
static int g_ship_controlled;

static Ship g_blue_ships[25] = {0};
static Ship g_red_ships[25] = {0};

static Proj g_proj[1000] = {0};

static cint g_num_blue_ships, g_num_red_ships;
static uint g_num_proj;

/* Functions */

// clean up

void clean_up(const char * error);

// loading

GLFWwindow * create_window(int size_x, int size_y, char * name);
void generate_buffers();
void create_shader_program();

void load_level(int level_n);

// physics

void game_physics();

// TODO 25/9/2022: Move these to physics.c
void move_proj(Proj * pro);
bool proj_collision(Proj * pro);

// rendering

void update_buffer();

// callbacks

void resize_callback(GLFWwindow* window, int width, int height);

// input callbacks

void mouse_click_callback(GLFWwindow * window, int button, int action, int mods);
void mouse_wheel_callback(GLFWwindow * window, double offsetx, double offsety);
void keyboard_callback(GLFWwindow * window, int key, int scancode, int action, int mods);

/* MAIN */
int main()
{
    // set basic values
    // base game resolution
    g_scn.x = 1200;
    g_scn.y = 600;

    g_phy_tick = 0;

    g_num_proj = 0;

    g_pixel_scale = 1.0; // pixel scale, used to be known as pixel size, dictates how big each pixel should be
    g_state = 1;

    g_ship_controlled = -1; // -1 is a base value

    load_level(0);

    if(settings_generate())
    {
        char * log = settings_get("output");

        if(log)
        {
            if(strcmp(log, "console") == 0)
                log_file = stdout;
            else if(strcmp(log, "file"))
            {
                log_file = fopen("logs.txt", "w");

                if(!log_file)
                    clean_up("Could not open logs.txt");
            } else
                clean_up("Setting \"output\" was invalid.");
        } else
            clean_up("Setting \"output\" was not found.");
    } else
        clean_up("Setting cache was not able to be generated.");

    if(log_file == NULL)
        clean_up("Var \"log_file\" is NULL");

    fprintf(log_file, "\nWarship  Copyright (C) 2022  licktheroom\nThis program comes with ABSOLUTELY NO WARRANTY; for details please see LICENSE.\nThis is free software, and you are welcome to redistribute it\nunder certain conditions; please see LICENSE.\n\n");

    // continue normally
    unsigned int fps_tracker = 0;
    double fps_track;

    double app_start, start, finish;

    // create window
    fprintf(log_file, "Starting Warship v0.1.6 by licktheroom.\n\n");
    fprintf(log_file, "Creating window... ");
    fflush(log_file);

    app_start = start = glfwGetTime();

    GLFWwindow * window = create_window(g_scn.x, g_scn.y, "Warship v0.1.6");

    finish = glfwGetTime();

    fprintf(log_file, "%fms\n", (finish - start) * 1000);

    // load buffers
    fprintf(log_file, "Generating vertex buffers... ");
    fflush(log_file);

    start = glfwGetTime();

    generate_buffers();

    finish = glfwGetTime();

    fprintf(log_file, "%fms\n", (finish - start) * 1000);

    // load shaders
    fprintf(log_file, "Compiling shaders... ");
    fflush(log_file);

    start = glfwGetTime();

    create_shader_program();

    finish = glfwGetTime();

    fprintf(log_file, "%fms\n", (finish - start) * 1000);

    // game loop

    fps_track = glfwGetTime();

    fprintf(log_file, "Loading successful.\n\nUse the number keys (0-9) to switch between test levels.\n\n");

    double phy_beginning, phy_current;

    // assume you are using the dvorak layout and press Q
    // glfw says you pressed X because thats where X is in qwerty
    // this code prints Q when using dvorak and X in qwerty
    //
    // fprintf(log_file, "%s\n", glfwGetKeyName(GLFW_KEY_X, glfwGetKeyScancode(GLFW_KEY_X)));

    phy_beginning = glfwGetTime();

    while(!glfwWindowShouldClose(window))
    {
        // update mouse position
        double tempx, tempy;

        glfwGetCursorPos(window, &tempx, &tempy);

        g_mouse_pos.x = tempx / (5.0 * g_pixel_scale);
        g_mouse_pos.y = tempy / (5.0 * g_pixel_scale);

        // physics
        phy_current = glfwGetTime();

        if(phy_current - phy_beginning >= 0.25)
        {
            phy_beginning = phy_current;
            game_physics();
        }

        // rendering
        glClearColor(0.0f, 0.0f, 0.098f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        update_buffer(0);

        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, vertex_uniform_size);

        // others
        glfwSwapBuffers(window);
        glfwPollEvents();

        fps_tracker++;

        finish = glfwGetTime();
        if(finish-fps_track >= 1)
        {
            fps_track = finish;

            fprintf(log_file, "\rFPS: %d", fps_tracker);
            fflush(log_file);

            fps_tracker = 0;
        }
    }

    // die

    finish = glfwGetTime();

    fprintf(log_file, "\n\nApp was alive for: %f seconds\n", finish-app_start);

    clean_up(NULL);
}

/* Functions */

// Clear all data and close the app
void clean_up(const char * error)
{
    settings_clear();

    fprintf(stdout, "HI\n");

    // OpenGL buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(s_program);

    // I don't bother explicitly destroying the window because glfwTerminate automatically destroys all windows
    glfwTerminate();

    // report exit status
    if(!error)
        fprintf(log_file, "Exit successful.\n");
    else
        fprintf(log_file, "There was an error:\n%s\n", error);

    fclose(log_file);

    exit(0);
}

// Creates a window
GLFWwindow * create_window(int size_x, int size_y, char * name)
{
    // glfw hints
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // supposed to turn off vsync
    // it's up to the drivers if they respect this setting
    // everyone turns this off anyway, right?
    // i'll turn this into a setting later
    glfwSwapInterval(0);

    // create window
    GLFWwindow* window = glfwCreateWindow(size_x, size_y, name, NULL, NULL);
    if(!window)
        clean_up("Could not create window.");

    glfwMakeContextCurrent(window);

    // start gl
    if(!gladLoadGL(glfwGetProcAddress))
        clean_up("Could not start OpenGL.");

    glViewport(0, 0, size_x, size_y);

    // set callbacks
    glfwSetFramebufferSizeCallback(window, resize_callback);
    glfwSetMouseButtonCallback(window, mouse_click_callback);
    glfwSetScrollCallback(window, mouse_wheel_callback);
    glfwSetKeyCallback(window, keyboard_callback);

    // hides cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    return window;
}

// does the dumb buffer stuff
// no idea how to add comments to this
void generate_buffers()
{
    float vertices[] = {
        0, 0,
        0, 10,
        10, 10,
        10, 0
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

// Compiles shaders and creates the shader program
void create_shader_program()
{
    int success;
    unsigned int v_shader, f_shader;

    // Vertex Shader
    v_shader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(v_shader, 1, &VERTEX_shader, NULL);
    glCompileShader(v_shader);

    glGetShaderiv(v_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char log[512];
        glGetShaderInfoLog(v_shader, 512, NULL, log);

        fprintf(log_file, "%s\n", log);

        clean_up("Could not compile vertex shader.");
    }

    // Fragment Shader
    f_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(f_shader, 1, &FRAGMENT_shader, NULL);
    glCompileShader(f_shader);

    glGetShaderiv(f_shader, GL_COMPILE_STATUS,  &success);
    if(!success)
    {
        char log[512];
        glGetShaderInfoLog(v_shader, 512, NULL, log);

        fprintf(log_file, "%s\n", log);

        clean_up("Could not compile fragment shader.");
    }

    // Program
    s_program = glCreateProgram();

    glAttachShader(s_program, v_shader);
    glAttachShader(s_program, f_shader);
    glLinkProgram(s_program);

    glGetProgramiv(s_program, GL_LINK_STATUS, &success);
    if(!success)
        clean_up("Could not link program.");

    // Finishing details
    glUseProgram(s_program);

    glDeleteShader(v_shader);
    glDeleteShader(f_shader);

    glUniform2i(glGetUniformLocation(s_program, "scn"), g_scn.x, g_scn.y);
    glUniform1f(glGetUniformLocation(s_program, "scale"), g_pixel_scale);
}

void load_level(int level_n)
{
    g_num_blue_ships = lev_data.blue_ship_n;
    g_num_red_ships = lev_data.red_ship_n;

    if(g_num_blue_ships > 0)
        for(uint i = 0; i < g_num_blue_ships; i++)
        {
            g_blue_ships[i].position = lev_data.blue_ships[i].pos;
            g_blue_ships[i].type = lev_data.blue_ships[i].type;

            for(uint d = 0; d < 4; d++)
                    g_blue_ships[i].current_directions[d] = lev_data.blue_ships[i].directions[d];

            int dir = ship_current_direction(g_blue_ships[i].current_directions, g_blue_ships[i].last_checked_directions);
            if(dir != -1 && dir != g_blue_ships[i].pointing)
                g_blue_ships[i].pointing = dir;

            ship_update_childern(&g_blue_ships[i]);

            if(g_blue_ships[i].type == SHIP_TYPE_SCOUT)
                g_blue_ships[i].hp = 3;
            else if(g_blue_ships[i].type == SHIP_TYPE_WARSHIP)
                g_blue_ships[i].hp = 6;
            else if(g_blue_ships[i].type == SHIP_TYPE_CARRIER)
                g_blue_ships[i].hp = 5;
            else
                g_blue_ships[i].hp = 0;
        }

    if(g_num_red_ships > 0)
        for(uint i = 0; i < g_num_red_ships; i++)
        {
            g_red_ships[i].position = lev_data.red_ships[i].pos;
            g_red_ships[i].type = lev_data.red_ships[i].type;

            for(uint d = 0; d < 4; d++)
                    g_red_ships[i].current_directions[d] = lev_data.red_ships[i].directions[d];

            int dir = ship_current_direction(g_red_ships[i].current_directions, g_red_ships[i].last_checked_directions);
            if(dir != -1 && dir != g_red_ships[i].pointing)
                g_red_ships[i].pointing = dir;

            ship_update_childern(&g_red_ships[i]);

            if(g_red_ships[i].type == SHIP_TYPE_SCOUT)
                g_red_ships[i].hp = 3;
            else if(g_red_ships[i].type == SHIP_TYPE_WARSHIP)
                g_red_ships[i].hp = 6;
            else if(g_red_ships[i].type == SHIP_TYPE_CARRIER)
                g_red_ships[i].hp = 5;
            else
                g_red_ships[i].hp = 0;
        }
}

// All game physics put 'neatly' into a function
void game_physics()
{

    // ships
    for(uint i = 0; i < g_num_blue_ships; i++)
    {
        int dir = ship_current_direction(g_blue_ships[i].current_directions, g_blue_ships[i].last_checked_directions);
        if(dir != -1 && dir != g_blue_ships[i].pointing)
        {
            g_blue_ships[i].pointing = dir;
            ship_update_childern(&g_blue_ships[i]);
        }

        if(g_blue_ships[i].type == SHIP_TYPE_SCOUT || g_phy_tick%2 == 0)
            ship_physics(&g_blue_ships[i]);
    }

    // projectiles
    uint deleted = 0;

    for(uint i = 0; i < g_num_proj; i++)
    {
        move_proj(&g_proj[i]);

        bool collided = proj_collision(&g_proj[i]);

        if(collided)
            deleted++;
        else
            g_proj[i-deleted] = g_proj[i];
    }

    g_num_proj -= deleted;

    g_phy_tick++;
}

// moves a projectile
void move_proj(Proj * pro)
{
    if(pro->dir == 0 || pro->dir == 1 || pro->dir == 7)
        pro->position.x++;
    else if(pro->dir == 3 || pro->dir == 4 || pro->dir == 5)
        pro->position.x--;

    if(pro->dir == 1 || pro->dir == 2 || pro->dir == 3)
        pro->position.y++;
    else if(pro->dir == 5 || pro->dir == 6 || pro->dir == 7)
        pro->position.y--;
}

// handles projectile collisions
// this includes dealing damages whe hitting a ship
bool proj_collision(Proj * pro)
{
    // board bounds
    if(pro->position.x < 0)
        return true;
    else if(pro->position.x > 100)
        return true;
    else if(pro->position.y < 0)
        return true;
    else if(pro->position.y > 100)
        return true;

    return false;
}

// checks for a mouse collision with blue ships
void check_mouse_col()
{
    bool changed = false;
    int prev = g_ship_controlled;

    g_ship_controlled = -1;

    // loop through blue ships
    for(uint i = 0; i < g_num_blue_ships; i++)
        if(v2_distance(g_mouse_pos, g_blue_ships[i].position) <= 20)
        {
            // check for root collision
            if(g_mouse_pos.x == g_blue_ships[i].position.x && g_mouse_pos.y == g_blue_ships[i].position.y)
            {
                g_ship_controlled = i;
                changed = true;
                break;
            }

            // check for children collision
            for(uint d = 0; d < g_blue_ships[i].children_size; d++)
                if(g_mouse_pos.x == g_blue_ships[i].children_positions[d].x && g_mouse_pos.y == g_blue_ships[i].children_positions[d].y)
                {
                    g_ship_controlled = i;
                    changed = true;
                    break;
                }

            // if there was a child collision then break out of the main loop
            if(changed)
                break;
        }

    // adjust ship settings
    if(prev == -1)
    {
        if(g_ship_controlled != -1)
        {
            // turn the newly controlled ship into the player
            for(uint i = 0; i < 4; i++)
                g_blue_ships[g_ship_controlled].current_directions[i] = false;

            g_blue_ships[g_ship_controlled].controlled = true;
        }
    } else if(g_ship_controlled != prev)
    {
        // turn the previously controlled ship back into an AI
        for(uint i = 0; i < 4; i++)
            g_blue_ships[prev].current_directions[i] = false;

        g_blue_ships[prev].controlled = false;

        // turn the newly controlled ship into the player
        if(g_ship_controlled != -1)
        {
            for(uint i = 0; i < 4; i++)
                g_blue_ships[g_ship_controlled].current_directions[i] = false;

            g_blue_ships[g_ship_controlled].controlled = true;
        }
    }
}

// updates uniform data in the vertex shader
void update_buffer()
{
    // long but detailed names
    uint total_blue_ship_children, total_red_ship_children;
    total_blue_ship_children = total_red_ship_children = 0;

    // name
    char name[30];

    // blues
    for(uint i = 0; i < g_num_blue_ships; i++)
    {
        // the stupid why to tell gl where in a tabel to put data
        // we must add total_blue_ship_children to i, otherwise we will overwrite the previous ship's children
        sprintf(name, "pos[%d]", i + total_blue_ship_children);

        glUniform3i(glGetUniformLocation(s_program, name), g_blue_ships[i].position.x * 10, g_blue_ships[i].position.y * 10, 1);

        for(uint d = 0; d < g_blue_ships[i].children_size; d++)
        {
            // we add 1 to make sure we don't overwrite the root position
            sprintf(name, "pos[%d]", d + i + total_blue_ship_children + 1);

            glUniform3i(glGetUniformLocation(s_program, name), g_blue_ships[i].children_positions[d].x * 10, g_blue_ships[i].children_positions[d].y * 10, 1);
        }

        total_blue_ship_children += g_blue_ships[i].children_size;
    }

    vertex_uniform_size = g_num_blue_ships + total_blue_ship_children;

    // reds
    for(uint i = 0; i < g_num_red_ships; i++)
    {
        // everything is the same as above but this time we need to add vertex_uniform_size
        sprintf(name, "pos[%d]", i + vertex_uniform_size + total_red_ship_children);

        glUniform3i(glGetUniformLocation(s_program, name), g_red_ships[i].position.x * 10, g_red_ships[i].position.y * 10, 0);

        for(uint d = 0; d < g_red_ships[i].children_size; d++)
        {
            // same as the blues
            sprintf(name, "pos[%d]", d + i + total_red_ship_children + vertex_uniform_size + 1);

            glUniform3i(glGetUniformLocation(s_program, name), g_red_ships[i].children_positions[d].x * 10, g_red_ships[i].children_positions[d].y * 10, 0);
        }

        total_red_ship_children += g_red_ships[i].children_size;
    }

    vertex_uniform_size += g_num_red_ships + total_red_ship_children;

    // bullets
    for(uint i = 0; i < g_num_proj; i++)
    {
        sprintf(name, "pos[%d]", vertex_uniform_size + i);

        uint color = 0;
        if(g_proj[i].team == 0)
            color = 3;
        else
            color = 4;

        glUniform3i(glGetUniformLocation(s_program, name), g_proj[i].position.x * 10, g_proj[i].position.y * 10, color);
    }

    vertex_uniform_size += g_num_proj;

    // mouse
    sprintf(name, "pos[%d]", vertex_uniform_size);

    glUniform3i(glGetUniformLocation(s_program, name), g_mouse_pos.x * 10, g_mouse_pos.y * 10, 2);

    vertex_uniform_size++;
}

// The Framebuffer Size Callback
void resize_callback(GLFWwindow* window, int width, int height)
{
    if(window)
    {
        g_scn.x = width;
        g_scn.y = height;

        glUniform2i(glGetUniformLocation(s_program, "scn"), g_scn.x, g_scn.y);

        glViewport(0, 0, g_scn.x, g_scn.y);
    }
}

// Mouse click callback
void mouse_click_callback(GLFWwindow * window, int button, int action, int mods)
{
    if(window)
    {
        // makes the compiler stop complaining about unused variables
        mods = 0;

        if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            check_mouse_col();
    }
}

// Handles mouse wheel input
void mouse_wheel_callback(GLFWwindow * window, double offsetx, double offsety)
{
    if(window)
    {
        // to stop the compiler from complaining about unused varibles
        offsetx = 0;

        // if offsety is 1 then the user is scolling up
        if(offsety == 1)
            g_pixel_scale += 0.1;
        else
            g_pixel_scale -= 0.1;

	// check bounds
        if(g_pixel_scale >= 20.05)
            g_pixel_scale = 20.0;
        else if(g_pixel_scale <= 0.6)
            g_pixel_scale = 0.6;

        glUniform1f(glGetUniformLocation(s_program, "scale"), g_pixel_scale);
    }
}

// Handles keyboard input
void keyboard_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if(window)
    {
        // stops the compiler's compaining about unused variables
        mods = 0;
        scancode = 0;

        switch(key)
        {
            case GLFW_KEY_ESCAPE:
                if(action == GLFW_PRESS)
                    glfwSetWindowShouldClose(window, true);

                break;

            case GLFW_KEY_X:
                if(g_ship_controlled >= 0)
                {
                    g_proj[g_num_proj] = proj_create(&g_blue_ships[g_ship_controlled], 0);
                    g_num_proj++;
                }

                break;

            // the following are arrow keys
            case GLFW_KEY_UP:
                if(g_ship_controlled >= 0)
                {
                    if(action == GLFW_PRESS)
                        g_blue_ships[g_ship_controlled].current_directions[0] = true;
                    else if(action == GLFW_RELEASE)
                        g_blue_ships[g_ship_controlled].current_directions[0] = false;
                }

                break;

            case GLFW_KEY_DOWN:
                if(g_ship_controlled >= 0)
                {
                    if(action == GLFW_PRESS)
                        g_blue_ships[g_ship_controlled].current_directions[1] = true;
                    else if(action == GLFW_RELEASE)
                        g_blue_ships[g_ship_controlled].current_directions[1] = false;
                }

                break;

            case GLFW_KEY_LEFT:
                if(g_ship_controlled >= 0)
                {
                    if(action == GLFW_PRESS)
                        g_blue_ships[g_ship_controlled].current_directions[2] = true;
                    else if(action == GLFW_RELEASE)
                        g_blue_ships[g_ship_controlled].current_directions[2] = false;
                }

                break;

            case GLFW_KEY_RIGHT:
                if(g_ship_controlled >= 0)
                {
                    if(action == GLFW_PRESS)
                        g_blue_ships[g_ship_controlled].current_directions[3] = true;
                    else if(action == GLFW_RELEASE)
                        g_blue_ships[g_ship_controlled].current_directions[3] = false;
                }

                break;
        }
    }
}
