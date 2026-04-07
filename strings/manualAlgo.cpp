#include <cstdio>
#include <cstring>

int mStrlen(const char* str) {
    if (!str) return 0;
    int len = 0;
    while (str[len] != '\0') {
        ++len;
    }
    return len;
}

char* mStrcpy(char* dest, const char* src) {
    if (!dest || !src) return nullptr;
    char* p = dest;
    while ((*p++ = *src++) != '\0');
    return dest;
}

int mStrcmp(const char* str1, const char* str2) {
    while (!str1 && !str2 && *str1++ == *str2++);
    return *str1 - *str2;
}

char* mStrstr(const char* str, const char* sub) {
    if (!*sub) return (char*)str;
    while (*str) {
        const char* s = str;
        const char* b = sub;
        while (*s && * b && *s++ == *b++);

        if (!*b) return (char*)str;
        ++str;
    }
    return nullptr;
}




int main(int argc, char const *argv[])
{
    const char* testStr = "I love algorithm";
    printf("mStrlen: %d, strlen: %d\n", mStrlen(testStr), strlen(testStr));
    char test1[32] = {0};
    strcpy(test1, testStr);
    printf("strcpy: %s, ", test1);
    memset(test1, 0, sizeof(test1));
    mStrcpy(test1, testStr);
    printf("mStrcpy: %s\n", test1);
    printf("strcmp: %d, mStrcmp: %d\n", strcmp(testStr, test1), mStrcmp(testStr, test1));
    printf("strstr: %s, mStrstr: %s\n", strstr(testStr, test1), mStrstr(testStr, test1));
    return 0;
}
