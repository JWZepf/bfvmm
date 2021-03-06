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
#include <bfconstants.h>
#include <bfexception.h>
#include <bferrorcodes.h>

#include <memory_manager/memory_manager_x64.h>
#include <exit_handler/exit_handler_intel_x64.h>
#include <exit_handler/exit_handler_intel_x64_entry.h>
#include <exit_handler/exit_handler_intel_x64_support.h>

#include <intrinsics/x86/common_x64.h>
#include <intrinsics/x86/intel_x64.h>

using namespace x64;
using namespace intel_x64;

#include <mutex>
std::mutex g_unimplemented_handler_mutex;

void
exit_handler_intel_x64::dispatch()
{
    handle_exit(vmcs::exit_reason::basic_exit_reason::get());
}

void
exit_handler_intel_x64::halt() noexcept
{
    bferror_brline(0);
    bferror_header(0, "halting vcpu");
    bferror_break1(0);

    bfdebug_subnhex(0, "m_state_save->rax", m_state_save->rax);
    bfdebug_subnhex(0, "m_state_save->rbx", m_state_save->rbx);
    bfdebug_subnhex(0, "m_state_save->rcx", m_state_save->rcx);
    bfdebug_subnhex(0, "m_state_save->rdx", m_state_save->rdx);
    bfdebug_subnhex(0, "m_state_save->rbp", m_state_save->rbp);
    bfdebug_subnhex(0, "m_state_save->rsi", m_state_save->rsi);
    bfdebug_subnhex(0, "m_state_save->rdi", m_state_save->rdi);
    bfdebug_subnhex(0, "m_state_save->r08", m_state_save->r08);
    bfdebug_subnhex(0, "m_state_save->r09", m_state_save->r09);
    bfdebug_subnhex(0, "m_state_save->r10", m_state_save->r10);
    bfdebug_subnhex(0, "m_state_save->r11", m_state_save->r11);
    bfdebug_subnhex(0, "m_state_save->r12", m_state_save->r12);
    bfdebug_subnhex(0, "m_state_save->r13", m_state_save->r13);
    bfdebug_subnhex(0, "m_state_save->r14", m_state_save->r14);
    bfdebug_subnhex(0, "m_state_save->r15", m_state_save->r15);
    bfdebug_subnhex(0, "m_state_save->rip", m_state_save->rip);
    bfdebug_subnhex(0, "m_state_save->rsp", m_state_save->rsp);
    bfdebug_subnhex(0, "m_state_save->vcpuid", m_state_save->vcpuid);

    // TODO:
    //
    // Need to put a place holder here. Instead of stopping, we should call
    // a function that stops, but can also be overridden by an extension like
    // the hyperkenel so that another vcpu can be scheduled instead.
    //

    pm::stop();
}

void
exit_handler_intel_x64::handle_exit(vmcs::value_type reason)
{
    switch (reason) {
        case vmcs::exit_reason::basic_exit_reason::cpuid:
            handle_cpuid();
            break;

        case vmcs::exit_reason::basic_exit_reason::invd:
            handle_invd();
            break;

        case vmcs::exit_reason::basic_exit_reason::vmcall:
            handle_vmcall();
            break;

        case vmcs::exit_reason::basic_exit_reason::vmxoff:
            handle_vmxoff();
            break;

        case vmcs::exit_reason::basic_exit_reason::rdmsr:
            handle_rdmsr();
            break;

        case vmcs::exit_reason::basic_exit_reason::wrmsr:
            handle_wrmsr();
            break;

        default:
            unimplemented_handler();
            break;
    };

    m_vmcs->resume();
}

void
exit_handler_intel_x64::handle_cpuid()
{
    auto ret = x64::cpuid::get(gsl::narrow_cast<x64::cpuid::field_type>(m_state_save->rax),
                               gsl::narrow_cast<x64::cpuid::field_type>(m_state_save->rbx),
                               gsl::narrow_cast<x64::cpuid::field_type>(m_state_save->rcx),
                               gsl::narrow_cast<x64::cpuid::field_type>(m_state_save->rdx));

    m_state_save->rax = std::get<0>(ret);
    m_state_save->rbx = std::get<1>(ret);
    m_state_save->rcx = std::get<2>(ret);
    m_state_save->rdx = std::get<3>(ret);

    advance_rip();
}

void
exit_handler_intel_x64::handle_invd()
{
    cache::wbinvd();
    advance_rip();
}

