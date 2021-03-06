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

#include <catch/catch.hpp>

#include <vcpu/vcpu.h>
#include <debug_ring/debug_ring.h>

TEST_CASE("vcpu: invalid_id")
{
    CHECK_THROWS(std::make_unique<vcpu>(vcpuid::reserved, nullptr));
}

TEST_CASE("vcpu: null_debug_ring")
{
    CHECK_NOTHROW(std::make_unique<vcpu>(0, nullptr));
}

TEST_CASE("vcpu: valid")
{
    auto dr = std::unique_ptr<debug_ring>(nullptr);
    CHECK_NOTHROW(std::make_unique<vcpu>(0, std::move(dr)));
}

TEST_CASE("vcpu: write_empty_string")
{
    char rb[DEBUG_RING_SIZE];
    debug_ring_resources_t *drr = nullptr;
    auto &&vc = std::make_unique<vcpu>(0);

    vc->write("");
    get_drr(0, &drr);

    CHECK(debug_ring_read(drr, static_cast<char *>(rb), DEBUG_RING_SIZE) == 0);
}

TEST_CASE("vcpu: write_hello_world")
{
    char rb[DEBUG_RING_SIZE];
    debug_ring_resources_t *drr = nullptr;
    auto &&vc = std::make_unique<vcpu>(0);

    vc->write("hello world");
    get_drr(0, &drr);

    CHECK(debug_ring_read(drr, static_cast<char *>(rb), DEBUG_RING_SIZE) == 11);
}

TEST_CASE("vcpu: init_null_attr")
{
    auto &&vc = std::make_unique<vcpu>(0);

    CHECK_FALSE(vc->is_initialized());
    vc->init(nullptr);
    CHECK(vc->is_initialized());
}

TEST_CASE("vcpu: init_valid_attr")
{
    user_data data{};
    auto &&vc = std::make_unique<vcpu>(0);

    CHECK_FALSE(vc->is_initialized());
    vc->init(&data);
    CHECK(vc->is_initialized());
}

TEST_CASE("vcpu: fini_null_attr")
{
    auto &&vc = std::make_unique<vcpu>(0);

    vc->init();

    CHECK(vc->is_initialized());
    vc->fini(nullptr);
    CHECK_FALSE(vc->is_initialized());
}

TEST_CASE("vcpu: fini_valid_attr")
{
    user_data data{};
    auto &&vc = std::make_unique<vcpu>(0);

    vc->init();

    CHECK(vc->is_initialized());
    vc->fini(&data);
    CHECK_FALSE(vc->is_initialized());
}

TEST_CASE("vcpu: fini_without_init_without_run")
{
    auto &&vc = std::make_unique<vcpu>(0);

    CHECK_FALSE(vc->is_running());
    CHECK_FALSE(vc->is_initialized());
    vc->fini();
    CHECK_FALSE(vc->is_running());
    CHECK_FALSE(vc->is_initialized());
}

TEST_CASE("vcpu: fini_with_init_without_run")
{
    auto &&vc = std::make_unique<vcpu>(0);

    vc->init();

    CHECK_FALSE(vc->is_running());
    CHECK(vc->is_initialized());
    vc->fini();
    CHECK_FALSE(vc->is_running());
    CHECK_FALSE(vc->is_initialized());
}

TEST_CASE("vcpu: fini_without_init_with_run")
{
    auto &&vc = std::make_unique<vcpu>(0);

    vc->run();

    CHECK(vc->is_running());
    CHECK_FALSE(vc->is_initialized());
    vc->fini();
    CHECK_FALSE(vc->is_running());
    CHECK_FALSE(vc->is_initialized());
}

TEST_CASE("vcpu: fini_with_init_with_run")
{
    auto &&vc = std::make_unique<vcpu>(0);

    vc->init();
    vc->run();

    CHECK(vc->is_running());
    CHECK(vc->is_initialized());
    vc->fini();
    CHECK_FALSE(vc->is_running());
    CHECK_FALSE(vc->is_initialized());
}

