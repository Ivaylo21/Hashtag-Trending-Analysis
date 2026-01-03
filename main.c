#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_HASHTAGS 100
#define MAX_TAG_LEN 50
#define LINE_LEN 256

// Структура за хаштаг и неговата честота
typedef struct
{
    char tag[MAX_TAG_LEN];
    int count;
} Hashtag;

// Масив за съхранение на всички хаштагове
Hashtag hashtags[MAX_HASHTAGS];
int hashtag_count = 0;

// Функция за преобразуване на низ в малки букви
void to_lowercase(char *s)
{
    for (int i = 0; s[i]; i++)
    {
        s[i] = tolower(s[i]);
    }
}

// Проверява дали хаштагът вече съществува
// Ако съществува – увеличава броя
// Ако не – добавя нов хаштаг
void add_hashtag(char *tag)
{
    to_lowercase(tag);

    for (int i = 0; i < hashtag_count; i++)
    {
        if (strcmp(hashtags[i].tag, tag) == 0)
        {
            hashtags[i].count++;
            return;
        }
    }

    // Добавяне на нов хаштаг
    if (hashtag_count < MAX_HASHTAGS)
    {
        strcpy(hashtags[hashtag_count].tag, tag);
        hashtags[hashtag_count].count = 1;
        hashtag_count++;
    }
}

// Извлича хаштагове от един ред текст
void extract_hashtags(char *line)
{
    char *token = strtok(line, " \n");

    while (token != NULL)
    {
        if (token[0] == '#')
        {
            // Премахване на пунктуация в края
            char clean_tag[MAX_TAG_LEN];
            int j = 0;

            for (int i = 1; token[i] && j < MAX_TAG_LEN - 1; i++)
            {
                if (isalnum(token[i]))
                {
                    clean_tag[j++] = token[i];
                }
            }
            clean_tag[j] = '\0';

            if (strlen(clean_tag) > 0)
            {
                add_hashtag(clean_tag);
            }
        }

        token = strtok(NULL, " \n");
    }
}

// Сортира хаштаговете по честота (низходящо)
void sort_hashtags()
{
    for (int i = 0; i < hashtag_count - 1; i++)
    {
        for (int j = i + 1; j < hashtag_count; j++)
        {
            if (hashtags[j].count > hashtags[i].count)
            {
                Hashtag temp = hashtags[i];
                hashtags[i] = hashtags[j];
                hashtags[j] = temp;
            }
        }
    }
}

// Отпечатва Top N трендващи хаштага
void print_trending(int top_n)
{
    printf("\nTrending Hashtags:\n");
    printf("------------------\n");

    for (int i = 0; i < top_n && i < hashtag_count; i++)
    {
        printf("#%s - %d posts\n", hashtags[i].tag, hashtags[i].count);
    }
}

int main(void)
{
    FILE *file = fopen("posts.txt", "r");
    if (file == NULL)
    {
        printf("Error: Could not open file.\n");
        return 1;
    }

    char line[LINE_LEN];

    // Четене на файла ред по ред
    while (fgets(line, LINE_LEN, file))
    {
        extract_hashtags(line);
    }

    fclose(file);

    sort_hashtags();

    // Показване на Top 5 хаштага
    print_trending(5);

    return 0;
}
