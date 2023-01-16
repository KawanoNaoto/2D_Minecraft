#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>

typedef struct {
    int hp;
    int power;
    int protection;
} Mobs;
typedef struct {
    int id;
    int effi;   /*効率*/
    int type; /*1...ブロック*/
    int amount;
} Items;

int map[128][272];

/*アイテム対応表*/
/*0...空気(空)
  001...草(草)           002...土(土)
  003...石(石)           004...丸石(丸)
  005...原木(木)         006...鉄鉱石(鉄)
  007...金鉱石（金）      008...石炭鉱石(炭)
  009...ダイヤ鉱石（ダ）  010...黒曜石(黒)
  099...岩盤(盤)         100...頭
  101...体
*/
Items block[200];
Items inventory[4][9];

Mobs players = {20, 1, 0};

void map_structure(int mode)
{
    for(int i = 0; i < 272; i++){
        map[0][i] = 99;
    }

    if(mode == 1){
        printf("Didn't prepared\n");
        exit(0);
    }
    else if(mode == 2){
        for(int i = 0; i < 272; i++){
            map[1][i] = 2;
            map[2][i] = 2;
            map[3][i] = 1;
            for(int j = 4; j < 128; j++){
                map[j][i] = 0;
            }
        }
    }
    else if(mode == 3){
        return;
    }
}

void itemcomparator(int id)
{
    if(id == block[1].id){
        printf("草");
    }
    else if(id == block[2].id){
        printf("土");
    }
    else if(id == block[3].id){
        printf("石");
    }
    else if(id == block[4].id){
        printf("丸");
    }
    else if(id == block[5].id){
        printf("木");
    }
    else if(id == block[6].id){
        printf("鉄");
    }
    else if(id == block[7].id){
        printf("金");
    }
    else if(id == block[8].id){
        printf("炭");
    }  
    else if(id == block[99].id){
        printf("盤");
    }  
    else if(id == block[100].id){
        printf("＠");
    }  
    else if(id == block[101].id){
        printf("体");
    }  
    else{
        printf("　");
    }
}

void show(int map_x, int map_y, int inhand)         /*ブロック判定&設置*/
{
    for(int i = 10; i >= 1; i--){
        printf("|");
        for(int j = 0; j < 17; j++) itemcomparator(map[map_y + i][map_x + j]);
        printf("|　");

        switch(i){
            case 10: printf("ステータス\n"); break;
            case 9: printf("　体力　　　%d\n", players.hp); break;
            case 8: printf("　力　　　　%d\n", players.power); break;
            case 7:  printf("　防御力　　%d\n", players.protection); break;
            case 6:  printf("道具\n"); break;
            case 5:  printf("　ソード　　\n"); break;
            case 4:  printf("　ピッケル　\n"); break;
            case 3:  printf("　アックス　\n"); break;
            case 2:  printf("　スコップ　\n"); break;
            case 1:  printf("防具\n"); break;
        }
    }
    printf("インベントリ　　　　　　　　　　　　　");
    printf("　頭　　　　\n");
    for(int i = 0; i <= 2; i++){
        printf("|");
        for(int j = 0; j < 9; j++){
            if(i == 1) itemcomparator(inventory[0][j].id);
            else if(i == 2){
                if(inventory[0][j].amount < 10) printf("0");
                printf("%d", inventory[0][j].amount);
            }
            else if(i == 0 && inhand == j) printf("＠");
            else printf("　");
            if(j != 8) printf("　");
        }
        printf("|　");
        if(i == 0) printf("　体　　　　\n");
        else if(i == 1) printf("　足　　　　\n");
        else if(i == 2) printf("　靴　　　　\n");
    }
}

void use(int num, int x, int y)
{
    char move;
    move = getch();
    if(block[num].type == 1){
        switch(move){
            case '7': map[y+1][x-1] = num; break;
            case '8': map[y+1][x] = num; break;
            case 'w': map[y+1][x] = num; break;
            case '9': map[y+1][x+1] = num; break;
            case 'a': map[y][x-1] = num; break;
            case 'd': map[y][x+1] = num; break;
            case '4': map[y-1][x-1] = num; break;
            case '6': map[y-1][x+1] = num; break;
            case '1': map[y-2][x-1] = num; break;
            case '2': map[y-2][x] = num; break;
            case 's': map[y-2][x] = num; break;
            case '3': map[y-2][x+1] = num; break;
        }
    }
}

