
/****************************************************************************
  Copyright (c) 2008-2020 Flexera Software LLC.
  All Rights Reserved.
  This software has been provided pursuant to a License Agreement
  containing restrictions on its use.  This software contains
  valuable trade secrets and proprietary information of
  Flexera Software LLC and is protected by law.
  It may not be copied or distributed in any form or medium, disclosed
  to third parties, reverse engineered or used in any manner not
  provided for in said License Agreement except with the prior
  written authorization from Flexera Software LLC.
*****************************************************************************/
#ifdef _MSC_VER

#pragma warning( push, 3 )
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <shlobj.h>
#pragma warning( pop )

#endif


#include "FileIO.h"
#include "ErrDisplay.h"


/*-------------------------------------------------------------------------*/
/*                                                                         */
/*      GetStoragePath                                                     */
/*                                                                         */
/*      Obtains the storage path in which to place regenerative (served),  */
/*      trial or short code license information.                           */
/*                                                                         */
/*      @param pathName    supplied area to returned the path name         */
/*      @param pathLength  length of the supplied area                     */
/*                                                                         */
/*      @return FLC_TRUE if successful;                                    */
/*                 FLC_FALSE otherwise                                     */
/*-------------------------------------------------------------------------*/
FlcBool
GetStoragePath(
    FlcChar *   pathName,
    FlcSize     pathLength)
