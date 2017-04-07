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

#ifndef MSRS_X64_H
#define MSRS_X64_H

#include <bfgsl.h>
#include <bfdebug.h>
#include <bfbitmanip.h>

// -----------------------------------------------------------------------------
// Exports
// -----------------------------------------------------------------------------

#include <bfexports.h>

#ifdef COMPILING_INTRINSICS
#define EXPORT_INTRINSICS EXPORT_SYM
#else
#define EXPORT_INTRINSICS IMPORT_SYM
#endif

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------

extern "C" EXPORT_INTRINSICS uint64_t _read_msr(uint32_t addr) noexcept;
extern "C" EXPORT_INTRINSICS void _write_msr(uint32_t addr, uint64_t val) noexcept;

// *INDENT-OFF*

namespace x64
{
namespace msrs
{
    using field_type = uint32_t;
    using value_type = uint64_t;

    template<typename A> inline auto get(A addr) noexcept
    { return _read_msr(gsl::narrow_cast<field_type>(addr)); }

    template<typename A, class T> void set(A addr, T val) noexcept
    { _write_msr(gsl::narrow_cast<field_type>(addr), val); }

    namespace ia32_pat
    {
        constexpr const auto addr = 0x00000277U;
        constexpr const auto name = "ia32_pat";

        inline auto get() noexcept
        { return _read_msr(addr); }

        template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
        void set(T val) noexcept { _write_msr(addr, val); }

        namespace pa0
        {
            constexpr const auto mask = 0x0000000000000007ULL;
            constexpr const auto from = 0;
            constexpr const auto name = "pa0";

            inline auto get() noexcept
            { return get_bits(_read_msr(addr), mask) >> from; }

            template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
            auto get(T val) noexcept
            { return get_bits(val, mask) >> from; }

            template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
            void set(T val) noexcept { _write_msr(addr, set_bits(_read_msr(addr), mask, val << from)); }
        }

        namespace pa1
        {
            constexpr const auto mask = 0x0000000000000700ULL;
            constexpr const auto from = 8;
            constexpr const auto name = "pa1";

            inline auto get() noexcept
            { return get_bits(_read_msr(addr), mask) >> from; }

            template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
            auto get(T val) noexcept
            { return get_bits(val, mask) >> from; }

            template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
            void set(T val) noexcept { _write_msr(addr, set_bits(_read_msr(addr), mask, val << from)); }
        }

        namespace pa2
        {
            constexpr const auto mask = 0x0000000000070000ULL;
            constexpr const auto from = 16;
            constexpr const auto name = "pa2";

            inline auto get() noexcept
            { return get_bits(_read_msr(addr), mask) >> from; }

            template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
            auto get(T val) noexcept
            { return get_bits(val, mask) >> from; }

            template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
            void set(T val) noexcept { _write_msr(addr, set_bits(_read_msr(addr), mask, val << from)); }
        }

        namespace pa3
        {
            constexpr const auto mask = 0x0000000007000000ULL;
            constexpr const auto from = 24;
            constexpr const auto name = "pa3";

            inline auto get() noexcept
            { return get_bits(_read_msr(addr), mask) >> from; }

            template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
            auto get(T val) noexcept
            { return get_bits(val, mask) >> from; }

            template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
            void set(T val) noexcept { _write_msr(addr, set_bits(_read_msr(addr), mask, val << from)); }
        }

        namespace pa4
        {
            constexpr const auto mask = 0x0000000700000000ULL;
            constexpr const auto from = 32;
            constexpr const auto name = "pa4";

            inline auto get() noexcept
            { return get_bits(_read_msr(addr), mask) >> from; }

            template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
            auto get(T val) noexcept
            { return get_bits(val, mask) >> from; }

            template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
            void set(T val) noexcept { _write_msr(addr, set_bits(_read_msr(addr), mask, val << from)); }
        }

        namespace pa5
        {
            constexpr const auto mask = 0x0000070000000000ULL;
            constexpr const auto from = 40;
            constexpr const auto name = "pa5";

            inline auto get() noexcept
            { return get_bits(_read_msr(addr), mask) >> from; }

            template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
            auto get(T val) noexcept
            { return get_bits(val, mask) >> from; }

            template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
            void set(T val) noexcept { _write_msr(addr, set_bits(_read_msr(addr), mask, val << from)); }
        }

        namespace pa6
        {
            constexpr const auto mask = 0x0007000000000000ULL;
            constexpr const auto from = 48;
            constexpr const auto name = "pa6";

            inline auto get() noexcept
            { return get_bits(_read_msr(addr), mask) >> from; }

            template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
            auto get(T val) noexcept
            { return get_bits(val, mask) >> from; }

            template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
            void set(T val) noexcept { _write_msr(addr, set_bits(_read_msr(addr), mask, val << from)); }
        }

        namespace pa7
        {
            constexpr const auto mask = 0x0700000000000000ULL;
            constexpr const auto from = 56;
            constexpr const auto name = "pa7";

            inline auto get() noexcept
            { return get_bits(_read_msr(addr), mask) >> from; }

            template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
            auto get(T val) noexcept
            { return get_bits(val, mask) >> from; }

            template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
            void set(T val) noexcept { _write_msr(addr, set_bits(_read_msr(addr), mask, val << from)); }
        }

        inline void dump() noexcept
        {
            bfdebug << "msrs::ia32_pat fields:" << bfendl;

            bfdebug << "    - " << pa0::name << " = "
                    << view_as_pointer(pa0::get()) << bfendl;
            bfdebug << "    - " << pa1::name << " = "
                    << view_as_pointer(pa1::get()) << bfendl;
            bfdebug << "    - " << pa2::name << " = "
                    << view_as_pointer(pa2::get()) << bfendl;
            bfdebug << "    - " << pa3::name << " = "
                    << view_as_pointer(pa3::get()) << bfendl;
            bfdebug << "    - " << pa4::name << " = "
                    << view_as_pointer(pa4::get()) << bfendl;
            bfdebug << "    - " << pa5::name << " = "
                    << view_as_pointer(pa5::get()) << bfendl;
            bfdebug << "    - " << pa6::name << " = "
                    << view_as_pointer(pa6::get()) << bfendl;
            bfdebug << "    - " << pa7::name << " = "
                    << view_as_pointer(pa7::get()) << bfendl;
        }

        template<typename T, typename =  std::enable_if<std::is_integral<T>::value>>
        auto pa(T index)
        {
            switch (index)
            {
                case 0: return pa0::get();
                case 1: return pa1::get();
                case 2: return pa2::get();
                case 3: return pa3::get();
                case 4: return pa4::get();
                case 5: return pa5::get();
                case 6: return pa6::get();
                case 7: return pa7::get();
                default:
                    throw std::runtime_error("unknown pat index");
            };
        }

        template<typename V, class I,
                 typename =  std::enable_if<std::is_integral<V>::value>,
                 typename =  std::enable_if<std::is_integral<I>::value>>
        auto pa(V value, I index)
        {
            switch (index)
            {
                case 0: return pa0::get(value);
                case 1: return pa1::get(value);
                case 2: return pa2::get(value);
                case 3: return pa3::get(value);
                case 4: return pa4::get(value);
                case 5: return pa5::get(value);
                case 6: return pa6::get(value);
                case 7: return pa7::get(value);
                default:
                    throw std::runtime_error("unknown pat index");
            };
        }
    }

}
}

// *INDENT-ON*

#endif