void
exit_handler_intel_x64::handle_vmcall()
{
    auto &&regs = vmcall_registers_t{};

    switch (m_state_save->rax) {
        case VMCALL_EVENT:
            regs.r02 = m_state_save->rcx;
            break;

        default:
            regs.r02 = m_state_save->rcx;
            regs.r03 = m_state_save->rbx;
            regs.r04 = m_state_save->rsi;
            regs.r05 = m_state_save->r08;
            regs.r06 = m_state_save->r09;
            regs.r07 = m_state_save->r10;
            regs.r08 = m_state_save->r11;
            regs.r09 = m_state_save->r12;
            regs.r10 = m_state_save->r13;
            regs.r11 = m_state_save->r14;
            regs.r12 = m_state_save->r15;
            break;
    };

    if (m_state_save->rdx != VMCALL_MAGIC_NUMBER) {
        return complete_vmcall(BF_VMCALL_FAILURE, regs);
    }

    auto &&ret = guard_exceptions(BF_VMCALL_FAILURE, [&] {
        switch (m_state_save->rax)
        {
            case VMCALL_VERSIONS:
                handle_vmcall_versions(regs);
                break;

            case VMCALL_REGISTERS:
                handle_vmcall_registers(regs);
                break;

            case VMCALL_DATA:
                handle_vmcall_data(regs);
                break;

            case VMCALL_EVENT:
                handle_vmcall_event(regs);
                break;

            case VMCALL_START:
                handle_vmcall_start(regs);
                break;

            case VMCALL_STOP:
                handle_vmcall_stop(regs);
                break;

            case VMCALL_UNITTEST:
                handle_vmcall_unittest(regs);
                break;

            default:
                throw std::runtime_error("unknown vmcall opcode");
        };
    });

    complete_vmcall(ret, regs);
}

void
exit_handler_intel_x64::complete_vmcall(
    ret_type ret, vmcall_registers_t &regs) noexcept
{
    switch (m_state_save->rax) {
        case VMCALL_EVENT:
            m_state_save->rcx = regs.r02;
            break;

        default:
            m_state_save->r15 = regs.r12;
            m_state_save->r14 = regs.r11;
            m_state_save->r13 = regs.r10;
            m_state_save->r12 = regs.r09;
            m_state_save->r11 = regs.r08;
            m_state_save->r10 = regs.r07;
            m_state_save->r09 = regs.r06;
            m_state_save->r08 = regs.r05;
            m_state_save->rsi = regs.r04;
            m_state_save->rbx = regs.r03;
            m_state_save->rcx = regs.r02;
            break;
    };

    m_state_save->rdx = static_cast < decltype(m_state_save->rdx) > (ret);
    advance_rip();
}

void
exit_handler_intel_x64::handle_vmxoff()
{ m_vmcs->promote(); }

void
exit_handler_intel_x64::handle_rdmsr()
{
    auto val = 0ULL;
    auto msr = gsl::narrow_cast<x64::msrs::field_type>(m_state_save->rcx);

    switch (msr) {
        case intel_x64::msrs::ia32_debugctl::addr:
            val = vmcs::guest_ia32_debugctl::get();
            break;

        case x64::msrs::ia32_pat::addr:
            val = vmcs::guest_ia32_pat::get();
            break;

        case intel_x64::msrs::ia32_efer::addr:
            val = vmcs::guest_ia32_efer::get();
            break;

        case intel_x64::msrs::ia32_perf_global_ctrl::addr:
            val = vmcs::guest_ia32_perf_global_ctrl::get();
            break;

        case intel_x64::msrs::ia32_sysenter_cs::addr:
            val = vmcs::guest_ia32_sysenter_cs::get();
            break;

        case intel_x64::msrs::ia32_sysenter_esp::addr:
            val = vmcs::guest_ia32_sysenter_esp::get();
            break;

        case intel_x64::msrs::ia32_sysenter_eip::addr:
            val = vmcs::guest_ia32_sysenter_eip::get();
            break;

        case intel_x64::msrs::ia32_fs_base::addr:
            val = vmcs::guest_fs_base::get();
            break;

        case intel_x64::msrs::ia32_gs_base::addr:
            val = vmcs::guest_gs_base::get();
            break;

        default:
            val = intel_x64::msrs::get(msr);
            break;

        // QUIRK:
        //
        // The following is specifically for CPU-Z. For whatever reason, it is
        // reading the following undefined MSRs, which causes the system to
        // freeze since attempting to read these MSRs in the exit handler
        // will cause a GP which is not being caught. The result is, the core
        // that runs RDMSR on these freezes, the other cores receive an
        // INIT signal to reset, and the system dies.
        //

        case 0x31:
        case 0x39:
        case 0x1ae:
        case 0x1af:
        case 0x602:
            val = 0;
            break;
    }

    m_state_save->rax = ((val >> 0x00) & 0x00000000FFFFFFFF);
    m_state_save->rdx = ((val >> 0x20) & 0x00000000FFFFFFFF);

    advance_rip();
}

