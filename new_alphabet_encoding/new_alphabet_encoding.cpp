#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LEN 256  // 字串最大長度
#define INIT_CAP 4   // 初始哈希表容量
#define BITS_PER_LETTER 2  // 每個字母的位元數（2位）

// 節點，用來存儲字母和它們對應的位元值
typedef struct {
    char letter;
    unsigned long long bits;
} Entry;

// 動態哈希表結構
typedef struct {
    Entry* entries;
    int size;
    int capacity;
} HashTable;

// 初始化哈希表
void initHashTable(HashTable* table) {
    table->entries = (Entry*)malloc(INIT_CAP * sizeof(Entry));
    table->size = 0;
    table->capacity = INIT_CAP;
}

// 擴充哈希表容量
void expandHashTable(HashTable* table) {
    table->capacity *= 2;
    table->entries = (Entry*)realloc(table->entries, table->capacity * sizeof(Entry));
}

// 在哈希表中查找字母，若存在則返回對應的位元編碼
unsigned long long findBits(HashTable* table, char letter) {
    for (int i = 0; i < table->size; i++) {
        if (table->entries[i].letter == letter) {
            return table->entries[i].bits;
        }
    }
    return -1;  // 如果未找到，返回 -1
}

// 在哈希表中插入新字母
void insertLetter(HashTable* table, char letter, unsigned long long bits) {
    if (table->size == table->capacity) {
        expandHashTable(table);
    }
    table->entries[table->size].letter = letter;
    table->entries[table->size].bits = bits;
    table->size++;
}

// 編碼 DNA 字串
unsigned long long encodeDNA(const char* str, HashTable* table) {
    unsigned long long result = 0;
    unsigned long long bitLength = 0;
    unsigned long long nextBits = 0;  // 下一個可用的位元編碼

    for (int i = 0; str[i] != '\0'; i++) {
        unsigned long long bits = findBits(table, str[i]);
        if (bits == -1) {
            // 如果字母未在表中，插入新字母並分配新位元
            bits = nextBits;
            insertLetter(table, str[i], bits);
            nextBits++;
        }

        // 將結果左移2位，然後加入新字母的位元
        result = (result << BITS_PER_LETTER) | bits;
        bitLength += BITS_PER_LETTER;
    }

    return result;
}

int main() {
    HashTable table;
    initHashTable(&table);

    char dna[MAX_LEN];

    // 檔案名稱
    const char* filename1 = "Bible_A63_4M.txt";
    FILE* infile1;
    errno_t err = fopen_s(&infile1, filename1, "r");  // 使用 fopen_s

    if (err != 0) {
        fprintf(stderr, "無法打開檔案 %s\n", filename1);
        return 1;
    }

    // 移動到檔案的結尾，獲取檔案大小
    fseek(infile1, 0, SEEK_END);
    long filesize = ftell(infile1);  // 獲取檔案大小
    fseek(infile1, 0, SEEK_SET);     // 返回到檔案的開頭

    // 分配內存來保存檔案內容
    char* T = (char*)malloc(filesize + 1);  // +1 是為了放置字串結尾符 '\0'
    if (T == NULL) {
        fprintf(stderr, "記憶體分配失敗\n");
        fclose(infile1);
        return 1;
    }

    // 讀取檔案內容到 T 陣列中
    fread(T, 1, filesize, infile1);
    T[filesize] = '\0';  // 確保以空字元結尾

    // 關閉檔案
    fclose(infile1);

    clock_t start3_time = clock();
    unsigned long long encoded = encodeDNA(T, &table);

    clock_t end3_time = clock();
    printf("編碼結果：%llu\n", encoded);

    double ttest = (double)(end3_time - start3_time) / CLOCKS_PER_SEC;


    // 輸出哈希表中的字母及其對應的位元編碼
    printf("哈希表內容：\n");
    for (int i = 0; i < table.size; i++) {
        printf("字母: %c, 位元編碼: %llu\n", table.entries[i].letter, table.entries[i].bits);
    }
    printf("time：%f", ttest);
    // 釋放記憶體
    free(table.entries);
    return 0;
}
