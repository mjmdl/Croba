#include <stdio.h>
#include <conio.h>
#include <Windows.h>

typedef enum { FORWARD, BACKWARD, LEFTWARD, RIGHTWARD} Dir;

typedef struct
{ 
    int* x,* y; 
    int size; 
    Dir dir;
} Snake;

typedef struct 
{ 
    int x, y; 
} Vec2;

Snake create_snake(int size, const Vec2* field)
{
    size = size < 1 ? 1 : size;

    int max_size = field->x * field->y;
    Snake s;
    s.x = (int*) malloc(sizeof(int) * max_size);
    s.y = (int*) malloc(sizeof(int) * max_size);
    s.size = size;
    s.dir = RIGHTWARD;

    for (int i = 0; i < size; i++)
    {
        s.x[i] = field->x / 2 - i;
        s.y[i] = field->y / 2;
    }

    return s;
}

Vec2 create_food(const Vec2* field, const Snake* snake)
{
    Vec2 food;

    food.x = rand() % field->x;
    food.y = rand() % field->y;

    for (int i = 0; i < snake->size; i++)
    {
        if (food.x == snake->x[i] && food.y == snake->y[i])
        {
            return create_food(field, snake);
        }
    }

    return food;
}

void print_title(int score)
{
    if (score < 0)
    {
        printf("\n[] Croba.exe []");
    }
    else
    {
        printf("\n[] Croba.exe [] Score: %d.", score);
    }
}

void print_line(int width, const char* str)
{
    for (int x = 0; x < width; x++)
    {
        printf(str);
    }
}

void draw_field(const Vec2* field, const Snake* snake, const Vec2* food)
{
    const char* wall_str = "[]";
    const char* floor_str = "  ";
    const char* food_str = "++";
    const char* head_str = "OO";
    const char* tail_str = "<>";

    printf("\n");

    print_line(field->x + 2, wall_str);
    printf("\n");

    for (int y = 0; y < field->y; y++)
    {
        printf(wall_str);
        for (int x = 0; x < field->x; x++)
        {
            if (x == snake->x[0] && y == snake->y[0])
            {
                printf(head_str);
            }
            else if (x == food->x && y == food->y)
            {
                printf(food_str);
            }
            else
            {
                int is_tail = 0;
                for (int i = 1; i < snake->size; i++)
                {
                    if (x == snake->x[i] && y == snake->y[i])
                    {
                        is_tail = 1;
                        break;
                    }
                }

                if (is_tail)
                {
                    printf(tail_str);
                }
                else
                {
                    printf(floor_str);
                }
            }
        }
        printf("%s\n", wall_str);
    }

    print_line(field->x + 2, wall_str);
    printf("\n");
}

void get_move_input(Dir* current)
{
    if (!kbhit())
    {
        return;
    }

    switch (getch())
    {
        case 'w':
            if (*current != BACKWARD)
                *current = FORWARD;
            break;
        case 's':
            if (*current != FORWARD)
                *current = BACKWARD;
            break;
        case 'a':
            if (*current != RIGHTWARD)
                *current = LEFTWARD;
            break;
        case 'd':
            if (*current != LEFTWARD)
                *current = RIGHTWARD;
            break;
    }
}

void move_snake(Snake* snake)
{
    for (int i = snake->size; i > 1; i--)
    {
        snake->x[i - 1] = snake->x[i - 2];
        snake->y[i - 1] = snake->y[i - 2];
    }

    switch (snake->dir)
    {
        case FORWARD:
        {
            snake->y[0] -= 1;
            break;
        }
        case BACKWARD:
        {
            snake->y[0] += 1;
            break;
        }
        case LEFTWARD:
        {
            snake->x[0] -= 1;
            break;
        }
        case RIGHTWARD:
        {
            snake->x[0] += 1;
            break;
        }
    }
}

void grow_snake(Snake* snake)
{
    snake->size++;
    snake->x[snake->size - 1] = snake->x[snake->size - 2];
    snake->y[snake->size - 1] = snake->y[snake->size - 2];
}

int check_for_food(Vec2* food, const Vec2* field, Snake* snake)
{
    if (food->x == snake->x[0] && food->y == snake->y[0])
    {
        *food = create_food(field, snake);
        grow_snake(snake);
        return 1;
    }

    return 0;
}

int is_colliding(const Vec2* field, const Snake* snake)
{
    if (snake->x[0] < 0 || snake->x[0] > field->x - 1)
    {
        return 1;
    }
    
    if (snake->y[0] < 0 || snake->y[0] > field->y - 1)
    {
        return 1;
    }

    if (snake->size > 2)
    {
        for (int i = 1; i < snake->size; i++)
        {
            if (snake->x[0] == snake->x[i] && snake->y[0] == snake->y[i])
            {
                return 1;
            }
        }
    }

    return 0;
}

int main()
{
    const int frame_delay = 100;
    int score = 0;

    system("cls");
    print_title(-1);
    Vec2 field = { 16, 16 };
    Snake snake = create_snake(1, &field);
    Vec2 food = create_food(&field, &snake);

    print_title(score);
    draw_field(&field, &snake, &food);

    printf("\n\nPress a key to start.");
    while (!kbhit());

    while (!is_colliding(&field, &snake))
    {
        system("cls");
        print_title(score);
        draw_field(&field, &snake, &food);

        get_move_input(&snake.dir);
        move_snake(&snake);

        score += check_for_food(&food, &field, &snake);
        
        Sleep(frame_delay);
    }

    printf("\n\t\tGAME OVER!");

    Sleep(1000);

    printf("\n\tPress a key to close.");
    while (!kbhit());

    return 0;
}
