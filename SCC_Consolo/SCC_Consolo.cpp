#define NOMINMAX // 禁用 windows.h 中的 max 和 min 宏#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <windows.h>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <locale>
#include <codecvt>
#include <random>
#include <ctime>
#include <chrono>//美化用件
#include <thread>//美化用件
#include <iostream>

using namespace std;


//美化界面
void simulateTyping(const std::wstring& text, int delayMillis = 100);

wstringstream rtn;
wstring unLock;
int pr = 0;

//加一些有趣的美观功能【？】
void beginConsoloImg() {
    srand(time(0));
    int randInt = rand() % 5;
    wstring printHead;
    switch (randInt)
    {
    case 0:
        printHead = LR"( ____    ____     ____      
/\  _`\ /\  _`\  /\  _`\    
\ \,\L\_\ \ \/\_\\ \ \/\_\  
 \/_\__ \\ \ \/_/_\ \ \/_/_ 
   /\ \L\ \ \ \L\ \\ \ \L\ \
   \ `\____\ \____/ \ \____/
    \/_____/\/___/   \/___/ 
                            
                            )";
        break;
    case 1:
        printHead = LR"(    ___       ___       ___   
   /\  \     /\  \     /\  \  
  /::\  \   /::\  \   /::\  \ 
 /\:\:\__\ /:/\:\__\ /:/\:\__\
 \:\:\/__/ \:\ \/__/ \:\ \/__/
  \::/  /   \:\__\    \:\__\  
   \/__/     \/__/     \/__/  )";
        break;
    case 2:
        printHead = LR"( (                  
 )\ )   (      (    
(()/(   )\     )\   
 /(_))(((_)  (((_)  
(_))  )\___  )\___  
/ __|((/ __|((/ __| 
\__ \ | (__  | (__  
|___/  \___|  \___| 
                    )";
        break;
    case 3:
        printHead = LR"(  __________________ _________  
 /   _____/\_   ___ \\_   ___ \ 
 \_____  \ /    \  \//    \  \/ 
 /        \\     \___\     \____
/_______  / \______  /\______  /
        \/         \/        \/ )";
        break;
    case 4:
        printHead = LR"( ____ ____ ____ 
||S |||C |||C ||
||__|||__|||__||
|/__\|/__\|/__\|)";
        break;
    default:
        simulateTyping(L"很抱歉的告诉你，这个程序出了非常严重的问题，请等待我们修复");
        Sleep(200);
        simulateTyping(L"什么啊，原来是美化出错了，不管了不管了");
        wcout << L"下次一定修好吧【请联系CVKNO80098@outlook.com】进行上报，您将会出现在鸣谢名单中";
        break;
    }
    wcout << printHead << endl;
}
//宽字符逐个打字
void simulateTyping(const std::wstring& text, int delayMillis) {
    for (wchar_t c : text) {
        std::wcout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis));
    }

    std::wcout << std::endl; // 在函数结束后输出换行符
}

//获取当前时间
wstring getCurrentTime() {
    time_t currentTime;
    time(&currentTime);
    tm localTime;
    localtime_s(&localTime, &currentTime);

    wchar_t formattedTime[100];
    wcsftime(formattedTime, sizeof(formattedTime) / sizeof(wchar_t), L"%Y-%m-%d %H:%M:%S", &localTime);
    return wstring(formattedTime);
}


//图像加密所用函数
void encryptData(vector<unsigned char>& data, const string& password) {
    // 使用密码哈希加盐
    string salt = "mysalt123";  // 自定义盐值
    string saltedPassword = password + salt;

    // 使用哈希函数生成密钥
    hash<string> hasher;
    size_t key = hasher(saltedPassword);

    // 异或加密数据
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = data[i] ^ (key & 0xFF);
        key >>= 1;  // 右移一位
    }
}

void decryptData(vector<unsigned char>& data, const string& password) {
    // 解密与加密相同，因为异或操作是可逆的
    encryptData(data, password);
}

//图像加密
string LockPic(const string& password, vector<unsigned char>& imageData, size_t fileSize) {
    encryptData(imageData, password);

    // 保存加密后的图像数据和原始文件大小到新文件
    string encryptedImagePath = "encryptedimage";
    ofstream encryptedFile(encryptedImagePath, ios::binary);
    encryptedFile.write(reinterpret_cast<char*>(imageData.data()), imageData.size());

    // 保存原始文件大小
    encryptedFile.write(reinterpret_cast<const char*>(&fileSize), sizeof(fileSize));

    cout << "已将生成的加密图像 " << encryptedImagePath << " 保存在当前文件夹" << endl;
    return encryptedImagePath;
}

