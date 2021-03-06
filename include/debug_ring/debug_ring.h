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

#ifndef DEBUG_RING_H
#define DEBUG_RING_H

#include <string>
#include <memory>

#include <bftypes.h>
#include <bfvcpuid.h>
#include <bfdebugringinterface.h>

// -----------------------------------------------------------------------------
// Exports
// -----------------------------------------------------------------------------

#include <bfexports.h>

#ifndef STATIC_DEBUG_RING
#ifdef SHARED_DEBUG_RING
#define EXPORT_DEBUG_RING EXPORT_SYM
#else
#define EXPORT_DEBUG_RING IMPORT_SYM
#endif
#else
#define EXPORT_DEBUG_RING
#endif

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------

/// Debug Ring
///
/// The debug ring is a simple debug facility that allows the vmm to write
/// string data into a ring buffer while a reader that has shared access to
/// the same buffer can read from the debug ring to extract the strings
/// that are written to the buffer.
///
class EXPORT_DEBUG_RING debug_ring
{
public:

    /// Default Constructor
    ///
    /// @expects none
    /// @ensures none
    ///
    debug_ring(vcpuid::type vcpuid) noexcept;

    /// Debug Ring Destructor
    ///
    /// @expects none
    /// @ensures none
    ///
    VIRTUAL ~debug_ring() noexcept;

    /// Write to Debug Ring
    ///
    /// Writes a string to the debug ring. If the string is larger than
    /// the debug ring's internal buffer, the write will fail. If the debug
    /// ring is full, the write will keep removing existing strings in the
    /// buffer until enough space is made, to add the string.
    ///
    /// @expects none
    /// @ensures none
    ///
    /// @param str the string to write to the debug ring
    ///
    VIRTUAL void write(const std::string &str) noexcept;

private:

    vcpuid::type m_vcpuid;
    std::unique_ptr<debug_ring_resources_t> m_drr;

public:

    debug_ring(debug_ring &&) noexcept = default;
    debug_ring &operator=(debug_ring &&) noexcept = default;

    debug_ring(const debug_ring &) = delete;
    debug_ring &operator=(const debug_ring &) = delete;
};

/// Get Debug Ring Resource
///
/// Returns a pointer to a debug_ring_resources_t for a given CPU.
///
/// @expects drr != nullptr
/// @expects vcpuid == vcpu that exists
/// @ensures none
///
/// @param vcpuid defines which debug ring to return
/// @param drr the resulting debug ring
/// @return the debug_ring_resources_t for the provided vcpuid
///
extern "C" EXPORT_DEBUG_RING int64_t get_drr(uint64_t vcpuid,
        struct debug_ring_resources_t **drr) noexcept;

#endif