TEST_CASE("vcpu: run_null_attr")
{
    auto &&vc = std::make_unique<vcpu>(0);

    CHECK_FALSE(vc->is_running());
    vc->run(nullptr);
    CHECK(vc->is_running());
}

TEST_CASE("vcpu: run_valid_attr")
{
    user_data data{};
    auto &&vc = std::make_unique<vcpu>(0);

    CHECK_FALSE(vc->is_running());
    vc->run(&data);
    CHECK(vc->is_running());
}

TEST_CASE("vcpu: run_without_init")
{
    auto &&vc = std::make_unique<vcpu>(0);

    CHECK_FALSE(vc->is_running());
    vc->run();
    CHECK(vc->is_running());
}

TEST_CASE("vcpu: run_with_init")
{
    auto &&vc = std::make_unique<vcpu>(0);

    vc->init();

    CHECK_FALSE(vc->is_running());
    vc->run();
    CHECK(vc->is_running());
}

TEST_CASE("vcpu: hlt_null_attr")
{
    auto &&vc = std::make_unique<vcpu>(0);

    CHECK_FALSE(vc->is_running());
    vc->hlt(nullptr);
    CHECK_FALSE(vc->is_running());
}

TEST_CASE("vcpu: hlt_valid_attr")
{
    user_data data{};
    auto &&vc = std::make_unique<vcpu>(0);

    CHECK_FALSE(vc->is_running());
    vc->hlt(&data);
    CHECK_FALSE(vc->is_running());
}

TEST_CASE("vcpu: hlt_without_run")
{
    auto &&vc = std::make_unique<vcpu>(0);

    CHECK_FALSE(vc->is_running());
    vc->hlt();
    CHECK_FALSE(vc->is_running());
}

TEST_CASE("vcpu: hlt_with_run")
{
    auto &&vc = std::make_unique<vcpu>(0);

    vc->run();

    CHECK(vc->is_running());
    vc->hlt();
    CHECK_FALSE(vc->is_running());
}

TEST_CASE("vcpu: id")
{
    auto vc = std::make_unique<vcpu>(1);
    CHECK(vc->id() == 1);
}

TEST_CASE("vcpu: is_bootstrap_vcpu")
{
    auto &&vc = std::make_unique<vcpu>(0);
    CHECK(vc->is_bootstrap_vcpu());
}

TEST_CASE("vcpu: is_not_bootstrap_vcpu")
{
    auto vc = std::make_unique<vcpu>(1);
    CHECK_FALSE(vc->is_bootstrap_vcpu());
}

TEST_CASE("vcpu: is_host_vm_vcpu")
{
    auto vc = std::make_unique<vcpu>(1);
    CHECK(vc->is_host_vm_vcpu());
}

TEST_CASE("vcpu: is_not_host_vm_vcpu")
{
    auto vc = std::make_unique<vcpu>(0x0000000100000000);
    CHECK_FALSE(vc->is_host_vm_vcpu());
}

TEST_CASE("vcpu: is_guest_vm_vcpu")
{
    auto vc = std::make_unique<vcpu>(0x0000000100000000);
    CHECK(vc->is_guest_vm_vcpu());
}

TEST_CASE("vcpu: is_not_guest_vm_vcpu")
{
    auto vc = std::make_unique<vcpu>(1);
    CHECK_FALSE(vc->is_guest_vm_vcpu());
}

TEST_CASE("vcpu: is_running_vm_vcpu")
{
    auto &&vc = std::make_unique<vcpu>(0);

    vc->run();
    CHECK(vc->is_running());
}

TEST_CASE("vcpu: is_not_running_vm_vcpu")
{
    auto &&vc = std::make_unique<vcpu>(0);
    CHECK_FALSE(vc->is_running());
}

TEST_CASE("vcpu: is_initialized_vm_vcpu")
{
    auto &&vc = std::make_unique<vcpu>(0);

    vc->init();
    CHECK(vc->is_initialized());
}

TEST_CASE("vcpu: is_not_initialized_vm_vcpu")
{
    auto &&vc = std::make_unique<vcpu>(0);
    CHECK_FALSE(vc->is_initialized());
}