//图像解密
string UnLockPic(const string& password, vector<unsigned char>& imageData, const string& encryptedImagePath) {
    // 获取加密后的文件大小
    size_t encryptedFileSize = imageData.size();

    // 读取原始文件大小
    size_t originalFileSize;
    ifstream encryptedFile(encryptedImagePath, ios::binary | ios::ate);
    if (encryptedFile) {
        encryptedFile.seekg(-static_cast<streamoff>(sizeof(originalFileSize)), ios::end);
        encryptedFile.read(reinterpret_cast<char*>(&originalFileSize), sizeof(originalFileSize));
        encryptedFile.seekg(0, ios::beg);
    }
    else {
        cout << "无法打开加密文件" << endl;
        return "无法打开文件，程序结束";
    }

    // 解密图像数据
    decryptData(imageData, password);

    // 保存解密后的图像数据到新文件
    string decryptedImagePath = "decrypted_image.jpg";
    ofstream decryptedFile(decryptedImagePath, ios::binary);
    decryptedFile.write(reinterpret_cast<char*>(imageData.data()), originalFileSize);
    return decryptedImagePath;
}

//文字加密：SCC主功能：
wstring stringToWstring(const string& str) {
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}
//截取字符串的
wstring wsubstr(const wchar_t* str, int i, int j) {
    wstring sub;
    sub.assign(str + i, str + i + j);
    return sub;
}
//生成随机字符数字
wstring generateRandomString(int length) {
    wstring characters = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(0, characters.length() - 1);

    wstringstream ss;

    for (int i = 0; i < length; ++i) {
        int randomIndex = distribution(generator);
        ss << characters[randomIndex];
    }

    return ss.str();
}
//使用hash作为头混淆,注意！不可逆推！初始值将遗失！缩短到100以内
size_t hashPassword(const wstring& password) {
    //转化哈希值
    hash<wstring> hasher;
    size_t hashValue = hasher(password);

    // 将哈希值缩短到 [0, 100] 范围内的整数
    int scaledValue = hashValue % 101;  // 取余数，范围是 [0, 100]
    return scaledValue;
}
//日常字符串用hash，缩短到10以内
size_t hashPasschar(const wchar_t* passChar) {
    hash<wstring> hashPlay;
    size_t hashValue = hashPlay(passChar);

    //缩短到 [0, 10] 范围内的整数
    int scaledValue = hashValue % 11;
    return scaledValue;
}
//去掉逗号
wstring removeComma(const wstring& str) {
    wstringstream result;
    size_t pos = str.find(L",");
    size_t start = 0;
    while (pos != wstring::npos) {
        result << str.substr(start, pos - start);
        start = pos + 1;
        pos = str.find(L",", start);
    }
    result << str.substr(start);
    return result.str();
}
//初步加密
wstring convertToHex(const wchar_t* wideString) {
    wstringstream ss;
    wstringstream feedback;

    for (int i = 0; wideString[i] != L'\0'; i++) {
        ss << wideString[i] << L":";
        if (iswalpha(wideString[i]) || iswdigit(wideString[i])) {
            ss << L"0x" << setw(4) << setfill(L'0') << hex << (int)wideString[i] << L"\n";
            feedback << setw(4) << setfill(L'0') << hex << (int)wideString[i];
        }
        else {
            ss << L"0x" << setw(4) << setfill(L'0') << hex << (int)wideString[i] << endl;
            feedback << setw(4) << setfill(L'0') << hex << (int)wideString[i];
        }
    }
    return removeComma(feedback.str());
}
//第一步解密：去掉头
wstring deleteHead(const wchar_t* wideString, const wstring& password) {
    wstringstream backWord;

    //传入密码：
    size_t headLength = hashPassword(password);

    wstring backSubword = wstring(&wideString[headLength]);
    return backSubword;
}
//第二步解密：解剖
wstring deleteOtherchar(const wchar_t* wideString, const wstring& password) {
    wstring result;
    size_t i = 0;
    size_t j = 0;
    while (i < wcslen(wideString)) {
        result += wideString[i];
        i += hashPasschar(&password[j]) + 1;
        j = (j + 1) % password.length();
    }
    return result;
}
//最终解密
wstring convertFromHex(const wchar_t* wideString) {
    wstringstream ss; // 创建一个字符串流对象，用于存储转换后的宽字符

    for (int i = 0; wideString[i] != L'\0'; i += 4) { // 遍历宽字符数组中的每个字符，每次递增4，以处理一组四个字符
        wstring hexSubstring = wstring(&wideString[i], &wideString[i + 4]); // 从宽字符数组中获取当前四个字符，存储在宽 字符串 hexSubstring 中

        int hexValue;
        wstringstream(hexSubstring) >> hex >> hexValue; // 将字符串转换为整数，存储在 hexValue 中，得到一个十六进制的整 数值

        wchar_t wideChar = static_cast<wchar_t>(hexValue); // 将 hexValue 转换为宽字符
        ss << wideChar; // 将宽字符追加到字符串流 ss 中

    }

    return ss.str(); // 将字符串流 ss 中的内容转换为宽字符串，并作为函数的返回值
}
//二次加密,函数加密，把输出转为A+b
wstring goToLocktwo(const wchar_t* wideString, const wchar_t* passWord) {
    wstring mergedString;
    size_t str1Length = wcslen(wideString);
    size_t str2Length = wcslen(passWord);

    size_t maxLength = str1Length; // 将 str1Length 的值保存在 maxLength 变量中。

    for (size_t i = 0; i < maxLength; i++) {
        mergedString += wideString[i]; // 将wideString的第i个字符添加到mergedString中。
        if (str2Length > 0) // 检查str2Length是否大于0。
        {
            mergedString += generateRandomString(hashPasschar(&passWord[i % str2Length]));// 将passWord的第(i % str2Length)个字符添加到mergedString中。
        }
    }
    return mergedString; // 返回mergedString作为函数的结果。
}
//加密程序EFI
int LockSub() {
    wstring userInput;
    wcout << L"请输入一个宽字符串：";
    wcin >> userInput;

    //如果密码为空
    if (userInput.empty()) {
        wcout << L"您输入的字符为空，自动退出程序" << endl;
    }

    wstring passWordsub;
    wcout << L"请输入密码（区分大小写）：";
    wcin >> passWordsub;

    if (passWordsub.empty()) {
        wcerr << L"喂喂喂，这程序不能没有密码吧，都加密了" << endl;
    }

    wstring head;
    head = generateRandomString(hashPassword(passWordsub));

    wstring result = convertToHex(userInput.c_str());

    result = goToLocktwo(result.c_str(), passWordsub.c_str());

    wstringstream rtn;
    rtn << head << result;
    wcout << rtn.str() << endl << endl;//加密并输出
    return 0;
}
//解密程序EFI
int UnLockSub() {
    //解密：
    wstring userInput;
    wcout << L"请输入您要解码的字符串：";
    wcin >> userInput;

    wstring passWordsub;
    wcout << L"请输入密码（区分大小写）：";
    wcin >> passWordsub;

    wstring unLock = convertFromHex(deleteOtherchar(deleteHead(userInput.c_str(), passWordsub).c_str(), passWordsub).c_str());

    wcout << L"您输入的字符是：" << unLock << endl;
    return 0;
}
//文件写入temp
void createTempFile(const string& filePath, const wstring& content) {
    wofstream file(filePath);
    if (file.is_open()) {
        file << content;
        file.close();
        wcout << L"文件 '" << wstring(filePath.begin(), filePath.end()) << L"' 创建成功，并成功写入字符串。" << endl;
    }
    else {
        wcout << L"无法创建文件。" << endl;
    }
}

