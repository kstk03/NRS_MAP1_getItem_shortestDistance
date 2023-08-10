/*MAP1_player��item���ŒZ�����Ŏ擾����*/

#include <stdio.h>
#include <stdbool.h>
#include <vector>

/* MAP��HEIGHT��WIDTH */
#define MAP_HEIGHT 7
#define MAP_WIDTH 11

#define PATH 0
#define WALL 1
#define PASSED 'X'
#define ITEM 'I'
#define START 'S'
#define GOAL 'G'
#define PLAYER 'P'
#define ENEMY_A 'A'
#define ENEMY_B 'B'


typedef enum {//player�̗񋓌^
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef enum {//enemyA�̗񋓌^
    A_DOWN,
    A_LEFT,
    A_UP,
    A_RIGHT
} Direction_enemyA;

typedef enum {//enemyB�̗񋓌^
    B_UP,
    B_LEFT,
    B_DOWN,
    B_RIGHT
} Direction_enemyB;


typedef struct {
    int x;
    int y;
} Position;

bool ISVALID_MOVE(Position position);
bool ISGOAL(Position position);
void collectItems();
void reachGoal();
void moveEnemyA();
void moveEnemyB();
void PRINT_MAP(void);

Position player, start, enemyA, enemyB, goal;

const int items = 5; //�A�C�e���̑���
int remainingItems;      // ITEM�̎c��̐�
int score;
int turns;



/* MAP�̏ڍ� */
static int MAP[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 'I', 0, 0, 'A', 1, 'B', 0, 0, 'G', 1},
    {1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1},
    {1, 0, 1, 0, 'I', 1, 'I', 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
    {1, 'S', 0, 0, 0, 'I', 0, 0, 0, 'I', 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

/* MAP�̕\�� */
void PRINT_MAP(void)
{
    int x, y;

    for (y = 0; y < MAP_HEIGHT; y++)
    {
        for (x = 0; x < MAP_WIDTH; x++)
        {
            /* �z��̒l�ɉ����ċL��/������\�� */
            if (MAP[y][x] == PATH)
            {
                printf(" ");
            }
            if (MAP[y][x] == WALL)
            {
                printf("#");
            }
            if (MAP[y][x] == ITEM)
            {
                printf("��");
            }
            if (MAP[y][x] == START)
            {
                printf("S");
            }
            if (MAP[y][x] == GOAL)
            {
                printf("G");
            }
            if (MAP[y][x] == ENEMY_A)
            {
                printf("A");
            }
            if (MAP[y][x] == ENEMY_B)
            {
                printf("B");
            }
            if (MAP[y][x] == PLAYER)
            {
                printf("P");
            }
        }
        printf("\n");
    }
    printf("\n");
}

bool ISVALID_MOVE(Position position) {

    if (position.x >= 0 && position.x < MAP_WIDTH && position.y >= 0 && position.y < MAP_HEIGHT && MAP[position.y][position.x] == PATH)
    {
        return true;
    }
    return false;
}


/* GOAL�̊m�F */
bool ISGOAL(Position position)
{
    if (turns > 0 && remainingItems == 0 && position.x == goal.x && position.y == goal.y)
    {
        return true;
    }
    return false;
}

//item���W�߂�֐�
void collectItems() {
    int i = 0;
    Position itemsPosition[items];
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (MAP[y][x] == ITEM) {
                itemsPosition[i].y = y;
                itemsPosition[i].x = x;
                i++;
            }
        }
    }

    // �A�C�e���̈ʒu�܂ł̋������v�Z���A�ł��߂��A�C�e���̈ʒu��I��
    int minDistance = MAP_WIDTH + MAP_HEIGHT;  // �\���ɑ傫�ȏ����l
    Position closestItemPosition;

    for (int i = 0; i < items; i++) {
        int dx = itemsPosition[i].x - player.x;
        int dy = itemsPosition[i].y - player.y;
        int distance = abs(dx) + abs(dy);

        if (distance < minDistance) {
            minDistance = distance;
            closestItemPosition = itemsPosition[i];
        }
    }

    // �v���C���[�̈ړ����������肵�ړ�
    int dx = closestItemPosition.x - player.x;
    int dy = closestItemPosition.y - player.y;

    Position newPlayerPosition = player;

    if (abs(dx) > abs(dy)) {
        newPlayerPosition.x += (dx > 0) ? 1 : -1;
    }
    else {
        newPlayerPosition.y += (dy > 0) ? 1 : -1;
    }

    if (ISVALID_MOVE(newPlayerPosition) || MAP[newPlayerPosition.y][newPlayerPosition.x] == ITEM) {
        if (MAP[newPlayerPosition.y][newPlayerPosition.x] == ITEM) {
            remainingItems--;
            printf("�c���ITEM�̐���%d�ł�\n", remainingItems);
        }
        MAP[player.y][player.x] = PATH;
        MAP[newPlayerPosition.y][newPlayerPosition.x] = PLAYER;
        player = newPlayerPosition;
    }
}

//goal�ɍs���֐�
void reachGoal() {
    int dx = goal.x - player.x;
    int dy = goal.y - player.y;

    Position newPlayerPosition = player;

    if (abs(dx) > abs(dy)) {
        newPlayerPosition.x += (dx > 0) ? 1 : -1;
    }
    else {
        newPlayerPosition.y += (dy > 0) ? 1 : -1;
    }

    if (ISVALID_MOVE(newPlayerPosition)) {
        MAP[player.y][player.x] = PATH;
        MAP[newPlayerPosition.y][newPlayerPosition.x] = PLAYER;
        player = newPlayerPosition;
    }
}



// EnemyA�̈ړ��֐�
void moveEnemyA() {
    int dx = player.x - enemyA.x;
    int dy = player.y - enemyA.y;

    Position New = enemyA;
    bool moved = false; // �ړ��������������̃t���O

    //1�ڂ̏���
    if (dy != 0) {
        New.y = enemyA.y + (dy > 0 ? 1 : -1);
        if (ISVALID_MOVE({ enemyA.x, New.y }) && MAP[New.y][enemyA.x] != ITEM) {
            MAP[enemyA.y][enemyA.x] = PATH;
            MAP[New.y][enemyA.x] = 'A';
            enemyA.y = New.y;
            moved = true;
            return;
        }
    }
    //2�ڂ̏���
    if (dx != 0) {
        New.x = enemyA.x + (dx > 0 ? 1 : -1);
        if (ISVALID_MOVE({ New.x, enemyA.y }) && MAP[enemyA.y][New.x] != ITEM) {
            MAP[enemyA.y][enemyA.x] = PATH;
            MAP[enemyA.y][New.x] = 'A';
            enemyA.x = New.x;
            moved = true;
            return;
        }
    }
    //3�ڂ̏���
    if (!moved) {
        Direction_enemyA nextDirections[4] = { A_DOWN, A_LEFT, A_UP, A_RIGHT };

        for (int i = 0; i < 4; i++) {
            New = enemyA;
            Direction_enemyA nextDirection = nextDirections[i];

            switch (nextDirection) {
            case A_UP:
                New.y--;
                break;
            case A_RIGHT:
                New.x++;
                break;
            case A_DOWN:
                New.y++;
                break;
            case A_LEFT:
                New.x--;
                break;
            }
            if (ISVALID_MOVE(New)) {
                MAP[enemyA.y][enemyA.x] = PATH;
                MAP[New.y][New.x] = ENEMY_A;
                enemyA = New;
                break;
            }
        }
    }
}

// EnemyB�̈ړ��֐�
void moveEnemyB() {
    int dx = player.x - enemyB.x;
    int dy = player.y - enemyB.y;

    Position New = enemyB;
    bool moved = false; // �ړ��������������̃t���O

    //����1
    if (dx != 0) {
        New.x = enemyB.x + (dx > 0 ? 1 : -1);/*�������Z�q��p����y���� ? �^�̏ꍇ�̒l : �U�̏ꍇ�̒l�z(dx > 0 : ++1) or (dx < 0 : --1)*/
        if (ISVALID_MOVE({ New.x, enemyB.y }) && MAP[enemyB.y][New.x] != ITEM) {
            MAP[enemyB.y][enemyB.x] = PATH;
            MAP[enemyB.y][New.x] = 'B';
            enemyB.x = New.x;
            moved = true;
            return;
        }
    }

    //����2

    if (dy != 0) {
        New.y = enemyB.y + (dy > 0 ? 1 : -1);
        if (ISVALID_MOVE({ enemyB.x, New.y }) && MAP[New.y][enemyB.x] != ITEM) {
            MAP[enemyB.y][enemyB.x] = PATH;
            MAP[New.y][enemyB.x] = 'B';
            enemyB.y = New.y;
            moved = true;
            return;
        }
    }

    //����3
    if (!moved) {
        Direction_enemyB nextDirections[4] = { B_UP ,B_LEFT, B_DOWN, B_RIGHT };

        for (int i = 0; i < 4; i++) {
            New = enemyB;
            Direction_enemyB nextDirection = nextDirections[i];

            switch (nextDirection) {
            case B_UP:
                New.y--;
                break;
            case B_RIGHT:
                New.x++;
                break;
            case B_DOWN:
                New.y++;
                break;
            case B_LEFT:
                New.x--;
                break;
            }
            if (ISVALID_MOVE(New)) {
                MAP[enemyB.y][enemyB.x] = PATH;
                MAP[New.y][New.x] = ENEMY_B;
                enemyB = New;
                break;
            }
        }
    }
}

int main(void) {
    //player
    player.x = 1;
    player.y = 5;
    //enemyA
    enemyA.x = 4;
    enemyA.y = 1;
    //enemyB
    enemyB.x = 6;
    enemyB.y = 1;

    //goal
    goal.x = 9;    // goal_x�ʒu
    goal.y = 1;    // goal_y�ʒu

    remainingItems = 5;  // �c���ITEM�̐�
    turns = 50;         // �^�[����

    printf("MAP1�̏ڍ�\n");
    printf("--------------------\n");
    printf("#:��, ��:�A�C�e��, S:START, G:GOAL\n\n");
    PRINT_MAP();
    printf("--------------------\n");
    while (turns > 0) {
        if (remainingItems == 0 && player.x == goal.x && player.y == goal.y) {
            printf("�S�[�����܂���!\n");
            printf("�X�R�A��%d�ł�!", score);
            break;
        }

        if (remainingItems > 0) {
            collectItems();
        }
        else if (remainingItems == 0) {
            reachGoal();
        }


        moveEnemyA();
        moveEnemyB();
        PRINT_MAP();//player�Eenemy���ړ��������MAP�̏��
        score = turns; // �^�[����-1�����O�̒l���X�R�A�ƂȂ�
        turns--;
        printf("�^�[��: %d\n", turns);

        if (turns == 0) {
            printf("�^�[�����Ȃ��Ȃ�܂����B�S�[���ł��܂���ł���\n");
        }
    }
    PRINT_MAP();

    return 0;
}
