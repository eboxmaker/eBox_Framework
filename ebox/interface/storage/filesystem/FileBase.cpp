/* ebox Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include <cstring>

#include "FileBase.h"
#include "FileLike.h"
#include "FileHandle.h"

namespace ebox {

FileBase *FileBase::_head = NULL;
FileBase *FileBase::_default = NULL;

FileBase::FileBase(const char *name, PathType t) : _next(NULL),
    _name(name),
    _path_type(t)
{
    if (name != NULL) {
        // put this object at head of the list
        _next = _head;
        _head = this;
    } else {
        _next = NULL;
    }
}

FileBase::~FileBase()
{
    if (_name != NULL) {
        // remove this object from the list
        if (_head == this) { // first in the list, so just drop me
            _head = _next;
        } else {             // find the object before me, then drop me
            FileBase *p = _head;
            while (p->_next != this) {
                p = p->_next;
            }
            p->_next = _next;
        }
    }

    if (_default == this) {
        _default = NULL;
    }


    if (getPathType() == FilePathType) {
//        extern void remove_filehandle(FileHandle * file);
//        remove_filehandle(static_cast<FileLike *>(this));
    }
}

void FileBase::set_as_default()
{
    _default = this;
}

FileBase *FileBase::lookup(const char *name, unsigned int len)
{
    FileBase *p = _head;
    while (p != NULL) {
        /* Check that p->_name matches name and is the correct length */
        if (p->_name != NULL && len == std::strlen(p->_name) && std::memcmp(p->_name, name, len) == 0) {
            return p;
        }
        p = p->_next;
    }
    if (len == (sizeof "default") - 1 && std::memcmp("default", name, len) == 0) {
        return _default;
    }
    return NULL;
}

FileBase *FileBase::get(int n)
{
    FileBase *p = _head;
    int m = 0;
    while (p != NULL) {
        if (m == n) {
            return p;
        }

        m++;
        p = p->_next;
    }
    return NULL;
}

const char *FileBase::getName(void)
{
    // Constant read so no lock needed
    return _name;
}

PathType FileBase::getPathType(void)
{
    // Constant read so no lock needed
    return _path_type;
}

} // namespace ebox
