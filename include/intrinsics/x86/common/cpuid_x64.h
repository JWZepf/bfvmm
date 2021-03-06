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

#ifndef CPUID_X64_H
#define CPUID_X64_H

#include <bfdebug.h>
#include <bfbitmanip.h>

// -----------------------------------------------------------------------------
// Exports
// -----------------------------------------------------------------------------

#include <bfexports.h>

#ifndef STATIC_INTRINSICS
#ifdef SHARED_INTRINSICS
#define EXPORT_INTRINSICS EXPORT_SYM
#else
#define EXPORT_INTRINSICS IMPORT_SYM
#endif
#else
#define EXPORT_INTRINSICS
#endif

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------

extern "C" EXPORT_INTRINSICS uint32_t _cpuid_eax(uint32_t val) noexcept;
extern "C" EXPORT_INTRINSICS uint32_t _cpuid_ebx(uint32_t val) noexcept;
extern "C" EXPORT_INTRINSICS uint32_t _cpuid_ecx(uint32_t val) noexcept;
extern "C" EXPORT_INTRINSICS uint32_t _cpuid_edx(uint32_t val) noexcept;
extern "C" EXPORT_INTRINSICS uint32_t _cpuid_subeax(uint32_t val, uint32_t sub) noexcept;
extern "C" EXPORT_INTRINSICS uint32_t _cpuid_subebx(uint32_t val, uint32_t sub) noexcept;
extern "C" EXPORT_INTRINSICS uint32_t _cpuid_subecx(uint32_t val, uint32_t sub) noexcept;
extern "C" EXPORT_INTRINSICS uint32_t _cpuid_subedx(uint32_t val, uint32_t sub) noexcept;
extern "C" EXPORT_INTRINSICS void _cpuid(void *eax, void *ebx, void *ecx, void *edx) noexcept;

// *INDENT-OFF*

namespace x64
{
namespace cpuid
{

using field_type = uint32_t;
using value_type = uint32_t;

inline auto get(field_type eax, field_type ebx, field_type ecx, field_type edx) noexcept
{
    _cpuid(&eax, &ebx, &ecx, &edx);
    return std::make_tuple(eax, ebx, ecx, edx);
}

namespace eax
{
    inline auto get(field_type eax) noexcept
    { return _cpuid_eax(eax); }
}

namespace ebx
{
    inline auto get(field_type ebx) noexcept
    { return _cpuid_ebx(ebx); }
}

namespace ecx
{
    inline auto get(field_type ecx) noexcept
    { return _cpuid_ecx(ecx); }
}

namespace edx
{
    inline auto get(field_type edx) noexcept
    { return _cpuid_edx(edx); }
}

namespace addr_size
{
    constexpr const auto addr = 0x80000008ULL;
    constexpr const auto name = "addr_size";

    inline auto get() noexcept
    { return _cpuid_eax(addr); }

    namespace phys
    {
        constexpr const auto mask = 0x000000FFULL;
        constexpr const auto from = 0;
        constexpr const auto name = "phys";

        inline auto get() noexcept
        { return get_bits(_cpuid_eax(addr), mask) >> from; }

        inline auto get(value_type msr) noexcept
        { return get_bits(msr, mask) >> from; }

        inline void dump(int level)
        { bfdebug_subnhex(level, name, get()); }
    }

    namespace linear
    {
        constexpr const auto mask = 0x0000FF00ULL;
        constexpr const auto from = 8;
        constexpr const auto name = "linear";

        inline auto get() noexcept
        { return get_bits(_cpuid_eax(addr), mask) >> from; }

        inline auto get(value_type msr) noexcept
        { return get_bits(msr, mask) >> from; }

        inline void dump(int level)
        { bfdebug_subnhex(level, name, get()); }
    }

    inline void dump(int level)
    {
        bfdebug_nhex(level, name, get());
        phys::dump(level);
        linear::dump(level);
    }
}

namespace basic_cpuid_info
{
    constexpr const auto addr = 0x00000000ULL;

    namespace eax
    {
        constexpr const auto name = "basic_cpuid_info_eax";

        inline auto get() noexcept
        { return _cpuid_eax(addr); }