int main() {
    setlocale(LC_ALL, "");
    // 设置输入流（stdin）和输出流（stdout）的字符编码为 GBK
    SetConsoleCP(936);
    SetConsoleOutputCP(936);

    // 多了一行，但是我不敢动它
    std::locale::global(std::locale(""));

    //美化标题
    beginConsoloImg();

    //函数变量初始化（部分在函数内部）
    string words;
    string password = "123";
    string address = "NULL";
    int choice;

chooseMode:
    wcout << L"[ * ]    请选择您所启用的模块:\n1、文字加密\n2、文字解密\n3、图像加密\n4、图像解密\n";
    wcin >> choice;
    switch (choice)
    {
    case 1:
        LockSub(); break;
    case 2:
        UnLockSub(); break;
    default:
        if (choice == 3 or choice == 4)
        {
            cout << "请输入您要进行操作文件路径:";
            string imagePath;  // 图像路径
            cin >> imagePath;

            cout << "请输入密码" << endl;
            string password;
            cin >> password;

            // 读取图像文件
            ifstream inputFile(imagePath, ios::binary);
            if (!inputFile) {
                wcout << L"无法打开图像文件" << endl;
                return 1;
            }

            // 获取文件大小
            inputFile.seekg(0, ios::end);//光标移到最后
            size_t fileSize = inputFile.tellg();//获取长度
            inputFile.seekg(0, ios::beg);//光标回到最开始

            // 读取图像数据
            vector<unsigned char> imageData(fileSize);
            inputFile.read(reinterpret_cast<char*>(imageData.data()), fileSize);

            string result;
            if (choice ==3)
            {
                result = LockPic(password, imageData, fileSize);
            }
            else{
            if (choice ==4){
                result = UnLockPic(password, imageData, imagePath);}
            else{
                wcout << L"您选择的模块不存在，请重新选择..." << endl;
                goto chooseMode;
            }}
            wcout << result.c_str() << endl;
        }
    }
    wcout << L"按任意键关闭此窗口. . .";
    getchar(); // 等待用户按下一个键
    cin.clear(); // 清除输入缓冲区中的错误标志
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清除输入缓冲区中的剩余字符
    return 0;

}

