/*
 * A library for BG96 into ESP32
 * This file is about the BG96 FILE operation manual
 * 
 * Copyright (c) Avi Energy Lab.
 * @Author       : Ariel Rios
 * @Create time  : 04/25/2021
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _LTEBG96FILE_H_
#define _LTEBG96FILE_H_

#include "LTEBG96TCPIP.h"

typedef enum open_file_mode{
    CREATE_OR_OPEN = 0,
    CREATE_OR_CLEAR = 1,
    ONLY_READ = 2,
}Open_File_Mode_t;

typedef enum pointer_mode{
    FILE_BEGINNING = 0,
    FILE_CURRENT = 1,
    FILE_ENDING = 2,
}Pointer_Mode_t;

class LTEBG96FILE : public LTEBG96TCPIP
{
  public:

    LTEBG96FILE();

    ~LTEBG96FILE();

    LTEBG96FILE(Stream &atserial, Stream &dserial);

    bool GetFliesSpace(unsigned long &free_bytes, unsigned long &total_bytes);

    bool GetFilesList(char *list);

    bool DeleteFiles(char *filename);

    bool UploadFiles(char *filename, char *u_file);

    bool DownloadFiles(char *filename, char *d_file);

    bool OpenFile(char *filename, Open_File_Mode_t mode, unsigned int &file_index);

    bool ReadFile(unsigned int file_index, char *read_data);

    bool WriteFile(unsigned int file_index, char *write_data);

    bool CloseFlie(unsigned int file_index);

    bool SetFilePointer(unsigned int file_index, unsigned int offset, Pointer_Mode_t p_mode);

    bool GetFilePointer(unsigned int file_index, unsigned int &offset);

    bool TruncateFile(unsigned int file_index);

  private:

};

#endif