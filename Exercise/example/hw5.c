#include <stdio.h>
#include <stdbool.h>

int total_height, total_width, step = 0;
int maze[50][50];
int route[50][50] = {0};
int height_index_for_step[2500], width_index_for_step[2500];


bool visited(int height, int width) {
    if (height == total_height - 1 && width == total_width - 1) {
        route[height][width] = 1;
        height_index_for_step[step] = height;
        width_index_for_step[step] = width;
        step += 1;
        return true;
    }

    if (height >= 0 && height < total_height &&
        0 <= width && width < total_width &&
        maze[height][width] == 0 &&
        route[height][width] == 0) {

        route[height][width] = 1;
        height_index_for_step[step] = height;
        width_index_for_step[step] = width;

        step += 1;

        if (visited(height - 1, width + 1))
            return true;
        if (visited(height, width + 1))
            return true;
        if (visited(height + 1, width + 1))
            return true;
        if (visited(height + 1, width))
            return true;
        if (visited(height + 1, width - 1))
            return true;
        if (visited(height - 1, width - 1))
            return true;
        if (visited(height - 1, width))
            return true;
        if (visited(height, width - 1))
            return true;
        route[height][width] = 0;
        step -= 1;
        return false;
    }
    return false;
}

int main() {
    scanf("%d%d", &total_height, &total_width);

    for (int i = 0; i < total_height; i++) {
        for (int j = 0; j < total_width; j++) {
            scanf("%d", &maze[i][j]);
        }
    }

    if (visited(0, 0)) {
        for (int i = 0; i < step; i++) {
            printf("%d %d\n", height_index_for_step[i], width_index_for_step[i]);
        }
    } else {
        printf("no answer");
    }
}