int main(void)
{
    UINT prevCP = GetConsoleOutputCP();
    SetConsoleOutputCP(CP_UTF8);
    char fname[FILENAME_MAX];
    int mode;
    int x = 136, y = 100; /*0 <= x <= 255, 1 <= y <= 127*/
    char move;
    int map_x = 128, map_y = 94;
    int jump = 0;
    int inhand = 0;
    int get = 0;
    int save_count = 0;
    int command = 0;
    puts("～起動中～");
    puts("起動構成を選んでください。");
    puts("1.サバイバル");
    puts("2.クリエイティブ");
    puts("3.ファイルロード");
    puts("0.やめる");
    do{
        printf("select:"); scanf("%d", &mode);
    }while((mode != 0) && (mode != 1) && (mode != 2) && (mode != 3));

    if(mode == 0) return 0;

    /*初期化*/ 
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 9; j++){
            inventory[i][j].id = 0;
            inventory[i][j].amount = 0;
        }
    }
    block[0].id = 0;
    block[0].type = 0;
    for(int i = 1; i < 100; i++){
        block[i].id = i;
        block[i].type = 1;
    }
    block[100].id = 100;
    block[101].id = 101;
    block[100].type = 0;
    block[101].type = 0;

    map_structure(mode);
    if(mode == 3){
        FILE *fp_r;
        printf("ロードするワールドファイル名:");
        scanf("%s", fname);

        if((fp_r = fopen(fname, "r")) == NULL){
            printf("\aファイルをオープンできません。\n");
            exit(1);
        }
        else{
            fscanf(fp_r, "%d", &mode);
            fscanf(fp_r, "%d", &players.hp);
            fscanf(fp_r, "%d", &players.power);
            fscanf(fp_r, "%d", &players.protection);
            fscanf(fp_r, "%d", &x);
            fscanf(fp_r, "%d", &y);
            for(int i = 0; i < 128; i++){
                for(int j = 0; j < 272; j++){
                    fscanf(fp_r, "%d", &map[i][j]);
                }
            }
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 9; j++){
                    fscanf(fp_r, "%d", &inventory[i][j].id);
                    fscanf(fp_r, "%d", &inventory[i][j].amount);
                }
            }
        }
        map_x = x - 8;
        map_y = y - 6;
        fclose(fp_r);
    }
    else{
        printf("生成する世界の名前:");
        scanf("%s", fname);
    }
    while(block[map[y-2][x]].type == 0){
        y--;
        map_y--;
    }

    FILE *fp_w;

    do{
        do{
            system("cls");
            map[y][x] = 100; map[y-1][x] = 101;
            printf("座標x:%d y:%d\n", x-8, y-1);
            show(map_x, map_y, inhand);
            if(command == 1) printf("コマンドは実装までしばらくお待ちください。\n");
            command = 0;
            move = getch();
            switch(move){
                case 'w':
                {
                    map[y][x] = 0, map[y-1][x] = 0;
                    if(jump > 0){
                        break;
                    }
                    else if((y != 127) && (block[map[y+1][x]].type != 1) && (block[map[y-2][x]].type == 1)){
                        y++;
                        map_y++;
                        jump++;
                    }
                    else if(y == 127) puts("高度限界です。\n");
                    break;
                }
                case 'W':
                {
                    map[y][x] = 0, map[y-1][x] = 0;
                    if(jump > 0){
                        break;
                    }
                    else if((y != 127) && (block[map[y+1][x]].type != 1) && (block[map[y-2][x]].type == 1)){
                        y++;
                        map_y++;
                        jump++;
                    }
                    else if(y == 127) puts("高度限界です。\n");
                    break;
                }
                case 'd':
                {
                    map[y][x] = 0, map[y-1][x] = 0;
                    if((x != 263) && (block[map[y][x+1]].type != 1) && (block[map[y-1][x+1]].type != 1)){
                        x++;
                        map_x++;
                    }
                    break;
                }
                case 'D':
                {
                    map[y][x] = 0, map[y-1][x] = 0;
                    if((x <= 262) && (block[map[y][x+1]].type != 1) && (block[map[y-1][x+1]].type != 1) && (block[map[y][x+2]].type != 1) && (block[map[y-1][x+2]].type != 1)){
                        x = x + 2;
                        map_x = map_x + 2;
                    }
                    else if((x != 263) && (block[map[y][x+1]].type != 1) && (block[map[y-1][x+1]].type != 1)){
                        x++;
                        map_x++;
                    }
                    break;
                }
                case 's':
                {
                    map[y][x] = 0, map[y-1][x] = 0;
                    if((y != 1 && (block[map[y-2][x]].type != 1))){
                        y--;
                        map_y--;
                    }
                    break;
                }
                case 'S':
                {
                    map[y][x] = 0, map[y-1][x] = 0;
                    if((y != 1 && (block[map[y-2][x]].type != 1))){
                        y--;
                        map_y--;
                    }
                    break;
                }
                case 'a':
                {
                    map[y][x] = 0, map[y-1][x] = 0;
                    if((x != 8 && (block[map[y][x-1]].type != 1) && (block[map[y-1][x-1]].type != 1))){
                        x--;
                        map_x--;
                    }
                    break;
                }
                case 'A':
                {
                    map[y][x] = 0, map[y-1][x] = 0;
                    if((x >= 9) && (block[map[y][x-1]].type != 1) && (block[map[y-1][x-1]].type != 1) && (block[map[y][x-2]].type != 1) && (block[map[y-1][x-2]].type != 1)){
                        x = x - 2;
                        map_x = map_x - 2;
                    }
                    else if((x != 8 && (block[map[y][x-1]].type != 1) && (block[map[y-1][x-1]].type != 1))){
                        x--;
                        map_x--;
                    }
                    break;
                }
                case 'b':
                {
                    move = getch();
                    switch(move){
                        case '7':
                        {
                            if(map[y+1][x-1] == 99) break;
                            get = map[y+1][x-1];
                            map[y+1][x-1] = 0;
                            break;
                        }
                        case '8':
                        {
                            if(map[y+1][x] == 99) break;
                            get = map[y+1][x];
                            map[y+1][x] = 0;
                            break;
                        }
                        case 'w':
                        {
                            if(map[y+1][x] == 99) break;
                            get = map[y+1][x];
                            map[y+1][x] = 0;
                            break;
                        }
                        case '9':
                        {
                            if(map[y+1][x+1] == 99) break;
                            get = map[y+1][x+1];
                            map[y+1][x+1] = 0;
                            break;
                        }
                        case 'a':
                        {
                            if(map[y][x-1] == 99) break;
                            get = map[y][x-1];
                            map[y][x-1] = 0;
                            break;
                        }
                        case 'd':
                        {
                            if(map[y][x+1] == 99) break;
                            get = map[y][x+1];
                            map[y][x+1] = 0;
                            break;
                        }
                        case '4':
                        {
                            if(map[y-1][x-1] == 99) break;
                            get = map[y-1][x-1];
                            map[y-1][x-1] = 0;
                            break;
                        }
                        case '6':
                        {
                            if(map[y-1][x+1] == 99) break;
                            get =  map[y-1][x+1];
                            map[y-1][x+1] = 0;
                            break;
                        }
                        case '1':
                        {
                            if(map[y-2][x+1] == 99) break;
                            get = map[y-2][x-1];
                            map[y-2][x-1] = 0;
                            break;
                        }
                        case '2':
                        {
                            if(map[y-2][x] == 99) break;
                            get = map[y-2][x];
                            map[y-2][x] = 0;
                            break;
                        }
                        case 's':
                        {
                            if(map[y-2][x] == 99) break;
                            get = map[y-2][x];
                            map[y-2][x] = 0;
                            break;
                        }
                        case '3':
                        {
                            if(map[y-2][x+1] == 99) break;
                            get = map[y-2][x+1];
                            map[y-2][x+1] = 0;
                            break;
                        }
                    }
                    if(get == 1) get = 2; 
                    int blank = 0;
                    for(int i = 0; i < 9; i++){
                        if(inventory[0][i].id == 0) blank++;
                        else if(get == inventory[0][i].id) inventory[0][i].amount++;
                    }
                    if(blank > 8){
                        inventory[0][0].id = get;
                        inventory[0][0].amount = 1;
                    }
                    break;
                }
                case '1':
                {
                    if(inventory[0][0].id > 0){
                        use(inventory[0][0].id, x, y);
                        inventory[0][0].amount--;
                    }
                    inhand = 0;
                    break;
                }
                case '2':
                {
                    if(inventory[0][1].id > 0){
                        use(inventory[0][1].id, x, y);
                        inventory[0][1].amount--;
                    }
                    inhand = 1;
                    break;
                }
                case '3':
                {
                    if(inventory[0][2].id > 0){
                        use(inventory[0][2].id, x, y);
                        inventory[0][2].amount--;
                    }
                    inhand = 2;
                    break;
                }
                case '4':
                {
                    if(inventory[0][3].id > 0){
                        use(inventory[0][3].id, x, y);
                        inventory[0][3].amount--;
                    }
                    inhand = 3;
                    break;
                }
                case '5':
                {
                    if(inventory[0][4].id > 0){
                        use(inventory[0][4].id, x, y);
                        inventory[0][4].amount--;
                    }
                    inhand = 4;
                    break;
                }
                case '6':
                {
                    if(inventory[0][5].id > 0){
                        use(inventory[0][5].id, x, y);
                        inventory[0][5].amount--;
                    }
                    inhand = 5;
                    break;
                }
                case '7':
                {
                    if(inventory[0][6].id > 0){
                        use(inventory[0][6].id, x, y);
                        inventory[0][6].amount--;
                    }
                    inhand = 6;
                    break;
                }
                case '8':
                {
                    if(inventory[0][7].id > 0){
                        use(inventory[0][7].id, x, y);
                        inventory[0][7].amount--;
                    }
                    inhand = 7;
                    break;
                }
                case '9':
                {
                    if(inventory[0][8].id > 0){
                        use(inventory[0][8].id, x, y);
                        inventory[0][8].amount--;
                    }
                    inhand = 8;
                    break;
                }
                case '/':
                {
                    command = 1;
                }
            }
            for(int i = 0; i < 9; i++){
                if(inventory[0][i].amount == 0){
                    inventory[0][i].id = 0;
                }
            }

            if(jump == 1) jump++;
            else if(block[map[y-2][x]].type != 1){
                map[y][x] = 0;
                y--;
                map_y--;
                jump = 0;
            }
            else jump = 0;

            if(save_count++ > 9){
                save_count = 0;
                
                fp_w = fopen(fname, "w");
                fprintf(fp_w, "%d\n", mode);
                fprintf(fp_w, "%d\n", players.hp);
                fprintf(fp_w, "%d\n", players.power);
                fprintf(fp_w, "%d\n", players.protection);
                fprintf(fp_w, "%d\n", x);
                fprintf(fp_w, "%d\n", y);
                for(int i = 0; i < 128; i++){
                    for(int j = 0; j < 272; j++){
                        fprintf(fp_w, "%d ", map[i][j]);
                    }
                }
                for(int i = 0; i < 4; i++){
                    for(int j = 0; j < 9; j++){
                        fprintf(fp_w, "%d ", inventory[i][j].id);
                        fprintf(fp_w, "%d ", inventory[i][j].amount);
                    }
                }
                fclose(fp_w);
            }
        }while(move != 'p');

        printf("Close this map? y/n:"); move = getch();
        if(move != 'n') break;
    }while(move == 'n');

    /*セーブ*/
    fp_w = fopen(fname, "w");
    fprintf(fp_w, "%d\n", mode);
    fprintf(fp_w, "%d\n", players.hp);
    fprintf(fp_w, "%d\n", players.power);
    fprintf(fp_w, "%d\n", players.protection);
    fprintf(fp_w, "%d\n", x);
    fprintf(fp_w, "%d\n", y);
    for(int i = 0; i < 128; i++){
        for(int j = 0; j < 272; j++){
            if(block[map[i][j]].id >= 100){
                fprintf(fp_w, "00 ");
            }
            else fprintf(fp_w, "%d ", block[map[i][j]].id);
        }
        fprintf(fp_w, "\n");
    }
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 9; j++){
            fprintf(fp_w, "%d ", inventory[i][j].id);
            fprintf(fp_w, "%d ", inventory[i][j].amount);
        }
        fprintf(fp_w, "\n");
    }
    fclose(fp_w);

    puts("");
    system("cls");
    SetConsoleOutputCP(prevCP);
    return 0;
}