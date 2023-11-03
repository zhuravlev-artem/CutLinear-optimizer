#include <stdio.h>
#include <windows.h>
#include <stdbool.h>


int combination[1000] [1000];                        //доска и номер детали
int best_combination[1000] [1000];                   //лучшее расположение
int old_remnat=10000;
int board[1000];
int part[1000];
int part_count[1000];              //кол-во деталей на одной доске*/
int lenght_boards=0;
int lenght_parts=0;
int disk_thickness=0;
int old_take_boards=1000;

void inputs(){
    for(int i=0; i<1000; i++){
        part_count[i]=0;
        board[i]=0;
        part[i]=0;
    }
    lenght_boards=0;
    lenght_parts=0;
    disk_thickness=0;

    int i=0;
    while(1){
        int scan4=1;
        printf("доска №%d:", i+1);
        scanf("%d", &scan4);
        if(scan4==0){break;}
        board[i]=scan4;
        lenght_boards=i+2;
        i++;
    }
    board[lenght_boards-1]=10000;
    printf("\n");

    i=1;
    part[0]=0;
    while(1){
        int scan4=1;
        printf("деталь №%d:", i);
        scanf("%d", &scan4);
        if(scan4==0){break;}
        lenght_parts=i+1;
        part[i]=scan4;
        i++;
    }

    printf("\nтолщина пилы:");
    scanf("%d", &disk_thickness);
    printf("\n");
}


void init(){                                    //инициализация нулями
    for(int i=0; i<lenght_boards; i++){
        for(int j=0; j<lenght_parts; j++){
            best_combination[i] [j]=0;
            combination[i] [j]=0;
        }
    }
}


int summ_length_parts(int b){                   //какой длинны все установленные на доске детаи?
    int buff=0;
    for(int i=0; i<lenght_parts; i++){
        buff += part[combination[b][i]];
        if(i>0&&combination[b][i]!=0){
            buff += disk_thickness;
        }
    }
return buff;
}


bool can_place_part(int p, int b){              //можем ли мы расположить деталь на доску?
    int d, f=summ_length_parts(b);
    if(f==0){
        d=0;
    }else{
        d=1;
    }
    if(f + part[p] + disk_thickness*d <=board[b]){
        return TRUE;
    } else{
        return FALSE;
    }
}


void place_part(int p, int b){                  //расположить доску
    combination[b] [part_count[b]] = p;
    part_count[b]++;
}


void remove_part(int b){                        //удалить доску
    if(combination[b] [0]!=0){
        part_count[b]--;
        combination[b] [part_count[b]]=0;
    }
}


void copy_best_combination(){                   //скопировать лучшую комбинацию и еë параметры
    for(int i=0; i<lenght_boards; i++){
        for(int j=0; j<lenght_parts+1; j++){
            best_combination[i] [j] = combination[i] [j];
        }
    }
old_take_boards = take_boards();
old_remnat = summ_length_parts(lenght_boards-1);  //запомнить суммарную длинну дкталей на мнимой доске
printmass(0);
}


int take_boards(){
    int buff=0;
    for(int i=0; i<lenght_boards-1; i++){
        if(combination[i][0]!=0){buff++;}
    }
return buff;
}


void check(){                                   //проверка на оптимальность
    int a=summ_length_parts(lenght_boards-1);   //остаток на мнимой доске
    if(old_remnat>a){                           //если суммарная длинна деталей на мнимой доске меньше, чем в последний раз, то
        copy_best_combination();                //запомнить комбинацию
    }else if(old_remnat==a && take_boards()<old_take_boards){
        copy_best_combination();
    }
}


void recurs(int p){                             //рекурсивный проход и проверка на оптимальность
    if(p<lenght_parts){                         //если переданная деталь не последняя, то
        for(int i=0; i<lenght_boards; i++){     //на каждой доске
            if(can_place_part(p, i)){           //попытаться расположить деталь
                place_part(p, i);               //если вмещается, то расположить, затем
                recurs(p+1);                    //вызвать этот же цикл, передать следующую деталь
                remove_part(i);                 //затем убрать последнюю деталь
            }
        }
    }else{                                      //если передаваемой детали не существует, то
        check();                                //сделать проверку на оптимальность
    }
}


void printmass(int a){
if(a==0){

    for(int i=0; i<lenght_boards-1; i++){
        printf("(%d):", board[i]);

        for(int j=0; j<lenght_parts; j++){
            if(part[best_combination[i][j]]!=0){
                printf(" (№%d)%d", best_combination[i][j], part[best_combination[i][j]]);
            }
        }
        printf("\n");
    }
    printf("(inf):");

    for(int j=0; j<lenght_parts; j++){
        if(part[best_combination[lenght_boards-1][j]]!=0){
            printf(" (№%d)%d", best_combination[lenght_boards-1][j], part[best_combination[lenght_boards-1][j]]);
        }
    }
    printf("\n\n");
}

else if(a==1){

    for(int i=0; i<lenght_boards; i++){

        if(i<lenght_boards-1){
            printf("(%d):", board[i]);
        }else{printf("(inf):");}

        for(int j=0; j<lenght_parts; j++){
            printf("%d; ", part[combination[i][j]]);
        }
        printf("\n");
        if(i<lenght_boards-1){
            printf("(%d):", board[i]);
        } else{
            printf("(inf):");
        }
        for(int k=0; k<part_count[i]; k++){
            printf("   ");
        }
    printf("^ %d", part_count[i]);
    printf("\n");
    }
    printf("\n");
}

}


int main(){
    SetConsoleOutputCP(CP_UTF8);
    init();
    inputs();
    recurs(1);
    system("pause");
return 0;
}
