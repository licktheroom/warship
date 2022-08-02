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
 * Version 0.0.1
 *
 * This will remain version 0.0.1 until it has somewhat caught upto the original SDL version.
 *
 * Huge thanks to learnopengl.com!
 *
 * I keep forgetting to add comments to my code.
 *
 * Use options like this:
 *      make CFLAGS=-D<option>
 *
 * To use more than one option:
 *      make CFLAGS=-D<option> CFLAGS+=<option2> CFLAGS+=<option3>
 *
 * Options:
        NO_TERMINAL - forces logging to file
 *
**/

/* Include non-standerd headers */
#include "include/glad/gl.h"
#include <GLFW/glfw3.h>

#include "include/vector.h"
#include "include/structs.h"
#include "include/enums.h"
#include "include/ship_offsets.h"
#include "include/level_data.h"
#include "include/logging.h"
#include "include/physics.h"

#include "include/shaders/vertex.h"
#include "include/shaders/fragment.h"

/* Standerd headers */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/* Static values */

static FILE * log_file;

static unsigned int VAO, VBO, EBO, s_program;
static unsigned int vertex_uniform_size;

static int g_state;
static vec2 g_scn;

static double g_pixel_scale;

static vec2 g_mouse_pos;
static int g_ship_controlled;

static Ship g_blue_ships[25] = {0};
static Ship g_red_ships[25] = {0};

static int g_num_blue_ships, g_num_red_ships;

/* Functions */

// clean up

void clean_up(int n);

// loading

GLFWwindow * create_window(int size_x, int size_y, char * name);
void generate_buffers();
void create_shader_program();

void load_level(int level_n);

// physics

void game_physics();

void check_mouse_col();

// rendering

void update_buffer(int which);

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

    g_pixel_scale = 1.0; // pixel scale, used to be known as pixel size, dictates how big each pixel should be
    g_state = 1;

    g_ship_controlled = -1; // -1 is a base value

    load_level(0);

#ifndef NO_TERMINAL
    log_file = LOGS_get_log_file(0);

    if(log_file == NULL)
        clean_up(6);
#else
    log_file = LOGS_get_log_file(2);

    if(log_file == NULL)
        clean_up(6);
#endif

    fprintf(log_file, "\nWarship  Copyright (C) 2022  licktheroom\nThis program comes with ABSOLUTELY NO WARRANTY; for details please see LICENSE.\nThis is free software, and you are welcome to redistribute it\nunder certain conditions; please see LICENSE.\n\n");

    // continue normally
    unsigned int fps_tracker = 0;
    double fps_track;

    double app_start, start, finish;

    // create window
    fprintf(log_file, "Starting Warship v0.0.1 by licktheroom.\n\n");
    fprintf(log_file, "Creating window... ");
    fflush(log_file);

    app_start = start = glfwGetTime();

    GLFWwindow * window = create_window(g_scn.x, g_scn.y, "Warship v0.0.1");

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

        // render blue team
        update_buffer(0);

        glUniform1i(glGetUniformLocation(s_program, "color"), 1);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, vertex_uniform_size);

        // render red team
        update_buffer(1);

        glUniform1i(glGetUniformLocation(s_program, "color"), 0);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, vertex_uniform_size);

        // render mouse
        update_buffer(2);

        glUniform1i(glGetUniformLocation(s_program, "color"), 2);
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

    clean_up(0);
}

/* Functions */

// Clear all data and close the app
void clean_up(int n)
{
    // OpenGL buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(s_program);

    // I don't bother explicitly destroying the window because glfwTerminate automatically destroys all windows
    glfwTerminate();

    // report exit status
    if(n > 0)
        fprintf(log_file, "There was an error, exiting with code: %d\nPlease refer to \"error_codes.txt\".\n", n);
    else
        fprintf(log_file, "Exit successful.\n");

    fclose(log_file);

    exit(n);
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
    glfwSwapInterval(0);

    // create window
    GLFWwindow* window = glfwCreateWindow(size_x, size_y, name, NULL, NULL);
    if(!window)
        clean_up(1);

    glfwMakeContextCurrent(window);

    // start gl
    if(!gladLoadGL(glfwGetProcAddress))
        clean_up(2);

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
        clean_up(3);

    // Fragment Shader
    f_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(f_shader, 1, &FRAGMENT_shader, NULL);
    glCompileShader(f_shader);

    glGetShaderiv(f_shader, GL_COMPILE_STATUS,  &success);
    if(!success)
        clean_up(4);

    // Program
    s_program = glCreateProgram();

    glAttachShader(s_program, v_shader);
    glAttachShader(s_program, f_shader);
    glLinkProgram(s_program);

    glGetProgramiv(s_program, GL_LINK_STATUS, &success);
    if(!success)
        clean_up(5);

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
        for(int i = 0; i < g_num_blue_ships; i++)
        {
            g_blue_ships[i].position = lev_data.blue_ships[i].pos;
            g_blue_ships[i].type = lev_data.blue_ships[i].type;

            for(int d = 0; d < 4; d++)
                    g_blue_ships[i].current_directions[d] = lev_data.blue_ships[i].directions[d];

            phy_update_ship_children(&g_blue_ships[i]);
        }

    if(g_num_red_ships > 0)
        for(int i = 0; i < g_num_red_ships; i++)
        {
            g_red_ships[i].position = lev_data.red_ships[i].pos;
            g_red_ships[i].type = lev_data.red_ships[i].type;

            for(int d = 0; d < 4; d++)
                    g_red_ships[i].current_directions[d] = lev_data.red_ships[i].directions[d];

            phy_update_ship_children(&g_red_ships[i]);
        }
}

