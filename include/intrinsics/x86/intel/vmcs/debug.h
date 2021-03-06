//
// Bareflank Hypervisor
//
// Copyright (C) 2015 Assured Information Security, Inc.
// Author: Rian Quinn        <quinnr@ainfosec.com>
// Author: Brendan Kerrigan  <kerriganb@ainfosec.com>
// Author: Connor Davis      <davisc@ainfosec.com>
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

#ifndef VMCS_INTEL_X64_DEBUG_H
#define VMCS_INTEL_X64_DEBUG_H

#include <intrinsics/x86/intel/vmcs/16bit_control_fields.h>
#include <intrinsics/x86/intel/vmcs/16bit_guest_state_fields.h>
#include <intrinsics/x86/intel/vmcs/16bit_host_state_fields.h>
#include <intrinsics/x86/intel/vmcs/64bit_control_fields.h>
#include <intrinsics/x86/intel/vmcs/64bit_read_only_data_fields.h>
#include <intrinsics/x86/intel/vmcs/64bit_guest_state_fields.h>
#include <intrinsics/x86/intel/vmcs/64bit_host_state_fields.h>
#include <intrinsics/x86/intel/vmcs/32bit_control_fields.h>
#include <intrinsics/x86/intel/vmcs/32bit_read_only_data_fields.h>
#include <intrinsics/x86/intel/vmcs/32bit_guest_state_fields.h>
#include <intrinsics/x86/intel/vmcs/32bit_host_state_field.h>
#include <intrinsics/x86/intel/vmcs/natural_width_control_fields.h>
#include <intrinsics/x86/intel/vmcs/natural_width_read_only_data_fields.h>
#include <intrinsics/x86/intel/vmcs/natural_width_guest_state_fields.h>
#include <intrinsics/x86/intel/vmcs/natural_width_host_state_fields.h>

// *INDENT-OFF*

