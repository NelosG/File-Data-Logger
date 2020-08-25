int UnicodeToUTF8(char *res, const uint64_t unicode) {
    if (unicode <= 0x7F) {
        res[0] = unicode;
        return 1;
    } else if (unicode <= 0x7FF) {
        res[0] = 0xC0 | (unicode >> 6);
        res[1] = 0x80 | (unicode & 0x3F);
        return 2;
    } else if (unicode >= 0xDC80 && unicode <= 0xDCFF) {
        res[0] = unicode - 0xDC00;
        return 1;
    } else if (unicode <= 0xFFFF) {
        res[0] = 0xE0 | (unicode >> 12);
        res[1] = 0x80 | ((unicode >> 6) & 0x3F);
        res[2] = 0x80 | (unicode & 0x3F);
        return 3;
    } else if (unicode <= 0x10FFFF) {
        res[0] = 0xF0 | (unicode >> 18);
        res[1] = 0x80 | ((unicode >> 12) & 0x3F);
        res[2] = 0x80 | ((unicode >> 6) & 0x3F);
        res[3] = 0x80 | (unicode & 0x3F);
        return 4;
    } else {
        res[0] = 0xE0 | 0xF;
        res[1] = 0x80 | 0x3F;
        res[2] = 0x80 | 0x3D;
        return 3;
    }
}

std::string cast(const std::wstring& ws) {
    std::string s;
    char *res = (char*)malloc(sizeof(char) * 5);
    for(auto wc : ws) {
        int temp = UnicodeToUTF8(res, wc);
        res[temp] = '\0';
        s+=res;
    }
    free(res);
    return s;
}
