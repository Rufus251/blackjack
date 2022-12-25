#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>

typedef struct queue_element{
    char suit[3];
    int points;
    struct queue_element *next_element;
}queue_element;
typedef struct queue{
    int len;
    queue_element *first, *last;
}queue;
typedef struct player{
    int bank, bet, sum_points;
    queue *deck;
}play;

int rand_int(int l, int r){
    return (rand() % (r - l + 1) + l);
} // Рандомное число

void insert_element(queue *queue, queue_element *insert, int pozition){
    queue_element *iterator;
    if (pozition == 0){
        queue->first = insert;
        queue->last = insert;
        queue->len++;
    }
    else if (pozition == queue->len){
        iterator = queue->first;
        for (int i = 0; i < pozition - 1; i++) {
            iterator = iterator->next_element;
        }
        iterator->next_element = insert;
        queue->len++;
        queue->last = insert;
    }
    else {
        iterator = queue->first;
        for (int i = 0; i < pozition - 1; i++) {
            iterator = iterator->next_element;
        }
        queue_element *a = iterator->next_element;
        iterator->next_element = insert;
        insert->next_element = a;
        queue->len++;
    }
} // Вставка в элемент списка
queue_element *pop(queue *queue) {
    queue_element *iterator;
    queue->len--;
    iterator = queue->first;
    if (queue->len == 0){
        queue->last = NULL;
        queue_element *a = queue->first;
        return a;
    }
    for (int i = 0; i < queue->len - 1; i++) {
        iterator = iterator->next_element;
    }
    queue->last = iterator;
    queue_element *a = iterator->next_element;
    iterator->next_element = NULL;
    return a;
} // Удаление последнего элемента списка и его возврат

queue_element create_element(){
    queue_element card;
    card.points = 0;
    card.suit[0] = 'a';
    card.suit[1] = 'a';
    card.suit[2] = 'a';
    card.next_element = NULL;
    return card;
} // Создание карты
queue create_deck(queue deck){
    deck.len = 52;
    queue_element *iterator;
    for (int i = 0; i < 52; i++){
        queue_element *new = (queue_element*)malloc(sizeof(queue_element));
        if (i < 32) {
            new->suit[0] = 0;
            new->suit[1] = i % 4 + 3;
            new->suit[2] = i / 4 + 50;
        }
        else if (i >= 32 && i < 36){
            new->suit[0] = i % 4 + 3;
            new->suit[1] = 49;
            new->suit[2] = 48;
        }
        else if (i >= 36 && i < 40){
            new->suit[0] = 0;
            new->suit[1] = i % 4 + 3;
            new->suit[2] = 74;
        }
        else if (i >= 40 && i < 44){
            new->suit[0] = 0;
            new->suit[1] = i % 4 + 3;
            new->suit[2] = 81;
        }
        else if (i >= 44 && i < 48){
            new->suit[0] = 0;
            new->suit[1] = i % 4 + 3;
            new->suit[2] = 75;
        }
        else if (i >= 48 && i < 52){
            new->suit[0] = 0;
            new->suit[1] = i % 4 + 3;
            new->suit[2] = 65;
        }
        switch (i / 4) {
            case 0: new->points = 2;
                break;
            case 1: new->points = 3;
                break;
            case 2: new->points = 4;
                break;
            case 3: new->points = 5;
                break;
            case 4: new->points = 6;
                break;
            case 5: new->points = 7;
                break;
            case 6: new->points = 8;
                break;
            case 7: new->points = 9;
                break;
            case 12: new->points = 11;
                break;
            default: new->points = 10;
                break;
        }
        if (i == 0){
            deck.first = new;
            iterator = deck.first;
        }
        else{
            iterator->next_element = new;
            iterator = iterator->next_element;
        }
        deck.last = new;
    }

    return deck;
} // Создание колоды
void create_zero_player(play *player){
    player->sum_points = 0;
    player->bank = 0;
    player->bet = 0;
    player->deck = (queue*)malloc(sizeof(queue));
    player->deck->len = 0;
    player->deck->first = NULL;
    player->deck->last = NULL;
} // Создание игрока

