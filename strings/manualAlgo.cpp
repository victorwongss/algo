#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>

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
    if (str1 == nullptr && str2 == nullptr) {
        return 0;
    } else if (str1 == nullptr) {
        return -1;
    } else if (str2 == nullptr) {
        return 1;
    }

    while ((*str1 || *str2) && *str1 == *str2) ++str1, ++str2;
    return *str1 - *str2;
}

char* mStrstr(const char* str, const char* sub) {
    if (str == nullptr || sub == nullptr) return nullptr;
    if (!*sub) return (char*)str;
    while (*str) {
        const char* str_ = str;
        const char* sub_ = sub;
        while (*str_ && * sub_ && *str_ == *sub_) ++str_, ++sub_;

        if (!*sub_) return (char*)str;
        ++str;
    }
    return nullptr;
}

// ===========-------------------- C++ section
void reverseStr(std::string& str) {
    int l = 0, r = str.length() - 1;
    while (l < r) {
        std::swap(str[l++], str[r--]);
    }
}

bool isPalindrome(const std::string& str) {
    int l = 0, r = str.length() - 1;
    while (l < r) {
        if (str[l++] != str[r--]) return false;
    }
    return true;
}

std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> ret;
    int len = str.length(), l = 0;
    for (int r = 0; r < len; ++r) {
        if (str[r] == delim) {
            if (l < r) ret.emplace_back(str.substr(l, r - l));
            l = r + 1;
        }
    }
    if (l < len) ret.emplace_back(str.substr(l));
    return ret;
}

std::vector<std::string> split_ss(const std::string& str, char delim) {
    std::vector<std::string> ret;
    std::string token;
    std::istringstream iss(str);
    while (getline(iss, token, delim)) ret.emplace_back(token);
    return ret;
}

std::string trim(std::string str) {
    if (str.empty()) return str;
    str.erase(0, str.find_first_of(' '));
    str.erase(str.find_last_of(' ') + 1);
    return str;
}

void toLower(std::string& str) {
    for (char& c: str) {
        if (c >= 'A' && c <= 'Z') {
            c+= 32;
        }
    }
}

void toUper(std::string& str) {
    for (char& c: str) {
        if (c >= 'a' && c <= 'z') {
            c -= 32;
        }
    }
}

int mStoi(const std::string& str) {
    int len = str.length(), i = 0, res = 0, sign = 1;
    // 去除头部空格
    while(i < len && str[i] == ' ') ++i;
    // 解析符号
    if (i < len && (str[i] == '+' || str[i] == '-')) {
        sign = str[i++] == '-' ? -1 : 1;
    }

    while (i < len && std::isdigit(str[i])) {
        // 处理溢出
        if (res > INT32_MAX / 10 ||
            (res == INT32_MAX / 10 && str[i] - '0' > 7)) {
            return sign == 1 ? INT32_MAX : INT32_MIN;
        }
        res = res * 10 + str[i++] - '0';
    }
    return res * sign;
}

std::string mItoa(const int i) {
    if (0 == i) return "0";

    bool neg = i < 0;
    std::string res;
    int tmp = std::abs(i);
    while (tmp) {
        res.insert(res.begin(), tmp % 10 + '0');
        tmp /= 10;
    }
    if (neg) res.insert(res.begin(), '-');
    return res;
}

int main(int argc, char const *argv[])
{
    const char* testStr = " I love algorithm ";
    printf("mStrlen: %d, strlen: %d\n", mStrlen(testStr), strlen(testStr));
    char test1[32] = {0};
    strcpy(test1, testStr);
    printf("strcpy: %s, ", test1);
    memset(test1, 0, sizeof(test1));
    mStrcpy(test1, testStr);
    printf("mStrcpy: %s\n", test1);
    printf("strcmp: %d, mStrcmp: %d\n", strcmp(testStr, test1), mStrcmp(testStr, test1));
    printf("strstr: %s, mStrstr: %s\n", strstr(testStr, test1), mStrstr(testStr, test1));


    std::string testStrpp = testStr;
    reverseStr(testStrpp);
    printf("reverseStr: %s\n", testStrpp.c_str());
    std::string palindrome = "imatestsetami!";
    printf("isPalindrome: %d\n", isPalindrome(palindrome));
    printf("split: ");
    for (auto &e : split(testStrpp, ' ')){
        printf("%s, ", e.c_str());
    }
    printf("\n");

    printf("split_ss: ");
    for (auto &e : split_ss(testStrpp, ' ')){
        printf("%s, ", e.c_str());
    }
    printf("\n");

    printf("trim: %s\n", trim(testStrpp).c_str());
    toLower(testStrpp);
    printf("toLower: %s\n", testStrpp.c_str());
    toUper(testStrpp);
    printf("toUper: %s\n", testStrpp.c_str());

    printf("mstoi: %d\n", mStoi("  -532168565124578"));

    printf("mItoa: %s\n", mItoa(-4519051).c_str());
    return 0;
}
