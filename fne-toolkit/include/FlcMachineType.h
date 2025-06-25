/****************************************************************************
  Copyright (c) 2012-2024 Flexera Software LLC.
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

/** @file
 *  Defines constants related to physical, virtual, and unknown machine types.
 */

#ifndef _INCLUDE_FLC_MACHINE_TYPE_H_
#define _INCLUDE_FLC_MACHINE_TYPE_H_

/**
 *  Machine type enumeration.
 */
typedef enum FlcMachineType
{
  FLC_MACHINE_TYPE_UNKNOWN                          = 0,          /**< Unknown */
  FLC_MACHINE_TYPE_PHYSICAL                         = 1,          /**< Physical */
  FLC_MACHINE_TYPE_VIRTUAL                          = 2,          /**< Virtual */
  FLC_MACHINE_TYPE_MAX = FLC_MACHINE_TYPE_VIRTUAL
}FlcMachineType;



/* Virtual Machine identifiers */
#define FLC_VIRTUAL_MACHINE_NAME            "VirtualMachineName"


/* Virtual Machine names */
#define FLC_VIRTUAL_MACHINE_VMWARE          "VMware"
#define FLC_VIRTUAL_MACHINE_HYPERV          "Microsoft Hyper-V"
#define FLC_VIRTUAL_MACHINE_VIRTUAL_PC      "Microsoft Virtual PC"
#define FLC_VIRTUAL_MACHINE_XEN             "Xen"
#define FLC_VIRTUAL_MACHINE_KVM             "KVM"
#define FLC_VIRTUAL_MACHINE_UNKNOWN         "Unknown"
#define FLC_VIRTUAL_MACHINE_AMAZONEC2       "Amazon EC2"
#define FLC_VIRTUAL_MACHINE_VIRTUALBOX      "Virtual Box"
#define FLC_VIRTUAL_MACHINE_EVERRUN         "Everrun"
#define FLC_VIRTUAL_MACHINE_NUTANIX         "Nutanix"
#define FLC_VIRTUAL_MACHINE_QEMU            "Qemu"
#define FLC_VIRTUAL_MACHINE_PARALLELS       "Parallels"
#define FLC_VIRTUAL_MACHINE_GOOGLE_COMPUTE  "Google Compute"
#define FLC_VIRTUAL_MACHINE_AZURE           "Azure"


#endif /*_INCLUDE_FLC_MACHINE_TYPE_H_*/