queue mix_deck(queue *deck){
    queue deck1;
    queue_element *iterator1 = deck->first;
    for (int i = 0; i < 25; i++){
        iterator1 = iterator1->next_element;
    }
    deck1.first = iterator1->next_element;

    iterator1->next_element = NULL;

    queue final_deck;

    queue_element *iterator2 = deck1.first;
    iterator1 = deck->first;
    final_deck.first = deck->first;
    for (int i = 0; i < 51; i++){
        if (i % 2 == 0){
            queue_element *a = iterator1->next_element;
            iterator1->next_element = iterator2;
            iterator1 = a;
        }
        if (i % 2 == 1){
            queue_element *a = iterator2->next_element;
            iterator2->next_element = iterator1;
            iterator2 = a;
        }
    }
    final_deck.last = iterator2;
    final_deck.len = 52;
    return final_deck;
} // Перемешивание колоды
void split_deck(queue *deck){
    queue_element *iterator = deck->first;
    for (int i = 0; i < 20; i++){
        iterator = iterator->next_element;
    }
    queue_element *a = deck->first;
    deck->first = iterator->next_element;
    deck->last = iterator;
    iterator->next_element = NULL;
    iterator = deck->first;
    for (int i = 0; i < 30; i++){
        iterator = iterator->next_element;
    }
    iterator->next_element = a;
} // Сплит колоды
void print_player_deck(queue *deck){
    //4 + (5 * player.deck->len)
    queue_element *iterator;
    for (int i = 0; i < 7; i++){
        iterator = deck->first;
        for (int j = 0; j < 5 + (4 * deck->len); j++){
            if ((i == 5 && j == 1) || (i == 1 && j % 4 == 1 && j != 1)){
                for (int k = 0; k < 3; k++){
                    if (iterator->suit[k] == 0 && i != 5 && j != 1){
                        printf(" ");
                    }
                    else if (i == 5 && j == 1 && k == 0);
                    else{
                        printf("%c", iterator->suit[k]);
                    }
                }
                j += 2;
                if (iterator->suit[0] == 0 && i == 5) printf(" ");
            }
            else if ((i == 0) && (j % 4 == 0 && j != 4)) printf(".");
            else if ((i != 0) && (j % 4 == 0 && j != 4)) printf("'");
            else if ((i == 0 || i == 6) && (j % 2 == 1)) printf("-");
            else if ((i == 0 || i == 6) && (j % 2 == 0) && ((j % 4 != 0 || j == 4))) printf(" ");
            else if (!((i != 0) && (j % 4 == 0 && j != 4))) printf(" ");
            if (i == 1 && j % 4 == 0 && j != 4 && j != 0) iterator = iterator->next_element;
        }
        printf("\n");
    }

}// Печать колоды
void print_diller_deck(queue *deck){
    //4 + (5 * player.deck->len)
    queue_element *iterator;
    for (int i = 0; i < 7; i++){
        iterator = deck->first;
        for (int j = 0; j < 5 + (4 * deck->len); j++){
            if ((i == 5 && j == 1) || (i == 1 && j % 4 == 1 && j != 1) && (j < 8)){
                for (int k = 0; k < 3; k++){
                    if (iterator->suit[k] == 0 && i != 5 && j != 1){
                        printf(" ");
                    }
                    else if (i == 5 && j == 1 && k == 0);
                    else{
                        printf("%c", iterator->suit[k]);
                    }
                }
                j += 2;
                if (iterator->suit[0] == 0 && i == 5) printf(" ");
            }
            else if ((i == 0) && (j % 4 == 0 && j != 4)) printf(".");
            else if ((i != 0) && (j % 4 == 0 && j != 4)) printf("'");
            else if ((i == 0 || i == 6) && (j % 2 == 1)) printf("-");
            else if ((i == 0 || i == 6) && (j % 2 == 0) && ((j % 4 != 0 || j == 4))) printf(" ");
            else if (!((i != 0) && (j % 4 == 0 && j != 4))) printf(" ");
            if (i == 1 && j % 4 == 0 && j != 4 && j != 0) iterator = iterator->next_element;
        }
        printf("\n");
    }

}// Печать колоды
play *play_normal(play *player, play *player1, play diller, queue *deck){
    while(1){
        int flag;
        // Вывод карт игрока
        printf("You cards: \n");
        print_player_deck(player->deck);
        printf("\nYou sum: %d", player->sum_points);

        // Для сплита
        if(player1 != NULL){
            printf("\n\nYou cards from second deck: \n");
            print_player_deck(player1->deck);
            printf("\nYou second deck sum: %d", player1->sum_points);
        }

        // Бонусные комбинации
        if (player->deck->len == 5){
            printf("\nBonus combo! +x3 bet!");
            player->bank += player->bet * 3;
        }
        if (player->sum_points == 21){
            queue_element *a = player->deck->first;
            queue_element *b = a->next_element;
            queue_element *c = b->next_element;
            if (a->points == 6 && b->points == 7 && c->points == 8){
                if (a->suit[1] == b->suit[1] && b->suit[1] == c->suit[1]){
                    printf("\nBonus combo! +x5 bet!");
                    player->bank += player->bet * 3;
                }
                else{
                    printf("\nBonus combo! +x3 bet!");
                    player->bank += player->bet * 3;
                }
            }
            if (a->points == 7 && b->points == 7 && c->points == 7){
                printf("\nBonus combo! +x3 bet!");
                player->bank += player->bet * 3;
            }
        }

        // Печать карт диллера
        printf("\n\n\nDiller cards: \n");
        print_diller_deck(diller.deck);

        // Взять ли ещё карту?
        printf("\n\nTake one more card?\n 1. Yes(1)\n 2. No(2)\n");
        scanf("%d", &flag);
        system("cls");

        // Если взять и сумма карт <= 21
        if (flag == 1 && player->sum_points < 22) {
            player->sum_points += deck->last->points;
            insert_element(player->deck, pop(deck), player->deck->len);

            if (player->sum_points < 22) continue;
            else return player;
        }
        else{
            return player;
        }
        system("cls");
    }
} // Обычная игра
void diller_take(play *diller, queue *deck){
    diller->sum_points += deck->last->points;
    insert_element(diller->deck, pop(deck), diller->deck->len);

    printf("Diller cards: \n");
    print_player_deck(diller->deck);
    printf("\nDiller sum: %d", diller->sum_points);
    sleep(3);
    system("cls");

} // Диллер берёт карты
play *play_split(play *player, play *player1, play diller, queue *deck){
    player1 = (play*)malloc(sizeof(play));
    player1->deck = (queue*)malloc(sizeof(queue));
    player1->sum_points = player->deck->last->points;
    player->sum_points -= player->deck->last->points;
    player1->bet = player->bet;
    player1->deck->len = 1;
    player->deck->len--;
    player1->deck->first = player->deck->last;
    player1->deck->last = player->deck->last;
    player->deck->last = player->deck->first;
    player->deck->first->next_element = NULL;
    system("cls");
    player = play_normal(player, player1, diller, deck);
    player1 = play_normal(player1, player, diller, deck);
    return player1;
} // Игра сплит
void victory_condition(play *player, play *player1, play diller){
    for (int i = 0; i < 2; i++) {
        if (player1 == NULL) i++;
        if ((player->sum_points > diller.sum_points || diller.sum_points > 21) && player->sum_points < 22) {
            printf("You win!\nYou sum = %d\nDiller sum = %d\n", player->sum_points, diller.sum_points);
            player->bank += player->bet;
        } else if (player->sum_points == diller.sum_points && player->sum_points < 22) {
            printf("Draw!\nYou sum = %d\nDiller sum = %d\n", player->sum_points, diller.sum_points);
        } else {
            printf("You Lose!\nYou sum = %d\nDiller sum = %d\n", player->sum_points, diller.sum_points);
            player->bank -= player->bet;
        }
        if (player1 != NULL) player->sum_points = player1->sum_points;
        else i++;
    }
} // Условия победы

