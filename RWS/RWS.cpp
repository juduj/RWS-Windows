#include <iostream>
#include <Windows.h>
#include <string>
#include <algorithm>

std::string GetClipboardText();
bool SetClipboardText(std::string clipBoardText);
std::string RemoveWhiteSpace(std::string clipboardText);

int main() {

    std::string clipboardText = GetClipboardText();
    SetClipboardText(RemoveWhiteSpace(clipboardText));
    return 0;
}


std::string GetClipboardText() {
    if (!OpenClipboard(nullptr)) {
        std::cerr << "Failed to open clipboard" << std::endl;
        return "";
    }

    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == nullptr) {
        std::cerr << "Failed to get clipboard data" << std::endl;
        CloseClipboard();
        return "";
    }

    char* pszText = static_cast<char*>(GlobalLock(hData));
    if (pszText == nullptr) {
        std::cerr << "Failed to lock clipboard data" << std::endl;
        CloseClipboard();
        return "";
    }

    std::string text(pszText);

    GlobalUnlock(hData);
    CloseClipboard();

    return text;
}

bool SetClipboardText(std::string clipBoardText)
{
    if (!OpenClipboard(nullptr))
    {
        std::cerr << "ERROR OPENING CLIPBOARD" << std::endl;
        return false;
    }

    //just a nice little flush
    EmptyClipboard();

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, strlen(clipBoardText.c_str()) + 1);
    if (hMem == nullptr)
    {
        std::cerr << "Failed to allocate memory for the clipboard!" << std::endl;
        CloseClipboard();
        return false;
    }


    char* pMem = static_cast<char*>(GlobalLock(hMem));

    strcpy_s(pMem, strlen(clipBoardText.c_str()) + 1, clipBoardText.c_str());

    GlobalUnlock(hMem);

    if(!SetClipboardData(CF_TEXT, hMem)) {
        std::cerr << "Failed to set clipboard data!" << std::endl;
        CloseClipboard();
        return false;
    }

    CloseClipboard();

    std::cout << "Copy succesful!" << std::endl;

    return true;
}

std::string RemoveWhiteSpace(std::string clipboardText)
{
    if (clipboardText.empty())
    {
        std::cerr << "Clipboard was empty!" << std::endl;
        return "EMPTY STRING";
    }

    std::string::iterator end_pos = std::remove(clipboardText.begin(), clipboardText.end(), ' ');
    clipboardText.erase(end_pos, clipboardText.end());

    return clipboardText;
}
