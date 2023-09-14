// perelocator.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include "windows.h"
#include "stdlib.h"
#include "stdio.h"

void* RelocatePE(char* prm_0,BOOL prm_1,UINT64 prm_2) {
    char buff0[4096];
    char* buff4pe;
    char* buff4pecl;
    char* dll4prevdll = (char*)"::/\\";
    HANDLE fh;
    UINT64 baseaddr = 0;
    UINT64 cnt = 0;
    UINT64 cnt2 = 0;
    UINT64 reloc = 0;
    UINT64 relocsize = 0;
    UINT64 textaddr = 0;
    UINT64 textaddrsize = 0;
    DWORD tmp;
    UINT64 PEHeaderaddr = 0;
    if (fh = CreateFileA(prm_0, GENERIC_ALL, 3, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0)) {
        ReadFile(fh, buff0, 512, &tmp, 0);
        PEHeaderaddr = (*(UINT32*)(&buff0[0x3c]));
        SetFilePointer(fh, (*(UINT32*)(&buff0[0x3c])), 0, 0);
        ReadFile(fh, buff0, 4096, &tmp, 0);
        SetFilePointer(fh, 0, 0, 0);
        //buff4pe = (char*)malloc((*(UINT32*)(&buff0[0x1c])) + (*(UINT32*)(&buff0[0x20])) + (*(UINT32*)(&buff0[0x24])));
        buff4pe = (char*)malloc((*(UINT32*)(&buff0[0x50])) + 4096);
        memcpy(buff4pe, buff0, 4096);
        if ((*(UINT16*)(&buff0[0x18])) == 0x10b) {
            //32bit
            baseaddr = (*(UINT32*)(&buff0[0x34]));
            cnt = 0;
            while (cnt < (*(UINT16*)(&buff0[0x6]))) {
                SetFilePointer(fh, (*(UINT32*)(&buff0[0xf8 + (0x28 * cnt) + 0x14])), 0, 0);
                ReadFile(fh, (void*)((UINT64)(buff4pe)+(*(UINT32*)(&buff0[0xf8 + (0x28 * cnt) + 0xc]))), (*(UINT32*)(&buff0[0xf8 + (0x28 * cnt) + 0x10])), &tmp, 0);
                if (strcmp(((char*)(&buff0[0xf8 + (0x28 * cnt)])), ".reloc") == 0) {
                    reloc = ((UINT64)(buff4pe)+(*(UINT32*)(&buff0[0xf8 + (0x28 * cnt) + 0xc]))); relocsize = (*(UINT32*)(&buff0[0xf8 + (0x28 * cnt) + 0x8]));
                }
                if (strcmp(((char*)(&buff0[0xf8 + (0x28 * cnt)])), ".text") == 0) {
                    textaddr = ((UINT64)(buff4pe)+(*(UINT32*)(&buff0[0xf8 + (0x28 * cnt) + 0xc]))); textaddrsize = (*(UINT32*)(&buff0[0xf8 + (0x28 * cnt) + 0x8]));
                }
                cnt++;
            }
        }
        else if ((*(UINT16*)(&buff0[0x18])) == 0x20b) {
            //64bit
            baseaddr = (*(UINT64*)(&buff0[0x30]));
            cnt = 0;
            while (cnt < (*(UINT16*)(&buff0[0x6]))) {
                SetFilePointer(fh, (*(UINT32*)(&buff0[0x108 + (0x28 * cnt) + 0x14])), 0, 0);
                ReadFile(fh, (void*)((UINT64)(buff4pe)+(*(UINT32*)(&buff0[0x108 + (0x28 * cnt) + 0xc]))), (*(UINT32*)(&buff0[0x108 + (0x28 * cnt) + 0x10])), &tmp, 0);
                if (strcmp(((char*)(&buff0[0x108 + (0x28 * cnt)])), ".reloc") == 0) {
                    reloc = (UINT64)(buff4pe)+(*(UINT32*)(&buff0[0x108 + (0x28 * cnt) + 0xc])); relocsize = (*(UINT32*)(&buff0[0x108 + (0x28 * cnt) + 0x8]));
                }
                if (strcmp(((char*)(&buff0[0x108 + (0x28 * cnt)])), ".text") == 0) {
                    textaddr = (UINT64)(buff4pe)+(*(UINT32*)(&buff0[0x108 + (0x28 * cnt) + 0xc])); textaddrsize = (*(UINT32*)(&buff0[0x108 + (0x28 * cnt) + 0x8]));
                }
                cnt++;
            }
        }
        UINT64 delta = ((UINT64)((prm_2 ? prm_2 : (UINT64)(buff4pe)) - baseaddr));
        UINT64 tmp4relocptx = 8;
        UINT32 armhi = 0;
        UINT32 armlo = 0;
        UINT32 armhi_ = 0;
        UINT32 armlo_ = 0;
        UINT64 deltatmp;
        HMODULE HM = 0;
        if (reloc == 0) { CloseHandle(fh); free(buff4pe); return 0; }

    loop4relocate:
        cnt = 0;
        for (cnt = 0; cnt < (((*(UINT32*)(reloc + (tmp4relocptx - 4))) - 8) / 2); cnt++) {
            switch (((*(UINT16*)(reloc + (tmp4relocptx)+(cnt * 2))) >> 12) & 0xF) {
            case 1:
                (*(UINT32*)(buff4pe + (((*(UINT16*)(reloc + (tmp4relocptx)+(cnt * 2))) & 0xFFF) + (*(UINT32*)(reloc + (tmp4relocptx - 8)))))) = (*(UINT32*)(buff4pe + (((*(UINT16*)(reloc + (tmp4relocptx)+(cnt * 2))) & 0xFFF) + (*(UINT32*)(reloc + (tmp4relocptx - 8)))))) + (UINT32)((delta >> 16) & 0xFFFF);
                break;
            case 2:
                (*(UINT32*)(buff4pe + (((*(UINT16*)(reloc + (tmp4relocptx)+(cnt * 2))) & 0xFFF) + (*(UINT32*)(reloc + (tmp4relocptx - 8)))))) = (*(UINT32*)(buff4pe + (((*(UINT16*)(reloc + (tmp4relocptx)+(cnt * 2))) & 0xFFF) + (*(UINT32*)(reloc + (tmp4relocptx - 8)))))) + (UINT32)((delta >> 0) & 0xFFFF);
                break;
            case 3:
                (*(UINT32*)(buff4pe + (((*(UINT16*)(reloc + (tmp4relocptx)+(cnt * 2))) & 0xFFF) + (*(UINT32*)(reloc + (tmp4relocptx - 8)))))) = (*(UINT32*)(buff4pe + (((*(UINT16*)(reloc + (tmp4relocptx)+(cnt * 2))) & 0xFFF) + (*(UINT32*)(reloc + (tmp4relocptx - 8)))))) + (UINT32)delta;
                break;
            case 7:
                armlo = (*(UINT32*)(buff4pe + (((*(UINT16*)(reloc + (tmp4relocptx)+(cnt * 2))) & 0xFFF) + (*(UINT32*)(reloc + (tmp4relocptx - 8))))));
                armlo_ = ((armlo << 1) & 0x0800) + ((armlo << 12) & 0xf000) + ((armlo >> 20) & 0x0700) + ((armlo >> 16) & 0x00ff);
                armhi = (*(UINT32*)(buff4pe + (((*(UINT16*)(reloc + (tmp4relocptx)+(cnt * 2))) & 0xFFF) + (*(UINT32*)(reloc + (tmp4relocptx - 8)))) + 4));
                armhi_ = ((armhi << 1) & 0x0800) + ((armhi << 12) & 0xf000) + ((armhi >> 20) & 0x0700) + ((armhi >> 16) & 0x00ff);
                deltatmp = (((armlo_ & 0xFFFF) << 0) | ((armhi_ & 0xFFFF) << 16)) + delta;
                armlo_ = (deltatmp >> 0) & 0xFFFF;
                armhi_ = (deltatmp >> 16) & 0xFFFF;
                armlo = (armlo & 0x8f00fbf0) + ((armlo_ >> 1) & 0x0400) + ((armlo_ >> 12) & 0x000f) + ((armlo_ << 20) & 0x70000000) + ((armlo_ << 16) & 0xff0000);
                armhi = (armhi & 0x8f00fbf0) + ((armhi_ >> 1) & 0x0400) + ((armhi_ >> 12) & 0x000f) + ((armhi_ << 20) & 0x70000000) + ((armhi_ << 16) & 0xff0000);
                (*(UINT32*)(buff4pe + (((*(UINT16*)(reloc + (tmp4relocptx)+(cnt * 2))) & 0xFFF) + (*(UINT32*)(reloc + (tmp4relocptx - 8)))))) = armlo;
                (*(UINT32*)(buff4pe + (((*(UINT16*)(reloc + (tmp4relocptx)+(cnt * 2))) & 0xFFF) + (*(UINT32*)(reloc + (tmp4relocptx - 8)))) + 4)) = armhi;
                break;
            case 10:
                (*(UINT64*)(buff4pe + (((*(UINT16*)(reloc + (tmp4relocptx)+(cnt * 2))) & 0xFFF) + (*(UINT32*)(reloc + (tmp4relocptx - 8)))) + 0)) = (*(UINT64*)(buff4pe + (((*(UINT16*)(reloc + (tmp4relocptx)+(cnt * 2))) & 0xFFF) + (*(UINT32*)(reloc + (tmp4relocptx - 8)))) + 0)) + delta;
                break;
            }
            //cnt++;
        }
        tmp4relocptx += (*(UINT32*)(reloc + (tmp4relocptx - 4)));
        if ((*(UINT32*)(reloc + (tmp4relocptx - 4))) != 0)
            goto loop4relocate;
        cnt = 0;
        //MessageBoxA(0, "A", "B", 0);
        /*if ((*(UINT16*)(&buff0[0x18])) == 0x10b) {
            //32bit
            while ((*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x80])) + (cnt * 20))) != 0) {
                cnt2 = 0;
                if ((*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x80])) + (cnt * 20) + 12)) != 0) {
                    HM = LoadLibraryA(((char*)(buff4pe + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x80])) + (cnt * 20) + 12)))));
                }
                if (HM != 0) {
                    while ((*(UINT32*)(buff4pe + (cnt2 * 4) + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x80])) + (cnt * 20) + 16)))) != 0) {//&& (*(UINT32*)(buff4pe + (cnt2 * 4) + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x80])) + (cnt * 20) + 0)))) != 0) {
                        // + ((*(UINT16*)(buff4pe + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x80])) + (cnt * 20) + 4)))) * 4)
                        if ((*(UINT32*)(buff4pe + (cnt2 * 4) + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x80])) + (cnt * 20) + 0)))) & 0x80000000) {
                            (*(UINT32*)(buff4pe + (cnt2 * 4) + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x80])) + (cnt * 20) + 16)))) = (UINT32)GetProcAddress(HM, ((LPCSTR)((*(UINT32*)(buff4pe + (cnt2 * 4) + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x80])) + (cnt * 20) + 0)))) & 0xFFFF)));
                        }
                        else {
                            (*(UINT32*)(buff4pe + (cnt2 * 4) + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x80])) + (cnt * 20) + 16)))) = (UINT32)GetProcAddress(HM, ((char*)(buff4pe + 2 + (*(UINT32*)(buff4pe + (cnt2 * 4) + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x80])) + (cnt * 20) + 0)))))));
                        }
                        cnt2++;
                    }
                    //(*(UINT32*)(buff4pe + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x80])) + (cnt * 20) + 0)))) = 0 | 0x80000000;
                }
                cnt++;
            }
        }
        else if ((*(UINT16*)(&buff0[0x18])) == 0x20b) {
            //64bit
            while ((*(UINT64*)(buff4pe + (*(UINT32*)(&buff0[0x90])) + (cnt * 20))) != 0) {
                cnt2 = 0;
                if ((*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x90])) + (cnt * 20) + 12)) != 0) {
                    HM = LoadLibraryA(((char*)(buff4pe + (*(UINT64*)(buff4pe + (*(UINT32*)(&buff0[0x90])) + (cnt * 20) + 12)))));
                }
                if (HM != 0) {
                    while ((*(UINT32*)(buff4pe + (cnt2 * 8) + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x90])) + (cnt * 20) + 16)))) != 0 && (*(UINT32*)(buff4pe + (cnt2 * 4) + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x90])) + (cnt * 20) + 0)))) != 0) {
                        // + ((*(UINT16*)(buff4pe + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x90])) + (cnt * 20) + 4)))) * 8)
                        if ((*(UINT64*)(buff4pe + (cnt2 * 4) + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x90])) + (cnt * 20) + 0)))) & 0x8000000000000000) {
                            (*(UINT64*)(buff4pe + (cnt2 * 4) + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x90])) + (cnt * 20) + 16)))) = (UINT64)GetProcAddress(HM, ((LPCSTR)((*(UINT32*)(buff4pe + (cnt2 * 4) + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x90])) + (cnt * 20) + 0)))) & 0xFFFF)));
                        }
                        else {
                            (*(UINT64*)(buff4pe + (cnt2 * 8) + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x90])) + (cnt * 20) + 16)))) = (UINT64)GetProcAddress(HM, ((char*)(buff4pe + 2 + (*(UINT32*)(buff4pe + (cnt2 * 4) + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x90])) + (cnt * 20) + 0)))))));
                        }
                        cnt2++;
                    }
                    //(*(UINT64*)(buff4pe + (*(UINT32*)(buff4pe + (*(UINT32*)(&buff0[0x90])) + (cnt * 20) + 0)))) = 0 | 0x8000000000000000;
                }
                cnt++;
            }
        }
        if (textaddr != 0) {
            VirtualProtect((void*)(textaddr), textaddrsize, PAGE_EXECUTE_READWRITE, &tmp);
            FlushInstructionCache(GetCurrentProcess(), (void*)(textaddr), textaddrsize);
        }*/
        if (prm_1 == true) {
            if ((*(UINT16*)(&buff0[0x18])) == 0x10b) {
                //32bit
                baseaddr = prm_2;//(*(UINT32*)(&buff0[0x34]));
                cnt = 0;
                while (cnt < (*(UINT16*)(&buff0[0x6]))) {
                    SetFilePointer(fh, (*(UINT32*)(&buff0[0xf8 + (0x28 * cnt) + 0x14])), 0, 0);
                    WriteFile(fh, (void*)((UINT64)(buff4pe)+(*(UINT32*)(&buff0[0xf8 + (0x28 * cnt) + 0xc]))), (*(UINT32*)(&buff0[0xf8 + (0x28 * cnt) + 0x10])), &tmp, 0);
                    cnt++;
                }
                SetFilePointer(fh, PEHeaderaddr + 0x34, 0, 0);
                WriteFile(fh, &baseaddr, 4, &tmp, 0);
            }
            else if ((*(UINT16*)(&buff0[0x18])) == 0x20b) {
                //64bit
                baseaddr = prm_2;//(*(UINT64*)(&buff0[0x30]));
                cnt = 0;
                while (cnt < (*(UINT16*)(&buff0[0x6]))) {
                    SetFilePointer(fh, (*(UINT32*)(&buff0[0x108 + (0x28 * cnt) + 0x14])), 0, 0);
                    WriteFile(fh, (void*)((UINT64)(buff4pe)+(*(UINT32*)(&buff0[0x108 + (0x28 * cnt) + 0xc]))), (*(UINT32*)(&buff0[0x108 + (0x28 * cnt) + 0x10])), &tmp, 0);
                    cnt++;
                }
                SetFilePointer(fh, PEHeaderaddr+0x30, 0, 0);
                WriteFile(fh, &baseaddr, 8, &tmp, 0);
            }

        }
        CloseHandle(fh);
        return buff4pe;
    }
    return 0;
}

INT8 hexchar2UINT4[] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,-1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };

UINT64 GetUINT64fromHEXchar(char* prm_0) {
    UINT64 result = 0;
    for (int cnt = 0; cnt < strlen(prm_0); cnt++) { result |= (((UINT64)hexchar2UINT4[prm_0[cnt]]&0xF)<<(((strlen(prm_0)-1)-cnt)*4)); }
    return result;
}

int main(int argc,char* argv[])
{
    UINT64 baseaddr = GetUINT64fromHEXchar(argv[1]);
    RelocatePE(argv[2],1,baseaddr);
    return 0;
    //std::cout << "Hello World!\n";
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