void
exit_handler_intel_x64::handle_wrmsr()
{
    auto val = 0ULL;
    auto msr = gsl::narrow_cast<x64::msrs::field_type>(m_state_save->rcx);

    val |= ((m_state_save->rax & 0x00000000FFFFFFFF) << 0x00);
    val |= ((m_state_save->rdx & 0x00000000FFFFFFFF) << 0x20);

    switch (msr) {
        case intel_x64::msrs::ia32_debugctl::addr:
            vmcs::guest_ia32_debugctl::set(val);
            break;

        case x64::msrs::ia32_pat::addr:
            vmcs::guest_ia32_pat::set(val);
            break;

        case intel_x64::msrs::ia32_efer::addr:
            vmcs::guest_ia32_efer::set(val);
            break;

        case intel_x64::msrs::ia32_perf_global_ctrl::addr:
            vmcs::guest_ia32_perf_global_ctrl::set(val);
            break;

        case intel_x64::msrs::ia32_sysenter_cs::addr:
            vmcs::guest_ia32_sysenter_cs::set(val);
            break;

        case intel_x64::msrs::ia32_sysenter_esp::addr:
            vmcs::guest_ia32_sysenter_esp::set(val);
            break;

        case intel_x64::msrs::ia32_sysenter_eip::addr:
            vmcs::guest_ia32_sysenter_eip::set(val);
            break;

        case intel_x64::msrs::ia32_fs_base::addr:
            vmcs::guest_fs_base::set(val);
            break;

        case intel_x64::msrs::ia32_gs_base::addr:
            vmcs::guest_gs_base::set(val);
            break;

        default:
            intel_x64::msrs::set(msr, val);
            break;
    }

    advance_rip();
}

void
exit_handler_intel_x64::advance_rip() noexcept
{ m_state_save->rip += vmcs::vm_exit_instruction_length::get(); }

void
exit_handler_intel_x64::unimplemented_handler() noexcept
{
    std::lock_guard<std::mutex> guard(g_unimplemented_handler_mutex);

    bferror_brline(0);
    bferror_header(0, "unhandled exit reason");
    bferror_break1(0);

    vmcs::exit_reason::basic_exit_reason::dump(0);
    vmcs::exit_qualification::dump(0);
    vmcs::vm_exit_instruction_information::dump(0);
    vmcs::vm_exit_interruption_information::dump(0);
    vmcs::vm_exit_instruction_length::dump(0);
    vmcs::guest_linear_address::dump(0);
    vmcs::guest_physical_address::dump(0);

    if (vmcs::exit_reason::vm_entry_failure::is_enabled()) {

        guard_exceptions([&]
        { vmcs::check::all(); });

        guard_exceptions([&]
        { vmcs::debug::dump(); });
    }

    this->halt();
}

void
exit_handler_intel_x64::handle_vmcall_versions(vmcall_registers_t &regs)
{
    switch (regs.r02) {
        case VMCALL_VERSION_PROTOCOL:
            regs.r03 = VMCALL_VERSION;
            regs.r04 = 0;
            regs.r05 = 0;
            break;

        case VMCALL_VERSION_BAREFLANK:
            regs.r03 = BAREFLANK_VERSION_MAJOR;
            regs.r04 = BAREFLANK_VERSION_MINOR;
            regs.r05 = BAREFLANK_VERSION_PATCH;
            break;

        case VMCALL_VERSION_USER:
            regs.r03 = USER_VERSION_MAJOR;
            regs.r04 = USER_VERSION_MINOR;
            regs.r05 = USER_VERSION_PATCH;
            break;

        default:
            throw std::runtime_error("unknown vmcall version index");
    }
}