        inline void dump(int level)
        { bfdebug_nhex(level, name, get()); }
    }

    inline void dump(int level)
    {
        eax::dump(level);
    }
}

namespace extend_cpuid_info
{
    constexpr const auto addr = 0x80000000ULL;

    namespace eax
    {
        constexpr const auto name = "extend_cpuid_info_eax";

        inline auto get() noexcept
        { return _cpuid_eax(addr); }

        inline void dump(int level)
        { bfdebug_nhex(level, name, get()); }
    }

    inline void dump(int level)
    {
        eax::dump(level);
    }
}

namespace processor_string_1
{
    constexpr const auto addr = 0x80000002ULL;

    namespace eax
    {
        constexpr const auto name = "processor_string_1_eax";

        inline auto get() noexcept
        { return _cpuid_eax(addr); }

        inline void dump(int level)
        { bfdebug_nhex(level, name, get()); }
    }

    namespace ebx
    {
        constexpr const auto name = "processor_string_1_ebx";

        inline auto get() noexcept
        { return _cpuid_ebx(addr); }

        inline void dump(int level)
        { bfdebug_nhex(level, name, get()); }
    }

    namespace ecx
    {
        constexpr const auto name = "processor_string_1_ecx";

        inline auto get() noexcept
        { return _cpuid_ecx(addr); }

        inline void dump(int level)
        { bfdebug_nhex(level, name, get()); }
    }

    namespace edx
    {
        constexpr const auto name = "processor_string_1_edx";

        inline auto get() noexcept
        { return _cpuid_edx(addr); }

        inline void dump(int level)
        { bfdebug_nhex(level, name, get()); }
    }

    inline void dump(int level)
    {
        eax::dump(level);
        ebx::dump(level);
        ecx::dump(level);
        edx::dump(level);
    }
}

namespace processor_string_2
{
    constexpr const auto addr = 0x80000003ULL;

    namespace eax
    {
        constexpr const auto name = "processor_string_2_eax";

        inline auto get() noexcept
        { return _cpuid_eax(addr); }

        inline void dump(int level)
        { bfdebug_nhex(level, name, get()); }
    }

    namespace ebx
    {
        constexpr const auto name = "processor_string_2_ebx";

        inline auto get() noexcept
        { return _cpuid_ebx(addr); }

        inline void dump(int level)
        { bfdebug_nhex(level, name, get()); }
    }

    namespace ecx
    {
        constexpr const auto name = "processor_string_2_ecx";

        inline auto get() noexcept
        { return _cpuid_ecx(addr); }

        inline void dump(int level)
        { bfdebug_nhex(level, name, get()); }
    }

    namespace edx
    {
        constexpr const auto name = "processor_string_2_edx";

        inline auto get() noexcept
        { return _cpuid_edx(addr); }

        inline void dump(int level)
        { bfdebug_nhex(level, name, get()); }
    }

    inline void dump(int level)
    {
        eax::dump(level);
        ebx::dump(level);
        ecx::dump(level);
        edx::dump(level);
    }
}

namespace processor_string_3
{
    constexpr const auto addr = 0x80000004ULL;

    namespace eax
    {
        constexpr const auto name = "processor_string_3_eax";

        inline auto get() noexcept
        { return _cpuid_eax(addr); }

        inline void dump(int level)
        { bfdebug_nhex(level, name, get()); }
    }

    namespace ebx
    {
        constexpr const auto name = "processor_string_3_ebx";

        inline auto get() noexcept
        { return _cpuid_ebx(addr); }

        inline void dump(int level)
        { bfdebug_nhex(level, name, get()); }
    }

    namespace ecx
    {
        constexpr const auto name = "processor_string_3_ecx";

        inline auto get() noexcept
        { return _cpuid_ecx(addr); }

        inline void dump(int level)
        { bfdebug_nhex(level, name, get()); }
    }

    namespace edx
    {
        constexpr const auto name = "processor_string_3_edx";

        inline auto get() noexcept
        { return _cpuid_edx(addr); }

        inline void dump(int level)
        { bfdebug_nhex(level, name, get()); }
    }
}

}
}

// *INDENT-ON*

#endif
