#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LEN 100  // 字串最大長度
#define BITS_PER_LETTER 2  // 每個字母的位元數（2位）
#define ASCII_SIZE 128  // ASCII 查找表大小

// 查找表，用於將字母對應到位元編碼
unsigned long long lookupTable[ASCII_SIZE];

// 初始化查找表
void initLookupTable() {
    for (int i = 0; i < ASCII_SIZE; i++) {
        lookupTable[i] = -1;  // 初始化查找表中所有位置為 -1，表示尚未分配
    }
}

// 編碼 DNA 字串，並自動分配新的字母位元
void encodeDNA(const char* str, unsigned long long* nextBits) {
    unsigned long long result = 0;  // 用來儲存最終的編碼結果

    for (int i = 0; str[i] != '\0'; i++) {
        unsigned char letter = (unsigned char)str[i];  // 讀取當前字母
        unsigned long long bits = lookupTable[letter];  // 查找該字母的位元編碼

        if (bits == -1) {
            // 如果該字母尚未編碼，為它分配新位元
            bits = (*nextBits)++;  // 分配下一個可用的位元編碼
            lookupTable[letter] = bits;  // 更新查找表，記錄新字母的位元編碼
        }

        //// 將結果左移 BITS_PER_LETTER 位，然後加入該字母的位元編碼
        //result = (result << BITS_PER_LETTER) | bits;
    }

}

int main() {
    initLookupTable();  // 初始化查找表
    unsigned long long nextBits = 0;  // 下一個可用的位元編碼從 0 開始

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
    encodeDNA(T, &nextBits);  // 編碼 DNA 字串
    clock_t end3_time = clock();

    double ttest = (double)(end3_time - start3_time) / CLOCKS_PER_SEC;

    // 輸出查找表中的字母及其對應的位元編碼
    printf("查找表內容：\n");
    for (int i = 0; i < ASCII_SIZE; i++) {
        if (lookupTable[i] != -1) {
            printf("字母: %c, 位元編碼: %llu\n", i, lookupTable[i]);
        }
    }
    printf("time：%f\n", ttest);
    return 0;
}
