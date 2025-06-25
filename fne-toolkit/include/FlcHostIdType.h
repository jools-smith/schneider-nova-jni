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
 *  Defines hostid types.
 */

#ifndef _INCLUDE_FLC_HOSTID_TYPE_H_
#define _INCLUDE_FLC_HOSTID_TYPE_H_

typedef enum FlcHostIdType
{
  FLC_HOSTID_TYPE_UNKNOWN                           = 0,          /**< Unknown */
  FLC_HOSTID_TYPE_UNSUPPORTED                       = 1,          /**< Unsupported */
  FLC_HOSTID_TYPE_LONGHOSTID                        = 2,          /**< Long */
  FLC_HOSTID_TYPE_ETHERNET                          = 3,          /**< Ethernet */
  FLC_HOSTID_TYPE_ANY                               = 4,          /**< Any */
  FLC_HOSTID_TYPE_USER                              = 5,          /**< User */
  FLC_HOSTID_TYPE_DISPLAY                           = 6,          /**< Display */
  FLC_HOSTID_TYPE_HOSTNAME                          = 7,          /**< Host Name */
  FLC_HOSTID_TYPE_STRING                            = 8,          /**< String */
  FLC_HOSTID_TYPE_FLEXID7                           = 9,          /**< FlexID 7 */
  FLC_HOSTID_TYPE_VSN                               = 10,         /**< Volume Serial Number */
  FLC_HOSTID_TYPE_INTERNET                          = 11,         /**< Internet */
  FLC_HOSTID_TYPE_INTERNET6                         = 12,         /**< Internet IPV6 */
  FLC_HOSTID_TYPE_FLEXID8                           = 13,         /**< FlexID 8 */
  FLC_HOSTID_TYPE_FLEXID9                           = 14,         /**< FlexID 9 */
  FLC_HOSTID_TYPE_HOSTDOMAIN                        = 15,         /**< Host Domain */
  FLC_HOSTID_TYPE_FLEXID6                           = 16,         /**< FlexID 6 */
  FLC_HOSTID_TYPE_COMPOSITE                         = 17,         /**< Composite */
  FLC_HOSTID_TYPE_VENDOR                            = 18,         /**< Vendor Defined */
  FLC_HOSTID_TYPE_FLEXID10                          = 19,         /**< FlexID 10 */
  FLC_HOSTID_TYPE_VM_UUID                           = 20,         /**< VM UUID */
  FLC_HOSTID_TYPE_AMAZON_EIP                        = 21,         /**< Amazon EIP */
  FLC_HOSTID_TYPE_AMAZON_AMI                        = 22,         /**< Amazon AMI */
  FLC_HOSTID_TYPE_TOLERANT                          = 23,         /**< Tolerant */
  FLC_HOSTID_TYPE_AMAZON_IID                        = 24,         /**< Amazon IID */
  FLC_HOSTID_TYPE_EXTENDED                          = 25,         /**< Extended */
  FLC_HOSTID_TYPE_PUBLISHER_DEFINED                 = 26,         /**< Publisher defined type */
  FLC_HOSTID_TYPE_CONTAINER_ID                      = 27,         /**< Docker container ID */
  FLC_HOSTID_NEXT
}FlcHostIdType;


#endif /*_INCLUDE_FLC_HOSTID_TYPE_H_*/
