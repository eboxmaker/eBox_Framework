/* ebox Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "FileSystemHandle.h"

namespace ebox {
int FileSystemHandle::open(FileHandle **file, const char *filename, int flags)
{
    return -ENOSYS;
}

int FileSystemHandle::open(ebox::DirHandle **dir, const char *path)
{
    return -ENOSYS;
}

int FileSystemHandle::remove(const char *path)
{
    return -ENOSYS;
}

int FileSystemHandle::rename(const char *path, const char *newpath)
{
    return -ENOSYS;
}

int FileSystemHandle::stat(const char *path, struct stat *st)
{
    return -ENOSYS;
}

int FileSystemHandle::mkdir(const char *path, mode_t mode)
{
    return -ENOSYS;
}

int FileSystemHandle::statvfs(const char *path, struct statvfs *buf)
{
    return -ENOSYS;
}

} // namespace ebox