namespace intel_x64
{
namespace vmcs
{

namespace debug
{
    inline void dump(int level = 0)
    {
        bfdebug_brline(level);
        bfdebug_header(level, "16bit control fields");
        bfdebug_break3(level);

        virtual_processor_identifier::dump(level);
        posted_interrupt_notification_vector::dump(level);
        eptp_index::dump(level);

        bfdebug_brline(level);
        bfdebug_header(level, "16bit guest state fields");
        bfdebug_break3(level);

        guest_es_selector::dump(level);
        guest_cs_selector::dump(level);
        guest_ss_selector::dump(level);
        guest_ds_selector::dump(level);
        guest_fs_selector::dump(level);
        guest_gs_selector::dump(level);
        guest_ldtr_selector::dump(level);
        guest_tr_selector::dump(level);
        guest_interrupt_status::dump(level);

        bfdebug_brline(level);
        bfdebug_header(level, "16bit host state fields");
        bfdebug_break3(level);

        host_es_selector::dump(level);
        host_cs_selector::dump(level);
        host_ss_selector::dump(level);
        host_ds_selector::dump(level);
        host_fs_selector::dump(level);
        host_gs_selector::dump(level);
        host_tr_selector::dump(level);

        bfdebug_brline(level);
        bfdebug_header(level, "64bit control fields");
        bfdebug_break3(level);

        address_of_io_bitmap_a::dump(level);
        address_of_io_bitmap_b::dump(level);
        address_of_msr_bitmap::dump(level);
        vm_exit_msr_store_address::dump(level);
        vm_exit_msr_load_address::dump(level);
        vm_entry_msr_load_address::dump(level);
        executive_vmcs_pointer::dump(level);
        tsc_offset::dump(level);
        virtual_apic_address::dump(level);
        apic_access_address::dump(level);
        posted_interrupt_descriptor_address::dump(level);
        vm_function_controls::dump(level);
        ept_pointer::dump(level);
        eoi_exit_bitmap_0::dump(level);
        eoi_exit_bitmap_1::dump(level);
        eoi_exit_bitmap_2::dump(level);
        eoi_exit_bitmap_3::dump(level);
        eptp_list_address::dump(level);
        vmread_bitmap_address::dump(level);
        vmwrite_bitmap_address::dump(level);
        virtualization_exception_information_address::dump(level);
        xss_exiting_bitmap::dump(level);

        bfdebug_brline(level);
        bfdebug_header(level, "64bit read-only data fields");
        bfdebug_break3(level);

        guest_physical_address::dump(level);

        bfdebug_brline(level);
        bfdebug_header(level, "64bit guest state fields");
        bfdebug_break3(level);

        vmcs_link_pointer::dump(level);
        guest_ia32_debugctl::dump(level);
        guest_ia32_pat::dump(level);
        guest_ia32_efer::dump(level);
        guest_ia32_perf_global_ctrl::dump(level);
        guest_pdpte0::dump(level);
        guest_pdpte1::dump(level);
        guest_pdpte2::dump(level);
        guest_pdpte3::dump(level);

        bfdebug_brline(level);
        bfdebug_header(level, "64bit host state fields");
        bfdebug_break3(level);

        host_ia32_pat::dump(level);
        host_ia32_efer::dump(level);
        host_ia32_perf_global_ctrl::dump(level);

        bfdebug_brline(level);
        bfdebug_header(level, "32bit control fields");
        bfdebug_break3(level);

        pin_based_vm_execution_controls::dump(level);
        primary_processor_based_vm_execution_controls::dump(level);
        exception_bitmap::dump(level);
        page_fault_error_code_mask::dump(level);
        page_fault_error_code_match::dump(level);
        cr3_target_count::dump(level);
        vm_exit_controls::dump(level);
        vm_exit_msr_store_count::dump(level);
        vm_exit_msr_load_count::dump(level);
        vm_entry_controls::dump(level);
        vm_entry_msr_load_count::dump(level);
        vm_entry_interruption_information_field::dump(level);
        vm_entry_exception_error_code::dump(level);
        vm_entry_instruction_length::dump(level);
        tpr_threshold::dump(level);
        secondary_processor_based_vm_execution_controls::dump(level);
        ple_gap::dump(level);
        ple_window::dump(level);

        bfdebug_brline(level);
        bfdebug_header(level, "32bit read-only data fields");
        bfdebug_break3(level);

        exit_reason::dump(level);
        vm_exit_interruption_information::dump(level);
        vm_exit_interruption_error_code::dump(level);
        idt_vectoring_information::dump(level);
        idt_vectoring_error_code::dump(level);
        vm_exit_instruction_length::dump(level);
        vm_exit_instruction_information::dump(level);

        bfdebug_brline(level);
        bfdebug_header(level, "32bit guest state fields");
        bfdebug_break3(level);

        guest_es_limit::dump(level);
        guest_cs_limit::dump(level);
        guest_ss_limit::dump(level);
        guest_ds_limit::dump(level);
        guest_fs_limit::dump(level);
        guest_gs_limit::dump(level);
        guest_ldtr_limit::dump(level);
        guest_tr_limit::dump(level);
        guest_gdtr_limit::dump(level);
        guest_idtr_limit::dump(level);
        guest_es_access_rights::dump(level);
        guest_cs_access_rights::dump(level);
        guest_ss_access_rights::dump(level);
        guest_ds_access_rights::dump(level);
        guest_fs_access_rights::dump(level);
        guest_gs_access_rights::dump(level);
        guest_ldtr_access_rights::dump(level);
        guest_tr_access_rights::dump(level);
        guest_interruptibility_state::dump(level);
        guest_activity_state::dump(level);
        guest_smbase::dump(level);
        guest_ia32_sysenter_cs::dump(level);
        vmx_preemption_timer_value::dump(level);

        bfdebug_brline(level);
        bfdebug_header(level, "32bit host state fields");
        bfdebug_break3(level);

        host_ia32_sysenter_cs::dump(level);

        bfdebug_brline(level);
        bfdebug_header(level, "natural width control fields");
        bfdebug_break3(level);

        cr0_guest_host_mask::dump(level);
        cr4_guest_host_mask::dump(level);
        cr0_read_shadow::dump(level);
        cr4_read_shadow::dump(level);
        cr3_target_value_0::dump(level);
        cr3_target_value_1::dump(level);
        cr3_target_value_2::dump(level);
        cr3_target_value_3::dump(level);

        bfdebug_brline(level);
        bfdebug_header(level, "natural width read-only data fields");
        bfdebug_break3(level);

        exit_qualification::dump(level);
        io_rcx::dump(level);
        io_rsi::dump(level);
        io_rdi::dump(level);
        io_rip::dump(level);
        guest_linear_address::dump(level);

        bfdebug_brline(level);
        bfdebug_header(level, "natural width guest state fields");
        bfdebug_break3(level);

        guest_cr0::dump(level);
        guest_cr3::dump(level);
        guest_cr4::dump(level);
        guest_es_base::dump(level);
        guest_cs_base::dump(level);
        guest_ss_base::dump(level);
        guest_ds_base::dump(level);
        guest_fs_base::dump(level);
        guest_gs_base::dump(level);
        guest_ldtr_base::dump(level);
        guest_tr_base::dump(level);
        guest_gdtr_base::dump(level);
        guest_idtr_base::dump(level);
        guest_dr7::dump(level);
        guest_rsp::dump(level);
        guest_rip::dump(level);
        guest_rflags::dump(level);
        guest_pending_debug_exceptions::dump(level);
        guest_ia32_sysenter_esp::dump(level);
        guest_ia32_sysenter_eip::dump(level);

        bfdebug_brline(level);
        bfdebug_header(level, "natural width host state fields");
        bfdebug_break3(level);

        host_cr0::dump(level);
        host_cr3::dump(level);
        host_cr4::dump(level);
        host_fs_base::dump(level);
        host_gs_base::dump(level);
        host_tr_base::dump(level);
        host_gdtr_base::dump(level);
        host_idtr_base::dump(level);
        host_ia32_sysenter_esp::dump(level);
        host_ia32_sysenter_eip::dump(level);
        host_rsp::dump(level);
        host_rip::dump(level);
    }
}

}
}

// *INDENT-ON*

#endif