int main() {
    srand(time(NULL));

    // Начало игры
    int flag;
    printf("Start new game?\n1. Yes (1)\n2. No (2)\n\n");
    scanf("%d", &flag);
    system("cls");

    // Если нет
    if (flag == 2) {
        return 0;
    }

    // Создаём игрока
    play player;
    create_zero_player(&player);

    // Создаём колоду
    queue deck = create_deck(deck);

    // Введите банк
    printf("Enter you bank: ");
    scanf("%d", &player.bank);
    system("cls");

    while (player.bank > 0) {

        // Перемешивание колоды
        for (int i = 0; i < rand_int(100, 200); i++) {
            split_deck(&deck);
            deck = mix_deck(&deck);
        }

        // Введите ставку
        printf("You bank: %d\n", player.bank);
        printf("Enter you bet: ");
        scanf("%d", &player.bet);
        if (player.bet > player.bank || player.bet < 1) {
            printf("You bet is too high or too low!\n");
            continue;
        }
        system("cls");

        // Создание колоды диллера
        play diller;
        create_zero_player(&diller);
        player.deck->len = 0;
        player.deck->first = NULL;
        player.deck->last = NULL;

        // Раздача первых двух карт игроку и диллеру
        for (int i = 0; i < 2; i++) {
            player.sum_points += deck.last->points;
            insert_element(player.deck, pop(&deck), player.deck->len);

            diller.sum_points += deck.last->points;
            insert_element(diller.deck, pop(&deck), diller.deck->len);
        }

        // Брать ли карты игроку?
        while (1) {

            // Вывод карт игрока
            printf("You cards: \n");
            print_player_deck(player.deck);
            printf("\nYou sum: %d", player.sum_points);

            // Печать карт диллера
            printf("\n\n\nDiller cards: \n");
            print_diller_deck(diller.deck);

            // Выбор режима игры
            printf("Play split?\n 1. Play normal(1)\n 2. Play split(2)\n");
            scanf("%d", &flag);
            if (flag == 1){
                system("cls");
                player = *play_normal(&player, NULL, diller, &deck);
            }
            play *player1 = NULL;
            if (flag == 2 && player.bank >= player.bet * 2){
                player1 = play_split(&player, player1, diller, &deck);
            }
            else if (flag == 2 && player.bank < player.bet * 2){
                system("cls");
                printf("Not enought money! Game was started in normal mode\n");
                player = *play_normal(&player, NULL, diller, &deck);
            }

            // Диллер берёт карты
            while ((diller.sum_points <= 11 || diller.sum_points < player.sum_points) && player.sum_points < 22){
                diller_take(&diller, &deck);
            }

            // Условия победы, ничьи и проигрыша
            victory_condition(&player, player1, diller);

            // Возвращение карт в колоду и обнуление очков
            int a = player.deck->len, b = diller.deck->len;
            if (player1 != NULL){
                int c = player1->deck->len;
                for (int i = 0; i < c; i++)insert_element(&deck, pop(player1->deck), deck.len);
            }
            for (int i = 0; i < a; i++) insert_element(&deck, pop(player.deck), deck.len);
            for (int i = 0; i < b; i++) insert_element(&deck, pop(diller.deck), deck.len);
            player.sum_points = 0;
            diller.sum_points = 0;

            // Сыграть снова?
            printf("Play again?\n 1. Yes(1)\n 2. No(2)\n");
            scanf("%d", &flag);
            system("cls");
            if (flag == 1) {
                break;
            } else return 0;

        }
    }
    return 0;
}