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
        //radius, x-coordinate, y-coordinate, x-velocity (m/s), y-velocity (m/s), mass (kg), coefficient of friction (mu), perimeter (m)
        double r, x, y, m, u, perimeter;
        double vx = 0;
        double vy = 0;
        bool enable_gravity;
        void set_values(double, double, double, double, int, bool);
        std::vector<std::vector<float>> edge;
        void calculate_new_position(double, double);
        double velocity();
};

double Circle::velocity() {
    return sqrt(pow(vx, 2) + pow(vy, 2));
}

void Circle::set_values (double setr, double setx, double sety, double setm, int setresolution, bool set_gravity) {
    r = setr; x = setx; y = sety; m = setm;
    enable_gravity = set_gravity;
    for (int i = 0; i < setresolution; i++) {
        edge.push_back({sin(i*((2*M_PI)/setresolution)) * setr,cos(i*((2*M_PI)/setresolution)) * setr});
    }
    perimeter = 2 * M_PI * setr;
}

void Circle::calculate_new_position(double delta_time, double ppm) {
    x = x + (ppm * vx * (delta_time / 1000000));
    y = y + (ppm * vy * (delta_time / 1000000));
}

void print_window (int plane[100][100]) {
    std::string canvas;
    for(size_t y = 100; y > 0; y--) {
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
    double pixels_per_metre = 5;
    int total_time;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::vector<Circle> circle_list;
    Circle ball;
    ball.set_values(5,47,100,1,300, false);
    ball.vy = -5;
    circle_list.push_back(ball);
    Circle ball_1;
    ball_1.set_values(8,50,30,1,300, false);
    circle_list.push_back(ball_1);
    int canvas[100][100] = {0};
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; j++) {
                canvas[i][j] = 0;
            }
        }

        for (size_t i = 0; i < circle_list.size(); i++) {
            for (size_t j = 0; j < circle_list[i].edge.size(); j++) {
                int xpos = rounder(circle_list[i].edge[j][0] + circle_list[i].x);
                int ypos = rounder(circle_list[i].edge[j][1] + circle_list[i].y);
                if (xpos < 100 && ypos < 100 && xpos > 0 && ypos > 0) {
                    canvas[xpos][ypos] = 1;
                }
            }
        }
        //int lolcount;
        for (size_t i = 0; i < circle_list.size(); i++) {
            for (size_t j = 0; j < circle_list.size(); j++) {
                double angle_of_bounce;
                double distance_xy[2] = {0,0};
                distance_xy[0] = circle_list[i].x - circle_list[j].x;
                distance_xy[1] = circle_list[i].y - circle_list[j].y;
                double sum_radius = pow(circle_list[i].r + circle_list[j].r, 2);
                double distance = pow(distance_xy[0], 2) + pow(distance_xy[1], 2);
                if ((j != i) && (sum_radius > distance)) {
                    if (distance_xy[0] < 0) {
                        angle_of_bounce = ((0.5 * M_PI) - atan(distance_xy[1] / distance_xy[0])) + (1.5 * M_PI);
                        set_console_cursor(201, 3, hConsole);
                        std::cout << angle_of_bounce;
                        _sleep(4000);
                    }
                }
                //double angle_of_bounce = atan(y_distance / x_distance);
                //set_console_cursor(201, lolcount, hConsole);
                //std::cout << "x: " << distance_xy[0];
                //set_console_cursor(208, lolcount, hConsole);
                //std::cout << "y: " << distance_xy[1];
                //lolcount++;
/*
                double x_distance;
                double y_distance;
                bool direction_of_bounce = false;

                if (circle_list[i].x > circle_list[j].x) {
                    x_distance = circle_list[i].x - circle_list[j].x;
                } else {
                    x_distance = circle_list[j].x - circle_list[i].x;
                    direction_of_bounce = true;
                }

                if (circle_list[i].y > circle_list[j].y) {
                    y_distance = circle_list[i].y - circle_list[j].y;
                } else {
                    y_distance = circle_list[j].y - circle_list[i].y;
                    direction_of_bounce = true;
                }
                double distance = pow(x_distance, 2) + pow(y_distance, 2);
                double sum_radius = pow(circle_list[i].r + circle_list[j].r, 2);
                if ((j != i) && (sum_radius > distance)) {
                    double angle_of_bounce = atan(y_distance / x_distance);
                    if (direction_of_bounce) {
                        angle_of_bounce = (M_PI) - angle_of_bounce;
                    }
                    circle_list[i].x += cos(angle_of_bounce) * 0.5;
                    circle_list[i].y += sin(angle_of_bounce) * 0.5;
                    //circle_list[j].x -= cos(angle_of_bounce) * 0.5;
                    //circle_list[j].y -= sin(angle_of_bounce) * 0.5;
                    circle_list[i].vx = cos(angle_of_bounce) * circle_list[i].velocity() * (circle_list[i].m/(circle_list[i].m+circle_list[j].m));
                    circle_list[i].vy = sin(angle_of_bounce) * circle_list[i].velocity() * (circle_list[i].m/(circle_list[i].m+circle_list[j].m));
                    //circle_list[j].vx = cos(angle_of_bounce) * circle_list[i].velocity() * -1 * (circle_list[j].m/(circle_list[i].m+circle_list[j].m));
                    //circle_list[j].vy = sin(angle_of_bounce) * circle_list[i].velocity() * -1 * (circle_list[j].m/(circle_list[i].m+circle_list[j].m));
                }
                */
            }
        }

        set_console_cursor(0, 1, hConsole);
        print_window(canvas);
        auto stop = std::chrono::high_resolution_clock::now();
        auto delta_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        total_time += delta_time.count();
        for (size_t i = 0; i < circle_list.size(); i++) {
            if (circle_list[i].enable_gravity == true) {
                circle_list[i].vy -= 9.8 * delta_time.count() / 1000000;
            }
            circle_list[i].calculate_new_position(delta_time.count(), pixels_per_metre);
        }

        set_console_cursor(0, 0, hConsole);
        std::cout << total_time << "      ";

    }
    return 0;
}