void
exit_handler_intel_x64::handle_vmcall_registers(vmcall_registers_t &regs)
{
    bfdebug << "vmcall registers:" << bfendl;
    bfdebug << "r02: " << view_as_pointer(regs.r02) << bfendl;
    bfdebug << "r03: " << view_as_pointer(regs.r03) << bfendl;
    bfdebug << "r04: " << view_as_pointer(regs.r04) << bfendl;
    bfdebug << "r05: " << view_as_pointer(regs.r05) << bfendl;
    bfdebug << "r06: " << view_as_pointer(regs.r06) << bfendl;
    bfdebug << "r07: " << view_as_pointer(regs.r07) << bfendl;
    bfdebug << "r08: " << view_as_pointer(regs.r08) << bfendl;
    bfdebug << "r09: " << view_as_pointer(regs.r09) << bfendl;
    bfdebug << "r10: " << view_as_pointer(regs.r10) << bfendl;
    bfdebug << "r11: " << view_as_pointer(regs.r11) << bfendl;
    bfdebug << "r12: " << view_as_pointer(regs.r12) << bfendl;
}

void
exit_handler_intel_x64::handle_vmcall_data(vmcall_registers_t &regs)
{
    expects(regs.r05 != 0);
    expects(regs.r08 != 0);
    expects(regs.r06 != 0);
    expects(regs.r09 != 0);
    expects(regs.r09 >= regs.r06);
    expects(regs.r06 <= VMCALL_IN_BUFFER_SIZE);
    expects(regs.r09 <= VMCALL_OUT_BUFFER_SIZE);

    auto &&imap = bfn::make_unique_map_x64<char>(regs.r05, vmcs::guest_cr3::get(), regs.r06,
                  vmcs::guest_ia32_pat::get());
    auto &&omap = bfn::make_unique_map_x64<char>(regs.r08, vmcs::guest_cr3::get(), regs.r09,
                  vmcs::guest_ia32_pat::get());

    switch (regs.r04) {
        case VMCALL_DATA_STRING_UNFORMATTED: {
            std::string ostr;
            handle_vmcall_data_string_unformatted(std::string(imap.get(), regs.r06), ostr);
            reply_with_string(regs, ostr, omap);
            break;
        }

        case VMCALL_DATA_STRING_JSON: {
            json ojson;
            handle_vmcall_data_string_json(json::parse(std::string(imap.get(), regs.r06)), ojson);
            reply_with_json(regs, ojson, omap);
            break;
        }

        case VMCALL_DATA_BINARY_UNFORMATTED: {
            handle_vmcall_data_binary_unformatted(imap, omap);
            regs.r07 = VMCALL_DATA_BINARY_UNFORMATTED;
            regs.r09 = regs.r06;
            break;
        }

        default:
            throw std::runtime_error("unknown vmcall data type");
    }
}

void
exit_handler_intel_x64::handle_vmcall_event(vmcall_registers_t &regs)
{
    bfdebug << "vmcall event:" << bfendl;
    bfdebug << "r02: " << view_as_pointer(regs.r02) << bfendl;
}

void
exit_handler_intel_x64::handle_vmcall_start(vmcall_registers_t &regs)
{ (void) regs; }

void
exit_handler_intel_x64::handle_vmcall_stop(vmcall_registers_t &regs)
{ (void) regs; }

void
exit_handler_intel_x64::handle_vmcall_data_string_unformatted(
    const std::string &istr, std::string &ostr)
{
    bfdebug << "received in vmm: " << istr << bfendl;
    ostr = istr;
}

void
exit_handler_intel_x64::handle_vmcall_data_string_json(
    const json &ijson, json &ojson)
{
    bfdebug << "received in vmm: " << ijson << bfendl;
    ojson = ijson;
}

void
exit_handler_intel_x64::handle_vmcall_data_binary_unformatted(
    const bfn::unique_map_ptr_x64<char> &imap,
    const bfn::unique_map_ptr_x64<char> &omap)
{
    bfdebug << "received binary data" << bfendl;
    memcpy(omap.get(), imap.get(), imap.size());
}

void exit_handler_intel_x64::reply_with_string(
    vmcall_registers_t &regs, const std::string &str,
    const bfn::unique_map_ptr_x64<char> &omap)
{
    auto &&len = str.length();

    memcpy(omap.get(), str.data(), len);

    regs.r07 = VMCALL_DATA_STRING_UNFORMATTED;
    regs.r09 = len;
}

void
exit_handler_intel_x64::reply_with_json(
    vmcall_registers_t &regs, const json &str,
    const bfn::unique_map_ptr_x64<char> &omap)
{
    auto &&dmp = str.dump();
    auto &&len = dmp.length();

    memcpy(omap.get(), dmp.data(), len);

    regs.r07 = VMCALL_DATA_STRING_JSON;
    regs.r09 = len;
}