// All game physics put 'neatly' into a function
void game_physics()
{

    for(int i = 0; i < g_num_blue_ships; i++)
    {
        int dir = phy_get_direction(g_blue_ships[i].current_directions, g_blue_ships[i].last_checked_directions);
        if(dir != -1 && dir != g_blue_ships[i].pointing)
        {
            g_blue_ships[i].pointing = dir;
            phy_update_ship_children(&g_blue_ships[i]);
        }

        phy_move_ship(&g_blue_ships[i]);
    }
}

// checks for a mouse collision with blue ships, only called when the user left clicks but migtht be useful else where
void check_mouse_col()
{
    bool changed = false;
    int prev = g_ship_controlled;

    g_ship_controlled = -1;

    // loop through blue ships
    for(int i = 0; i < g_num_blue_ships; i++)
        if(VEC_dist(g_mouse_pos, g_blue_ships[i].position) <= 20)
        {
            // check for root collision
            if(VEC_eq(g_mouse_pos, g_blue_ships[i].position))
            {
                g_ship_controlled = i;
                changed = true;
                break;
            }

            // check for children collision
            for(int d = 0; d < g_blue_ships[i].children_size; d++)
                if(VEC_eq(g_mouse_pos, g_blue_ships[i].children_positions[d]))
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
            for(int i = 0; i < 4; i++)
                g_blue_ships[g_ship_controlled].current_directions[i] = false;

            g_blue_ships[g_ship_controlled].controlled = true;
        }
    } else if(g_ship_controlled != prev)
    {
        // turn the previously controlled ship back into an AI
        for(int i = 0; i < 4; i++)
            g_blue_ships[prev].current_directions[i] = false;

        g_blue_ships[prev].controlled = false;

        // turn the newly controlled ship into the player
        if(g_ship_controlled != -1)
        {
            for(int i = 0; i < 4; i++)
                g_blue_ships[g_ship_controlled].current_directions[i] = false;

            g_blue_ships[g_ship_controlled].controlled = true;
        }
    }
}

// updates uniform data in the vertex shader
void update_buffer(int which)
{
    vertex_uniform_size = 0;

    // doing this is the fastest way to do this, luckily it doesn't need to be cleared each time
    char name[30];

    if(which == 0 || which == 1)
    {
        // forgot why I called this con, but it's used to switch between blue and red ships
        int con;

	// that switch happens here
        if(which == 0)
            con = g_num_blue_ships;
        else
            con = g_num_red_ships;

        for(int i = 0; i < con; i++)
        {
            // set ship
            Ship tmp;

            if(which == 0)
                tmp = g_blue_ships[i];
            else
                tmp = g_red_ships[i];

	    // if the ship actually exists
            if(tmp.type != SHIP_TYPE_NONE)
            {
		// dumb string stuff
                sprintf(name, "pos[%d]", vertex_uniform_size);

		// set the uniform for the root position
                glUniform2i(glGetUniformLocation(s_program, name), tmp.position.x * 10, tmp.position.y * 10);

                vertex_uniform_size++;

		// the same as above but for the ship's children
                for(int d = 0; d < tmp.children_size; d++)
                {
                    sprintf(name, "pos[%d]", vertex_uniform_size);

                    glUniform2i(glGetUniformLocation(s_program, name), tmp.children_positions[d].x * 10, tmp.children_positions[d].y * 10);

                    vertex_uniform_size++;
                }
            }
        }
    } else
    {
	// mouse
        sprintf(name, "pos[0]");

        glUniform2i(glGetUniformLocation(s_program, name), g_mouse_pos.x * 10, g_mouse_pos.y * 10);

        vertex_uniform_size++;
    }
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
