#include <iostream>
#include <vector>
#include <chrono>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>


int rounder(float d)
{
  return floor(d + 0.5);
}

class Circle {
    public:
        //radius, x-coordinate, y-coordinate, x-velocity (p/s), y-velocity (p/s), mass (kg), coefficient of friction (mu), perimeter (p)
        float r, x, y, vx, vy, m, u, perimeter;
        void set_values(float, float, float, int);
        std::vector<std::vector<float>> edge;
        float calculate_new_position(float, float);
};

void Circle::set_values (float setr, float setx, float sety, float setm, int setresolution) {
    r = setr; x = setx; y = sety; m = setm;
    for (int i = 0; i < setresolution; i++) {
        edge.push_back({sin(i*((2*M_PI)/setresolution)) * setr,cos(i*((2*M_PI)/setresolution)) * setr});
    }
    perimeter = 2 * M_PI * setr;
}

void print_window (int plane[100][100]) {
    std::string canvas;
    for(size_t y = 0; y < 100; y++) {
        char line[200];
        for(size_t x = 0; x < 100; x++) {
            if(plane[x][y] == 1) {
                line[x * 2] = '0';
                line[x * 2 - 1] = '0';
            } else {
                line[x * 2] = ' ';
                line[x * 2 - 1] = ' ';
            }
        }
        canvas.append(line);
        canvas.append("\n");
    }
    std::cout << canvas << std::endl;
}

void set_console_cursor(int x, int y, HANDLE console) {
    COORD ccoord;
    ccoord.X = x;
    ccoord.Y = y;
    SetConsoleCursorPosition(console, ccoord);
}

int main()
{
    int total_time;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::vector<Circle> circle_list;
    Circle ball;
    ball.set_values(10,50,50,1,300);
    circle_list.push_back(ball);
    int canvas[100][100] = {0};
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; j++) {
                canvas[i][j] = 0;
            }
        }
        for (size_t i = 0; i < circle_list.size(); i++) {
            //circle_list[i].x+=.05;
            //circle_list[i].y+=.1;
            for (size_t j = 0; j < circle_list[i].edge.size(); j++) {
                canvas[rounder(circle_list[i].edge[j][0] + circle_list[i].x)][rounder(circle_list[i].edge[j][1] + circle_list[i].y)] = 1;
            }
        }
        set_console_cursor(0, 1, hConsole);
        print_window(canvas);

        auto stop = std::chrono::high_resolution_clock::now();
        auto delta_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        total_time += delta_time.count();
        set_console_cursor(0, 0, hConsole);
        std::cout << total_time << "      ";
    }
    return 0;
}
