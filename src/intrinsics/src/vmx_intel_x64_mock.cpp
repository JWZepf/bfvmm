//
// Bareflank Hypervisor
//
// Copyright (C) 2015 Assured Information Security, Inc.
// Author: Rian Quinn        <quinnr@ainfosec.com>
// Author: Brendan Kerrigan  <kerriganb@ainfosec.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#include <bfgsl.h>
#include <bfdebug.h>

#include <intrinsics/vmx_intel_x64.h>

extern "C" bool
__vmxon(void *ptr) noexcept
{
    (void) ptr;

    std::cerr << __FUNC__ << " called" << '\n';
    abort();
}

extern "C" bool
__vmxoff(void) noexcept
{
    std::cerr << __FUNC__ << " called" << '\n';
    abort();
}

extern "C" bool
__vmclear(void *ptr) noexcept
{
    (void) ptr;

    std::cerr << __FUNC__ << " called" << '\n';
    abort();
}

extern "C" bool
__vmptrld(void *ptr) noexcept
{
    (void) ptr;

    std::cerr << __FUNC__ << " called" << '\n';
    abort();
}

extern "C" bool
__vmptrst(void *ptr) noexcept
{
    (void) ptr;

    std::cerr << __FUNC__ << " called" << '\n';
    abort();
}

extern "C" bool
__vmread(uint64_t field, const uint64_t *value) noexcept
{
    (void) value;

    std::cerr << __FUNC__ << " called with: " << '\n';
    std::cerr << "    - field: " << view_as_pointer(field) << '\n';
    abort();
}

extern "C" bool
__vmwrite(uint64_t field, uint64_t value) noexcept
{
    std::cerr << __FUNC__ << " called with: " << '\n';
    std::cerr << "    - field: " << view_as_pointer(field) << '\n';
    std::cerr << "    - value: " << view_as_pointer(value) << '\n';
    abort();
}

extern "C" bool
__vmlaunch_demote(void) noexcept
{
    std::cerr << __FUNC__ << " called" << '\n';
    abort();
}

extern "C" bool
__invept(uint64_t type, void *ptr) noexcept
{
    (void) type;
    (void) ptr;

    std::cerr << __FUNC__ << " called" << '\n';
    abort();
}

extern "C" bool
__invvpid(uint64_t type, void *ptr) noexcept
{
    (void) type;
    (void) ptr;

    std::cerr << __FUNC__ << " called" << '\n';
    abort();
}