/*=========================================================================*/
{
#if defined(_MSC_VER)

    wchar_t folder[MAX_PATH] = {0};
    LPITEMIDLIST pidl = NULL;

    HRESULT hr = SHGetSpecialFolderLocation( NULL, CSIDL_PERSONAL, &pidl );
    if(SUCCEEDED(hr))
    {
        (void)SHGetPathFromIDListW(pidl, folder);
        CoTaskMemFree(pidl);
    }

    if (!WideCharToMultiByte(CP_UTF8, 0, folder, -1, pathName, pathLength, 0, 0))
    {
        return FLC_FALSE;
    }

#else

    char*    envp;
    if (( envp = getenv("HOME")) != NULL )
    {
        strncpy(pathName, envp, pathLength);
    }
    else
    {
        return FLC_FALSE;
    }

#endif

    return FLC_TRUE;
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/*      ReadFileData                                                       */
/*                                                                         */
/*      Allocates memory to hold the contents of the file, reads the       */
/*      file into the provided buffer                                      */
/*                                                                         */
/*      @param fileName   filename relative to where the program is run    */
/*      @param buffer     pointer to file buffer                           */
/*      @param bufferSize size of allocated buffer to hold file            */
/*      @param error      if there was an error allocating memory to hold  */
/*                        the file contents, the error information is      */
/*                        returned here                                    */
/*                                                                         */
/*      @return FLC_TRUE if successfully read the entire file;             */
/*                 FLC_FALSE otherwise                                     */
/*-------------------------------------------------------------------------*/
FlcBool
ReadFileData(
    const char *   fileName,
    FlcUInt8 **     buffer,
    FlcSize *       bufferSize)
/*=========================================================================*/
{
    FILE *  file        = 0;

    if ( !fileName  ||  !buffer  ||  !bufferSize )
    {
        DisplayErrorString("ReadFileData: bad arguments", 0);
        return FLC_FALSE;
    }

    file = fopen(fileName, "rb");
    if (!file)
    {
        sprintf(msg, "ReadFileData: opening %s for reading", fileName);
        DisplayErrorString(msg, 0);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* obtain file size                                                    */
    /*---------------------------------------------------------------------*/
    fseek (file , 0 , SEEK_END);
    *bufferSize = ftell (file);
    fseek (file , 0 , SEEK_SET);

    /*---------------------------------------------------------------------*/
    /* make sure there's data                                              */
    /*---------------------------------------------------------------------*/
    if (!(*bufferSize))
    {
        sprintf(msg, "ReadFileData: %s contains no data", fileName);
        DisplayErrorString(msg, 0);
        return FLC_FALSE;
    }

    *buffer = (FlcUInt8 *) malloc(sizeof(FlcInt8)*(*bufferSize));
    if (!(*buffer))
    {
        DisplayErrorString("ReadFileData: allocating buffer for reading", 0);
        fclose(file);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* read file into the buffer                                           */
    /*---------------------------------------------------------------------*/
    if ( fread(*buffer, sizeof(FlcInt8), *bufferSize, file) != *bufferSize )
    {
        sprintf(msg, "ReadFileData: reading data from %s", fileName);
        DisplayErrorString(msg, 0);
        free(*buffer);
        *buffer = 0;
        fclose(file);
        return  FLC_FALSE;
    }

    fclose(file);
    return FLC_TRUE;
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/*      ReadFileText                                                       */
/*                                                                         */
/*      Allocates memory to hold the contents of the text file, reads      */
/*      the file content as a character string                             */
/*                                                                         */
/*      @param fileName   filename relative to where the program is run    */
/*      @param text       pointer to returned character string             */
/*      @param error      if there was an error allocating memory to hold  */
/*                        the file contents, the error information is      */
/*                        returned here                                    */
/*                                                                         */
/*      @return FLC_TRUE if successfully read the entire file;             */
/*                 FLC_FALSE otherwise                                     */
/*-------------------------------------------------------------------------*/
FlcBool
ReadFileText(
    const char *   fileName,
    FlcChar **      text)
/*=========================================================================*/
{
    FILE *  file  = 0;
    FlcSize size  = 0;

    if (!fileName || !text)
    {
        DisplayErrorString("ReadTextFile: bad arguments", 0);
        return FLC_FALSE;
    }

    file = fopen(fileName, "r");

    if (!file)
    {
        sprintf(msg, "ReadTextFile: opening %s for reading", fileName);
        DisplayErrorString(msg, 0);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* obtain file size                                                    */
    /*---------------------------------------------------------------------*/
    fseek (file , 0 , SEEK_END);
    size = ftell(file);
    fseek (file , 0 , SEEK_SET);

    /*---------------------------------------------------------------------*/
    /* make sure there's data                                              */
    /*---------------------------------------------------------------------*/
    if (!size)
    {
        sprintf(msg, "ReadTextFile: %s contains no data", fileName);
        DisplayErrorString(msg, 0);
        return FLC_FALSE;
    }

    *text = (FlcChar *) malloc(size + sizeof(FlcChar));
    if (!(*text))
    {
        DisplayErrorString("ReadTextFile: allocating buffer for reading", 0);
        fclose(file);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* read file into the buffer                                           */
    /*---------------------------------------------------------------------*/
    if (fread(*text, sizeof(FlcChar), size, file) != size)
    {
        sprintf(msg, "reading data from %s", fileName);
        DisplayErrorString(msg, 0);
        free(*text);
        *text = 0;
        fclose(file);
        return  FLC_FALSE;
    }

    fclose(file);
    (*text)[size] = 0;
    return FLC_TRUE;
}


/*-------------------------------------------------------------------------*/
/*                                                                         */
/*      WriteFileData                                                      */
/*                                                                         */
/*      @param fileName     filename relative to where the program is run  */
/*      @param buffer       pointer to buffer to write to file             */
/*      @param bufferSize   number of bytes to write to the file starting  */
/*                          at the beginning of the buffer                 */
/*      @return FLC_TRUE if successfully read the entire file;             */
/*                 FLC_FALSE otherwise                                     */
/*-------------------------------------------------------------------------*/
FlcBool
WriteFileData(
    const char *       fileName,
    const FlcUInt8 *    buffer,
    FlcSize             bufferSize)
/*=========================================================================*/
{
    FILE *  file        = 0;
    char   msg[512]    = {0};

    if (!fileName || !buffer)
    {
        DisplayErrorString("WriteFileData: bad arguments", 0);
        return FLC_FALSE;
    }

    file = fopen(fileName, "w+b");
    if (!file)
    {
        sprintf(msg, "WriteFileData: opening %s for writing", fileName);
        DisplayErrorString(msg, 0);
        return FLC_FALSE;
    }

    if (fwrite(buffer, sizeof(FlcUInt8), bufferSize, file) != bufferSize)
    {
        sprintf(msg, "WriteFileData: writing data to %s", fileName);
        DisplayErrorString(msg, 0);
        fclose(file);
        return FLC_FALSE;
    }

    fclose(file);
    return FLC_TRUE;
}

