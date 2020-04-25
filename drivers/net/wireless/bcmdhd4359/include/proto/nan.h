/*
 * Fundamental types and constants relating to WFA NAN
 * (Neighbor Awareness Networking)
 *
 * Copyright (C) 1999-2017, Broadcom Corporation
 * 
 *      Unless you and Broadcom execute a separate written software license
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2 (the "GPL"),
 * available at http://www.broadcom.com/licenses/GPLv2.php, with the
 * following added to such license:
 * 
 *      As a special exception, the copyright holders of this software give you
 * permission to link this software with independent modules, and to copy and
 * distribute the resulting executable under terms of your choice, provided that
 * you also meet, for each linked independent module, the terms and conditions of
 * the license of that module.  An independent module is a module which is not
 * derived from this software.  The special exception does not apply to any
 * modifications of the software.
 * 
 *      Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a license
 * other than the GPL, without Broadcom's express prior written consent.
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id: nan.h 649085 2016-07-14 22:09:00Z $
 */
#ifndef _NAN_H_
#define _NAN_H_

#include <typedefs.h>
#include <proto/802.11.h>


/* This marks the start of a packed structure section. */
#include <packed_section_start.h>

/* WiFi NAN OUI values */
#define NAN_OUI            WFA_OUI     /* WiFi OUI */
/* For oui_type field identifying the type and version of the NAN IE. */
#define NAN_OUI_TYPE       0x13        /* Type/Version */
/* IEEE 802.11 vendor specific information element. (Same as P2P_IE_ID.) */
#define NAN_IE_ID          0xdd

/* Same as P2P_PUB_AF_CATEGORY and DOT11_ACTION_CAT_PUBLIC */
#define NAN_PUB_AF_CATEGORY     0x04
/* IEEE 802.11 Public Action Frame Vendor Specific. (Same as P2P_PUB_AF_ACTION.) */
#define NAN_PUB_AF_ACTION       0x09
/* Number of octents in hash of service name. (Same as P2P_WFDS_HASH_LEN.) */
#define NAN_SVC_HASH_LEN    6
/* Size of fixed length part of nan_pub_act_frame_t before attributes. */
#define NAN_PUB_ACT_FRAME_FIXED_LEN 6
/* Number of octents in master rank value. */
#define NAN_MASTER_RANK_LEN     8
/* NAN public action frame header size */
#define NAN_PUB_ACT_FRAME_HDR_SIZE (OFFSETOF(nan_pub_act_frame_t, data))
/* NAN network ID */
#define NAN_NETWORK_ID		"\x51\x6F\x9A\x01\x00\x00"
/* Service Control Type length */
#define NAN_SVC_CONTROL_TYPE_LEN	2
/* Binding Bitmap length */
#define NAN_BINDING_BITMAP_LEN		2
/* Service Response Filter (SRF) control field masks */
#define NAN_SRF_BLOOM_MASK		0x01
#define NAN_SRF_INCLUDE_MASK		0x02
#define NAN_SRF_INDEX_MASK		0x0C
/* SRF Bloom Filter index shift */
#define NAN_SRF_BLOOM_SHIFT	2
#define NAN_SRF_INCLUDE_SHIFT	1
/* Mask for CRC32 output, used in hash function for NAN bloom filter */
#define NAN_BLOOM_CRC32_MASK	0xFFFF

/* Attribute TLV header size */
#define NAN_ATTR_ID_OFF		0
#define NAN_ATTR_LEN_OFF	1
#define NAN_ATTR_DATA_OFF	3

#define NAN_ATTR_ID_LEN		 1	/* ID field length */
#define NAN_ATTR_LEN_LEN	 2	/* Length field length */
#define NAN_ATTR_HDR_LEN	 3	/* ID + 2-byte length field */
#define NAN_ENTRY_CTRL_LEN       1      /* Entry control field length from FAM attribute */
#define NAN_MAP_ID_LEN           1	/* MAP ID length to signify band */
#define NAN_OPERATING_CLASS_LEN  1	/* operating class field length from NAN FAM */
#define NAN_CHANNEL_NUM_LEN      1	/* channel number field length 1 byte */

#define NAN_MAP_ID_2G   2  /* NAN Further Avail Map ID for band 2.4G */
#define NAN_MAP_ID_5G   5  /* NAN Further Avail Map ID for band 5G */
#define NAN_MAP_NUM_IDS 2  /* Max number of NAN Further Avail Map IDs supported */

/* no. of peer devices supported TODO make it tunable */
#define NAN_DATA_PEER_DEV_SUPPORT	8
/* no. of instaces supported (ndp, mgmt) */
#define NAN_DATA_NDP_INST_SUPPORT	16
/* instaces supported (same as ndp) */
#define NAN_DATA_MGMT_INST_SUPPORT	NAN_DATA_NDP_INST_SUPPORT
#define NAN_DATA_NDL_INST_SUPPORT	NAN_DATA_PEER_DEV_SUPPORT
/* ndc base schedule cannot be more than ndl instances */
#define NAN_DATA_NDC_INST_SUPPORT	NAN_DATA_PEER_DEV_SUPPORT

/* map id is 4 bits */
#define NAN_CMN_MAP_ID_LEN_BITS	4
/* siz eof ndc id */
#define NAN_DATA_NDC_ID_SIZE 6

/*
 * Period
 * Indicate the repeat interval of the following bitmap.
 * when set to 0, the indicated bitmap is not repeated.
 * When set to non-zero, the repeat interval is:
 * 1:128 TU, 2: 256 TU, 3: 512 TU, 4: 1024 TU, 5: 2048 TU, 6: 4096 TU, 7: 8192 TU
*/
#define NAN_DATA_MAX_AVAIL_INTRVL	7	/* no. of period intervals supported */

#define NAN_AVAIL_ENTRY_LEN_RES0 7      /* Avail entry len in FAM attribute for resolution 16TU */
#define NAN_AVAIL_ENTRY_LEN_RES1 5      /* Avail entry len in FAM attribute for resolution 32TU */
#define NAN_AVAIL_ENTRY_LEN_RES2 4      /* Avail entry len in FAM attribute for resolution 64TU */

/* NAN 2.0 NDP Setup */
#define NAN_DATA_NDP_SETUP  0	 /* arbitrary value */
/* NAN 2.0 Mgmt Setup */
#define NAN_DATA_MGMT_SETUP 1	 /* arbitrary value */
/* NAN 2.0 NDL Setup */
#define NAN_DATA_NDL_SETUP 2	 /* arbitrary value */

/* Vendor-specific public action frame for NAN */
typedef BWL_PRE_PACKED_STRUCT struct nan_pub_act_frame_s {
	/* NAN_PUB_AF_CATEGORY 0x04 */
	uint8 category_id;
	/* NAN_PUB_AF_ACTION 0x09 */
	uint8 action_field;
	/* NAN_OUI 0x50-6F-9A */
	uint8 oui[DOT11_OUI_LEN];
	/* NAN_OUI_TYPE 0x13 */
	uint8 oui_type;
	/* One or more NAN Attributes follow */
	uint8 data[1];
} BWL_POST_PACKED_STRUCT nan_pub_act_frame_t;

/* NAN attributes as defined in the nan spec */
enum {
	NAN_ATTR_MASTER_IND	= 0,
	NAN_ATTR_CLUSTER	= 1,
	NAN_ATTR_SVC_ID_LIST    = 2,
	NAN_ATTR_SVC_DESCRIPTOR = 3,
	NAN_ATTR_CONN_CAP       = 4,
	NAN_ATTR_INFRA		= 5,
	NAN_ATTR_P2P		= 6,
	NAN_ATTR_IBSS		= 7,
	NAN_ATTR_MESH		= 8,
	NAN_ATTR_FURTHER_NAN_SD = 9,
	NAN_ATTR_FURTHER_AVAIL	= 10,
	NAN_ATTR_COUNTRY_CODE	= 11,
	NAN_ATTR_RANGING	= 12,
	NAN_ATTR_CLUSTER_DISC	= 13,
	/* nan 2.0 */
	NAN_ATTR_SVC_DESC_EXTENSION = 14,
	NAN_ATTR_NAN_DEV_CAP = 15,
	NAN_ATTR_NAN_NDP = 16,
	NAN_ATTR_NAN_NMSG = 17,
	NAN_ATTR_NAN_AVAIL = 18,
	NAN_ATTR_NAN_NDC = 19,
	NAN_ATTR_NAN_NDL = 20,
	NAN_ATTR_NAN_NDL_QOS = 21,
	NAN_ATTR_MCAST_SCHED = 22,
	NAN_ATTR_UNALIGN_SCHED = 23,
	NAN_ATTR_PAGING_UCAST = 24,
	NAN_ATTR_PAGING_MCAST = 25,
	NAN_ATTR_RANGING_INFO = 26,
	NAN_ATTR_RANGING_SETUP = 27,
	NAN_ATTR_FTM_RANGE_REPORT = 28,
	NAN_ATTR_ELEMENT_CONTAINER = 29,
	NAN_ATTR_WLAN_INFRA_EXT = 30,
	NAN_ATTR_EXT_P2P_OPER = 31,
	NAN_ATTR_EXT_IBSS = 32,
	NAN_ATTR_EXT_MESH = 33,

	NAN_ATTR_VENDOR_SPECIFIC = 221,
	NAN_ATTR_NAN_MGMT	= 222	/* NAN Mgmt Attr (TBD; not in spec yet) */
};

#define NAN_ALL_NAN_MGMT_FRAMES (NAN_FRM_MGMT_AF | \
	NAN_FRM_NDP_AF | NAN_FRM_NDL_AF | \
	NAN_FRM_DISC_BCN | NAN_FRM_SYNC_BCN | \
	NAN_FRM_SVC_DISC)

enum wifi_nan_avail_resolution {
	NAN_AVAIL_RES_16_TU = 0,
	NAN_AVAIL_RES_32_TU = 1,
	NAN_AVAIL_RES_64_TU = 2,
	NAN_AVAIL_RES_INVALID = 255
};

typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_ie_s {
	uint8	id;		/* IE ID: NAN_IE_ID 0xDD */
	uint8	len;		/* IE length */
	uint8	oui[DOT11_OUI_LEN]; /* NAN_OUI 50:6F:9A */
	uint8	oui_type;	/* NAN_OUI_TYPE 0x13 */
	uint8	attr[1];	/* var len attributes */
} BWL_POST_PACKED_STRUCT wifi_nan_ie_t;

#define NAN_IE_HDR_SIZE	(OFFSETOF(wifi_nan_ie_t, attr))

/* master indication record  */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_master_ind_attr_s {
	uint8	id;
	uint16	len;
	uint8	master_preference;
	uint8	random_factor;
} BWL_POST_PACKED_STRUCT wifi_nan_master_ind_attr_t;

/* cluster attr record  */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_cluster_attr_s {
	uint8	id;
	uint16	len;
	uint8   amr[NAN_MASTER_RANK_LEN];
	uint8   hop_count;
	/* Anchor Master Beacon Transmission Time */
	uint32  ambtt;
} BWL_POST_PACKED_STRUCT wifi_nan_cluster_attr_t;

/*  container for service ID records  */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_svc_id_attr_s {
	uint8	id;
	uint16	len;
	uint8	svcid[0]; /* 6*len of srvc IDs */
} BWL_POST_PACKED_STRUCT wifi_nan_svc_id_attr_t;

/* service_control bitmap for wifi_nan_svc_descriptor_attr_t below */
#define NAN_SC_PUBLISH 0x0
#define NAN_SC_SUBSCRIBE 0x1
#define NAN_SC_FOLLOWUP 0x2
/* Set to 1 if a Matching Filter field is included in descriptors. */
#define NAN_SC_MATCHING_FILTER_PRESENT 0x4
/* Set to 1 if a Service Response Filter field is included in descriptors. */
#define NAN_SC_SR_FILTER_PRESENT 0x8
/* Set to 1 if a Service Info field is included in descriptors. */
#define NAN_SC_SVC_INFO_PRESENT 0x10
/* range is close proximity only */
#define NAN_SC_RANGE_LIMITED 0x20
/* Set to 1 if binding bitamp is present in descriptors */
#define NAN_SC_BINDING_BITMAP_PRESENT 0x40

/* Service descriptor */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_svc_descriptor_attr_s {
	/* Attribute ID - 0x03. */
	uint8 id;
	/* Length of the following fields in the attribute */
	uint16 len;
	/* Hash of the Service Name */
	uint8 svc_hash[NAN_SVC_HASH_LEN];
	/* Publish or subscribe instance id */
	uint8 instance_id;
	/* Requestor Instance ID */
	uint8 requestor_id;
	/* Service Control Bitmask. Also determines what data follows. */
	uint8 svc_control;
	/* Optional fields follow */
} BWL_POST_PACKED_STRUCT wifi_nan_svc_descriptor_attr_t;

/* IBSS attribute */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_ibss_attr_s {
	/* Attribute ID - 0x07. */
	uint8 id;
	/* Length of the following fields in the attribute */
	uint16 len;
	/* BSSID of the ibss */
	struct ether_addr bssid;
	/*
	 map control:, bits:
	[0-3]: Id for associated further avail map attribute
	[4-5]: avail interval duration: 0:16ms; 1:32ms; 2:64ms; 3:reserved
	[6] : repeat : 0 - applies to next DW, 1: 16 intervals max? wtf?
	[7] : reserved
	*/
	uint8 map_ctrl;
	/* avail. intervals bitmap, var len  */
	uint8 avail_bmp[1];
} BWL_POST_PACKED_STRUCT wifi_nan_ibss_attr_t;

/* Further Availability MAP attr  */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_favail_attr_s {
	/* Attribute ID - 0x0A. */
	uint8 id;
	/* Length of the following fields in the attribute */
	uint16 len;
	/* MAP id: val [0..15], values[16-255] reserved */
	uint8 map_id;
	/*  availibility entry, var len */
	uint8 avil_entry[1];
} BWL_POST_PACKED_STRUCT wifi_nan_favail_attr_t;

/* Further Availability MAP attr  */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_avail_entry_s {
	/*
	 entry control
	 [0-1]: avail interval duration: 0:16ms; 1:32ms; 2:64ms;
	 [2:7] reserved
	*/
	uint8 entry_ctrl;
	/* operating class: freq band etc IEEE 802.11 */
	uint8 opclass;
	/* channel number */
	uint8 chan;
	/*  avail bmp, var len */
	uint8 avail_bmp[1];
} BWL_POST_PACKED_STRUCT wifi_nan_avail_entry_t;

/* Map control Field */
#define NAN_MAPCTRL_IDMASK	0x7
#define NAN_MAPCTRL_DURSHIFT	4
#define NAN_MAPCTRL_DURMASK	0x30
#define NAN_MAPCTRL_REPEAT	0x40
#define NAN_MAPCTRL_REPEATSHIFT	6

#define NAN_VENDOR_TYPE_RTT	0
#define NAN_VENDOR_TYPE_P2P	1

/* Vendor Specific Attribute */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_vendor_attr_s {
	uint8	id;			/* 0xDD */
	uint16	len;		/* IE length */
	uint8	oui[DOT11_OUI_LEN]; /* 00-90-4C */
	uint8	type;		/* attribute type */
	uint8	attr[1];	/* var len attributes */
} BWL_POST_PACKED_STRUCT wifi_nan_vendor_attr_t;

#define NAN_VENDOR_HDR_SIZE	(OFFSETOF(wifi_nan_vendor_attr_t, attr))

/* p2p operation attribute */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_p2p_op_attr_s {
	/* Attribute ID - 0x06. */
	uint8 id;
	/* Length of the following fields in the attribute */
	uint16 len;
	/* P2P device role */
	uint8 dev_role;
	/* BSSID of the ibss */
	struct ether_addr p2p_dev_addr;
	/*
	map control:, bits:
	[0-3]: Id for associated further avail map attribute
	[4-5]: avail interval duration: 0:16ms; 1:32ms; 2:64ms; 3:reserved
	[6] : repeat : 0 - applies to next DW, 1: 16 intervals max? wtf?
	[7] : reserved
	*/
	uint8 map_ctrl;
	/* avail. intervals bitmap */
	uint8 avail_bmp[1];
} BWL_POST_PACKED_STRUCT wifi_nan_p2p_op_attr_t;

/* ranging attribute */
#define NAN_RANGING_MAP_CTRL_ID_SHIFT 0
#define NAN_RANGING_MAP_CTRL_ID_MASK 0x0F
#define NAN_RANGING_MAP_CTRL_DUR_SHIFT 4
#define NAN_RANGING_MAP_CTRL_DUR_MASK 0x30
#define NAN_RANGING_MAP_CTRL_REPEAT_SHIFT 6
#define NAN_RANGING_MAP_CTRL_REPEAT_MASK 0x40
#define NAN_RANGING_MAP_CTRL_REPEAT_DW(_ctrl) (((_ctrl) & \
	NAN_RANGING_MAP_CTRL_DUR_MASK) ? 16 : 1)
#define NAN_RANGING_MAP_CTRL(_id, _dur, _repeat) (\
	(((_id) << NAN_RANGING_MAP_CTRL_ID_SHIFT) & \
		 NAN_RANGING_MAP_CTRL_ID_MASK) | \
	(((_dur) << NAN_RANGING_MAP_CTRL_DUR_SHIFT) & \
		NAN_RANGING_MAP_CTRL_DUR_MASK) | \
	(((_repeat) << NAN_RANGING_MAP_CTRL_REPEAT_SHIFT) & \
		 NAN_RANGING_MAP_CTRL_REPEAT_MASK))

enum {
	NAN_RANGING_PROTO_FTM = 0
};

typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_ranging_attr_s {
	uint8 id;			/* 0x0C */
	uint16 len;			/* length that follows */
	struct ether_addr dev_addr;	/* device mac address */

	/*
	map control:, bits:
	[0-3]: Id for associated further avail map attribute
	[4-5]: avail interval duration: 0:16ms; 1:32ms; 2:64ms; 3:reserved
	[6] : repeat : 0 - applies to next DW, 1: 16 intervals max? wtf?
	[7] : reserved
	*/
	uint8 map_ctrl;

	uint8 protocol;					/* FTM = 0 */
	uint32 avail_bmp;				/* avail interval bitmap */
} BWL_POST_PACKED_STRUCT wifi_nan_ranging_attr_t;

#define NAN_CONN_CAPABILITY_WFD		0x0001
#define NAN_CONN_CAPABILITY_WFDS	0x0002
#define NAN_CONN_CAPABILITY_TDLS	0x0004
#define NAN_CONN_CAPABILITY_INFRA	0x0008
#define NAN_CONN_CAPABILITY_IBSS	0x0010
#define NAN_CONN_CAPABILITY_MESH	0x0020

typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_conn_cap_attr_s {
	/* Attribute ID - 0x04. */
	uint8 id;
	/* Length of the following fields in the attribute */
	uint16	len;
	uint16	conn_cap_bmp;	/* Connection capability bitmap */
} BWL_POST_PACKED_STRUCT wifi_nan_conn_cap_attr_t;

#define NAN_SLOT_RES_16TU 16
#define NAN_SLOT_RES_32TU 32
#define NAN_SLOT_RES_64TU 64

/* NAN 2.0 (section 5.7.18.2): NAN availability attribute */

/* NAN Availability Attribute */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_availability_attr_s {
	uint8 id;				/* TBD */
	uint16 len;				/* length that follows */
	uint8 attr_cntrl[3];			/* attribute control */
	uint8 avail_entry_list[1];		/* availability entry list */
} BWL_POST_PACKED_STRUCT wifi_nan_availability_attr_t;

/* Attribute Control field */
#define NAN_ATTR_CNTRL_MAP_ID_MASK	0x0F	/* Map Id */
#define NAN_ATTR_CNTRL_RSVD_MASK	0xF0	/* Reserved */
#define NAN_ATTR_CNTRL_SEQ_ID_MASK	0xFF	/* Seq Id */

/* NAN Element container Attribute */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_container_attr_s {
	uint8 id;	/* id - 0x20 */
	uint16 len;	/* Total length of following IEs */
	uint8 data[1];	/* Data pointing to one or more IEs */
} BWL_POST_PACKED_STRUCT wifi_nan_container_attr_t;

/* Availability Entry format */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_avail_entry_attr_s {
	uint16 len;		/* Length */
	uint32 entry_cntrl;	/* Entry Control */
	uint8 var[1];	/* Time Bitmap & Channel Entry List */
} BWL_POST_PACKED_STRUCT wifi_nan_avail_entry_attr_t;

/*
 * NDL Attribute WFA Tech. Spec ver 1.0.r12 (section 10.7.19.2)
 */
#define NDL_ATTR_IM_SCHED_CTRL_SCHED_IND_MASK 0x1E
#define NDL_ATTR_IM_SCHED_CTRL_SCHED_IND_SHIFT	1
#define NDL_ATTR_IM_SCHED_CTRL_TIME_BMP_PRSNT_MASK	0x20
#define NDL_ATTR_IM_SCHED_CTRL_TIME_BMP_PRSNT_SHIFT	5

typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_ndl_im_sched_ctrl_s {
	uint8 sched_ctrl;		/* Schedule control */
	uint8 bitmap_ctrl[3];
	uint8 time_bitmap[1];
} BWL_POST_PACKED_STRUCT wifi_nan_ndl_im_sched_ctrl_t;

/*
 * NDL Control field - Table xx
 */
#define NDL_ATTR_CTRL_PEER_ID_PRESENT_MASK		0x01
#define NDL_ATTR_CTRL_PEER_ID_PRESENT_SHIFT		0
#define NDL_ATTR_CTRL_IM_SCHED_PRESENT_MASK		0x02
#define NDL_ATTR_CTRL_IM_SCHED_PRESENT_SHIFT	1
#define NDL_ATTR_CTRL_NDC_ATTR_PRESENT_MASK		0x04
#define NDL_ATTR_CTRL_NDC_ATTR_PRESENT_SHIFT	2
#define NDL_ATTR_CTRL_QOS_ATTR_PRESENT_MASK		0x08
#define NDL_ATTR_CTRL_QOS_ATTR_PRESENT_SHIFT	3

#define NDL_ATTR_TYPE_STATUS_REQUEST	0x01
#define NDL_ATTR_TYPE_STATUS_RESPONSE	0x02
#define NDL_ATTR_TYPE_STATUS_CONFIRM	0x04
#define NDL_ATTR_TYPE_STATUS_CONTINUED	0x10
#define NDL_ATTR_TYPE_STATUS_ACCEPTED	0x20
#define NDL_ATTR_TYPE_STATUS_REJECTED	0x40

#define NDL_ATTR_CTRL_NONE				0
#define NDL_ATTR_CTRL_PEER_ID_PRESENT	(1 << NDL_ATTR_CTRL_PEER_ID_PRESENT_SHIFT)
#define NDL_ATTR_CTRL_IMSCHED_PRESENT	(1 << NDL_ATTR_CTRL_IM_SCHED_PRESENT_SHIFT)
#define NDL_ATTR_CTRL_NDC_PRESENT		(1 << NDL_ATTR_CTRL_NDC_ATTR_PRESENT_SHIFT)
#define NDL_ATTR_CTRL_NDL_QOS_PRESENT	(1 << NDL_ATTR_CTRL_QOS_ATTR_PRESENT_SHIFT)

typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_ndl_attr_s {
	uint8 id;		/* NAN_ATTR_NAN_NDL = 0x17 */
	uint16 len;		/* Length of the fields in the attribute */
	uint8 dialog_token;	/* Identify req and resp */
	uint8 type_status;		/* Bits[3-0] type subfield, Bits[7-4] status subfield */
	uint8 reason;		/* Identifies reject reason */
	uint16 ndl_ctrl;	/* NDL control field ?? spec only defines 8 bits */
	uint8 ndl_peer_id;	/* optional - to be used for paging */
	uint8 imm_sched[1];	/* Optional field immutable schedule */
} BWL_POST_PACKED_STRUCT wifi_nan_ndl_attr_t;

/*
 * NDL QoS Attribute  WFA Tech. Spec ver 1.0.r12 (section 10.7.19.4)
 */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_ndl_qos_attr_s {
	uint8 id;		/* NAN_ATTR_NAN_NDL_QOS = 24 */
	uint16 len;		/* Length of the attribute field following */
	uint8 min_slots;	/* Min. number of FAW slots needed per DW interval */
	uint8 max_latency[3];	/* Max allowed time interval between non-cont FAW */
} BWL_POST_PACKED_STRUCT wifi_nan_ndl_qos_attr_t;

/* Entry Control Field (section 5.7.18.2.2) */

/* Type of  Availability */
#define NAN_ENTRY_CNTRL_TYPE_OF_AVAIL_MASK	0x07
#define NAN_ENTRY_CNTRL_TYPE_OF_AVAIL_SHIFT	0
/* Usage Preference */
#define NAN_ENTRY_CNTRL_USAGE_PREF_MASK		0x18
#define NAN_ENTRY_CNTRL_USAGE_PREF_SHIFT	3
/* Utilization */
#define NAN_ENTRY_CNTRL_UTIL_MASK		0x1E0
#define NAN_ENTRY_CNTRL_UTIL_SHIFT		5
/* Rx Nss */
#define NAN_ENTRY_CNTRL_RX_NSS_MASK		0x1E00
#define NAN_ENTRY_CNTRL_RX_NSS_SHIFT		9
/* Paged Resource block */
#define NAN_ENTRY_CNTRL_PAGED_RSC_BLK_MASK	0x2000
#define NAN_ENTRY_CNTRL_PAGED_RSC_BLK_SHIFT	13
/* Time Bitmap Present */
#define NAN_ENTRY_CNTRL_TIME_BMP_PRSNT_MASK	0x4000
#define NAN_ENTRY_CNTRL_TIME_BMP_PRSNT_SHIFT	14
/* Channel Entry Present */
#define NAN_ENTRY_CNTRL_CHAN_ENTRY_PRSNT_MASK	0x8000
#define NAN_ENTRY_CNTRL_CHAN_ENTRY_PRSNT_SHIFT	15
/* Reserved */
#define NAN_ENTRY_CNTRL_RESERVED_MASK		0xFF0000
#define NAN_ENTRY_CNTRL_RESERVED_SHIFT		16

/* Type of  Availability: committed */
#define NAN_ENTRY_CNTRL_TYPE_COMM_AVAIL	        0x1
/* Type of  Availability: potential */
#define NAN_ENTRY_CNTRL_TYPE_POTEN_AVAIL	0x2
/* Type of  Availability: conditional */
#define NAN_ENTRY_CNTRL_TYPE_COND_AVAIL	        0x4

/* Type of  Availability: committed */
#define NAN_ENTRY_CNTRL_TYPE_COMM_AVAIL_MASK	0x1
/* Type of  Availability: potential */
#define NAN_ENTRY_CNTRL_TYPE_POTEN_AVAIL_MASK	0x2
/* Type of  Availability: conditional */
#define NAN_ENTRY_CNTRL_TYPE_COND_AVAIL_MASK	0x4


/* Time Bitmap Control field (section 5.7.18.2.3) */

/* Reserved */
#define NAN_TIME_BMP_CNTRL_RSVD_MASK	0x01
#define NAN_TIME_BMP_CNTRL_RSVD_SHIFT	0
/* Bitmap Len */
#define NAN_TIME_BMP_CNTRL_BMP_LEN_MASK	0x7E
#define NAN_TIME_BMP_CNTRL_BMP_LEN_SHIFT 1
/* Bit Duration */
#define NAN_TIME_BMP_CNTRL_BIT_DUR_MASK	0x380
#define NAN_TIME_BMP_CNTRL_BIT_DUR_SHIFT	7
/* Bitmap Len */
#define NAN_TIME_BMP_CNTRL_PERIOD_MASK	0x1C00
#define NAN_TIME_BMP_CNTRL_PERIOD_SHIFT	10
/* Start Offset */
#define NAN_TIME_BMP_CNTRL_START_OFFSET_MASK	0x3FE000
#define NAN_TIME_BMP_CNTRL_START_OFFSET_SHIFT	13
/* Reserved */
#define NAN_TIME_BMP_CNTRL_RESERVED_MASK	0xC00000
#define NAN_TIME_BMP_CNTRL_RESERVED_SHIFT	22

/* Time Bitmap Control field: Bit Duration */
typedef enum
{
	NAN_TIME_BMP_CTRL_BIT_DUR_DUR_16TU = 0,
	NAN_TIME_BMP_CTRL_BIT_DUR_DUR_32TU = 1,
	NAN_TIME_BMP_CTRL_BIT_DUR_DUR_48TU = 2,
	NAN_TIME_BMP_CTRL_BIT_DUR_DUR_64TU = 3,
	NAN_TIME_BMP_CTRL_BIT_DUR_DUR_80TU = 4,
	NAN_TIME_BMP_CTRL_BIT_DUR_DUR_96TU = 5,
	NAN_TIME_BMP_CTRL_BIT_DUR_DUR_112TU = 6,
	NAN_TIME_BMP_CTRL_BIT_DUR_DUR_128TU = 7
} nan_time_bmp_ctrl_bit_dur_t;

/* Time Bitmap Control field: Period */
typedef enum
{
	NAN_TIME_BMP_CTRL_PERIOD_128TU = 1,
	NAN_TIME_BMP_CTRL_PERIOD_256TU,
	NAN_TIME_BMP_CTRL_PERIOD_512TU,
	NAN_TIME_BMP_CTRL_PERIOD_1024TU,
	NAN_TIME_BMP_CTRL_PERIOD_2048U,
	NAN_TIME_BMP_CTRL_PERIOD_4096U,
	NAN_TIME_BMP_CTRL_PERIOD_8192U
} nan_time_bmp_ctrl_repeat_interval_t;

/* FAC Channel Entry  (section 10.7.19.1.5) */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_fac_chan_entry_s {
	uint8 oper_class;		/* Operating Class */
	uint16 chan_bitmap;		/* Channel Bitmap */
	uint8 primary_chan_bmp;		/* Primary Channel Bitmap */
	uint16 aux_chan;			/* Auxiliary Channel bitmap */
} BWL_POST_PACKED_STRUCT wifi_nan_fac_chan_entry_t;

/* Channel Entries List field (section 5.7.18.2.4) */

/* Type */
#define NAN_CHAN_ENTRY_TYPE_MASK	0x01
#define NAN_CHAN_ENTRY_TYPE_SHIFT	0
/* Channel Entry Length Indication */
#define NAN_CHAN_ENTRY_LEN_IND_MASK	0x02
#define NAN_CHAN_ENTRY_LEN_IND_SHIFT	1
/* Reserved */
#define NAN_CHAN_ENTRY_RESERVED_MASK	0x0C
#define NAN_CHAN_ENTRY_RESERVED_SHIFT	2
/* Number of FAC Band or Channel Entries  */
#define NAN_CHAN_ENTRY_NO_OF_CHAN_ENTRY_MASK	0xF0
#define NAN_CHAN_ENTRY_NO_OF_CHAN_ENTRY_SHIFT	4

#define NAN_CHAN_ENTRY_TYPE_BANDS	0
#define NAN_CHAN_ENTRY_TYPE_OPCLASS_CHANS	1

#define NAN_CHAN_ENTRY_BW_LT_80MHZ	0
#define NAN_CHAN_ENTRY_BW_EQ_160MHZ	1

/* Device Capability Attribute (section 5.7.17.4) */

typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_dev_cap_s {
	uint8 id;		/* TBD */
	uint16 len;		/* Length */
	uint8 awake_dw_info;	/* Awake DW Info */
	uint8 bands_supported;	/* Supported Bands */
	uint8 op_mode;		/* Operation Mode */
} BWL_POST_PACKED_STRUCT wifi_nan_dev_cap_t;

/* Awake DW Info field format */

/* 2.4GHz DW */
#define NAN_DEV_CAP_AWAKE_DW_2G_MASK	0x07
/* 5GHz DW */
#define NAN_DEV_CAP_AWAKE_DW_5G_MASK	0x38
/* Reserved */
#define NAN_DEV_CAP_AWAKE_DW_RSVD_MASK	0xC0

/* bit shift for dev cap */
#define NAN_DEV_CAP_AWAKE_DW_2G_SHIFT	0
#define NAN_DEV_CAP_AWAKE_DW_5G_SHIFT	3


/* Device Capability Attribute Format */

/* Operation Mode: HT */
#define NAN_DEV_CAP_HT_OPER_MODE_MASK	0x01
/* Operation Mode: VHT */
#define NAN_DEV_CAP_VHT_OPER_MODE_MASK	0x02

/* Committed DW Info field format */
/* 2.4GHz DW */
#define NAN_DEV_CAP_COMMIT_DW_2G_MASK	0x07
#define NAN_DEV_CAP_COMMIT_DW_2G_OVERWRITE_MASK	0x3C0
/* 5GHz DW */
#define NAN_DEV_CAP_COMMIT_DW_5G_MASK	0x38
#define NAN_DEV_CAP_COMMIT_DW_5G_OVERWRITE_MASK 0x3C00
/* Reserved */
#define NAN_DEV_CAP_COMMIT_DW_RSVD_MASK	0xC000
/* bit shift for dev cap */
#define NAN_DEV_CAP_COMMIT_DW_2G_SHIFT	0
#define NAN_DEV_CAP_COMMIT_DW_5G_SHIFT	3
#define NAN_DEV_CAP_COMMIT_DW_2G_OVERWRITE_SHIFT	6
#define NAN_DEV_CAP_COMMIT_DW_5G_OVERWRITE_SHIFT	10
/* Operation Mode */
#define NAN_DEV_CAP_OP_MODE_VHT_MASK		0x01
#define NAN_DEV_CAP_OP_MODE_VHT8080_MASK	0x02
#define NAN_DEV_CAP_OP_MODE_VHT160_MASK		0x04
/* Band IDs */
enum {
	NAN_BAND_ID_TVWS		= 0,
	NAN_BAND_ID_SIG			= 1,
	NAN_BAND_ID_2G			= 2,
	NAN_BAND_ID_5G			= 4,
	NAN_BAND_ID_60G			= 5
};
typedef uint8 nan_band_id_t;

/*
 * Unaligned schedule attribute section 10.7.19.6 spec. ver r15
 */
#define NAN_UAW_ATTR_CTRL_SCHED_ID_MASK 0x000F
#define NAN_UAW_ATTR_CTRL_SCHED_ID_SHIFT 0
#define NAN_UAW_ATTR_CTRL_SEQ_ID_MASK 0xFF00
#define NAN_UAW_ATTR_CTRL_SEQ_ID_SHIFT 8

#define NAN_UAW_OVWR_ALL_MASK 0x01
#define NAN_UAW_OVWR_ALL_SHIFT 0
#define NAN_UAW_OVWR_MAP_ID_MASK 0x1E
#define NAN_UAW_OVWR_MAP_ID_SHIFT 1

#define NAN_UAW_CTRL_TYPE_MASK 0x03
#define NAN_UAW_CTRL_TYPE_SHIFT 0
#define NAN_UAW_CTRL_CHAN_AVAIL_MASK 0x04
#define NAN_UAW_CTRL_CHAN_AVAIL_SHIFT 2
#define NAN_UAW_CTRL_RX_NSS_MASK 0x78
#define NAN_UAW_CTRL_RX_NSS_SHIFT 3

typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_uaw_attr_s {
	uint8 id;
	uint16 len;
	uint16 ctrl;
	uint32 start; /* low 32 bits of tsf */
	uint32 dur;
	uint32 period;
	uint8 count_down;
	uint8 overwrite;
	/*
	 * uaw[0] == optional field UAW control when present.
	 * band ID or channel follows
	 */
	uint8 uaw_entry[1];
} BWL_POST_PACKED_STRUCT wifi_nan_uaw_attr_t;

/* NAN2 Management Frame (section 5.6) */

/* Public action frame for NAN2 */
typedef BWL_PRE_PACKED_STRUCT struct nan2_pub_act_frame_s {
	/* NAN_PUB_AF_CATEGORY 0x04 */
	uint8 category_id;
	/* NAN_PUB_AF_ACTION 0x09 */
	uint8 action_field;
	/* NAN_OUI 0x50-6F-9A */
	uint8 oui[DOT11_OUI_LEN];
	/* NAN_OUI_TYPE TBD */
	uint8 oui_type;
	/* NAN_OUI_SUB_TYPE TBD */
	uint8 oui_sub_type;
	/* NAN_PUB_AF_DIALOG_TOKEN */
	uint8 dialog_token;
	/* One or more NAN Attributes follow */
	uint8 data[1];
} BWL_POST_PACKED_STRUCT nan2_pub_act_frame_t;

#define NAN2_PUB_ACT_FRM_SIZE	(OFFSETOF(nan2_pub_act_frame_t, data))

/* NAN2 Management Frame Subtypes */

/* NAN2 Management */
#define NAN_MGMT_FRM_SUBTYPE_MGMT		0
/* NAN Ranging Request */
#define NAN_MGMT_FRM_SUBTYPE_RANGING_REQ	1
/* NAN Ranging Response */
#define NAN_MGMT_FRM_SUBTYPE_RANGING_RESP	2
/* NAN Ranging Termination */
#define NAN_MGMT_FRM_SUBTYPE_RANGING_TERM	4
/* NAN Ranging Report */
#define NAN_MGMT_FRM_SUBTYPE_RANGING_RPT	5
/* NDP Request */
#define NAN_MGMT_FRM_SUBTYPE_NDP_REQ		6
/* NDP Response */
#define NAN_MGMT_FRM_SUBTYPE_NDP_RESP		7
/* NDP Confirm */
#define NAN_MGMT_FRM_SUBTYPE_NDP_CONFIRM	8
/* NDP Key Installment */
#define NAN_MGMT_FRM_SUBTYPE_NDP_KEY_INST	9
/* Schedule Request */
#define NAN_MGMT_FRM_SUBTYPE_SCHED_REQ		10
/* Schedule Response */
#define NAN_MGMT_FRM_SUBTYPE_SCHED_RESP		11
/* Schedule Confirm */
#define NAN_MGMT_FRM_SUBTYPE_SCHED_CONF		12
/* Schedule Update */
#define NAN_MGMT_FRM_SUBTYPE_SCHED_UPD		13
/* GAS Sechdule Request */
#define NAN_MGMT_FRM_SUBTYPE_GAS_SCHED_REQ	14	/* Not part of spec. ver 1.0.r12 */
/* GAS Sechdule Response */
#define NAN_MGMT_FRM_SUBTYPE_GAS_SCHED_RESP	15	/* Not part of spec. ver 1.0.r12 */

/* NDP End (internal implementation) */
#define NAN_MGMT_FRM_SUBTYPE_NDP_END		16	/* Not part of spec. ver 1.0.r12 */

/* NDL Schedule request */
#define NAN_MGMT_FRM_SUBTYPE_NDL_UPDATE_REQ	17	/* Not part of spec. ver 1.0.r12 */
/* NDL Schedule response */
#define	NAN_MGMT_FRM_SUBTYPE_NDL_UPDATE_RESP	18	/* Not part of spec. ver 1.0.r12 */

/* nan 2.0 qos */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_ndp_qos_s {
	uint8 tid;		/* traffic identifier */
	uint16 pkt_size;	/* service data pkt size */
	uint8 data_rate;	/* mean data rate */
	uint8 svc_interval;	/* max service interval */
} BWL_POST_PACKED_STRUCT wifi_nan_ndp_qos_t;

/* NDP control bitmap defines */
#define NAN_NDP_CTRL_CONFIRM_REQUIRED		0x01
#define NAN_NDP_CTRL_EXPLICIT_CONFIRM		0x02
#define NAN_NDP_CTRL_SECURTIY_PRESENT		0x04
#define NAN_NDP_CTRL_PUB_ID_PRESENT		0x08
#define NAN_NDP_CTRL_RESP_NDI_PRESENT		0x10
#define NAN_NDP_CTRL_SPEC_INFO_PRESENT		0x20
#define NAN_NDP_CTRL_RESERVED			0xA0

/* NDP Information Element (internal) */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_ndp_setup_s {
	uint8 id;		/* 221 */
	uint8 len;		/* Length */
	uint8 oui[DOT11_OUI_LEN];	/* "\x00\x10\x18" BRCM OUI */
	uint8 type;		/* NAN_OUI_TYPE 0x13 */
	uint8 subtype;		/* NAN_DATA_NDP_SETUP */
	uint8 msg_type;		/* NDP Req, NDP Resp etc. */
	uint8 pub_inst_id;	/* publish instance id */
	struct ether_addr peer_mac_addr; /* publisher mac addr (aka peer mgmt address) */
	struct ether_addr data_if_addr;	/* local data i/f address */
	uint8 msg_status;
	uint8 ndp_ctrl;
	uint8 security;
	wifi_nan_ndp_qos_t qos;	/* qos info */
	uint8 var[1];		/* NDP specific info */
} BWL_POST_PACKED_STRUCT wifi_nan_ndp_setup_t;

/* NDP Attribute */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_ndp_attr_s {
	uint8 id;		/* 0x10 */
	uint16 len;		/* length */
	uint8 dialog_token;	/* dialog token */
	uint8 type_status;	/* bits 0-3 type, 4-7 status */
	uint8 reason;		/* reason code */
	struct ether_addr init_ndi;	/* ndp initiator's data interface address */
	uint8 ndp_id;		/* ndp identifier (created by initiator */
	uint8 control;		/* ndp control field */
	uint8 var[1];		/* Optional fields follow */
} BWL_POST_PACKED_STRUCT wifi_nan_ndp_attr_t;
/* NDP attribute type and status macros */
#define NAN_NDP_TYPE_MASK	0x0F
#define NAN_NDP_TYPE_REQUEST	0x0
#define NAN_NDP_TYPE_RESPONSE	0x1
#define NAN_NDP_TYPE_CONFIRM	0x2
#define NAN_NDP_TYPE_SECURITY	0x3
#define NAN_NDP_TYPE_TERMINATE	0x4
#define NAN_NDP_REQUEST(_ndp)	(((_ndp)->type_status & NAN_NDP_TYPE_MASK) == NAN_NDP_TYPE_REQUEST)
#define NAN_NDP_RESPONSE(_ndp)	(((_ndp)->type_status & NAN_NDP_TYPE_MASK) == NAN_NDP_TYPE_RESPONSE)
#define NAN_NDP_CONFIRM(_ndp)	(((_ndp)->type_status & NAN_NDP_TYPE_MASK) == NAN_NDP_TYPE_CONFIRM)
#define NAN_NDP_SECURITY_INST(_ndp)	(((_ndp)->type_status & NAN_NDP_TYPE_MASK) == \
									NAN_NDP_TYPE_SECURITY)
#define NAN_NDP_TERMINATE(_ndp) (((_ndp)->type_status & NAN_NDP_TYPE_MASK) == \
									NAN_NDP_TYPE_TERMINATE)
#define NAN_NDP_STATUS_SHIFT	4
#define NAN_NDP_STATUS_MASK	0xF0
#define NAN_NDP_STATUS_CONT	(0 << NAN_NDP_STATUS_SHIFT)
#define NAN_NDP_STATUS_ACCEPT	(1 << NAN_NDP_STATUS_SHIFT)
#define NAN_NDP_STATUS_REJECT	(2 << NAN_NDP_STATUS_SHIFT)
#define NAN_NDP_CONT(_ndp)	(((_ndp)->type_status & NAN_NDP_TYPE_MASK) == NAN_NDP_STATUS_CONT)
#define NAN_NDP_ACCEPT(_ndp)	(((_ndp)->type_status & NAN_NDP_TYPE_MASK) == NAN_NDP_STATUS_ACCEPT)
#define NAN_NDP_REJECT(_ndp)	(((_ndp)->type_status & NAN_NDP_TYPE_MASK) == NAN_NDP_STATUS_REJECT)
/* NDP Setup Status */
#define NAN_NDP_SETUP_STATUS_OK		1
#define NAN_NDP_SETUP_STATUS_FAIL	0
#define NAN_NDP_SETUP_STATUS_REJECT	2

/* NAN mgmt information element */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_mgmt_setup_s {
	uint8 id;		/* 221 */
	uint8 len;		/* Length */
	uint8 oui[DOT11_OUI_LEN];	/* "\x00\x10\x18" BRCM OUI */
	uint8 type;		/* NAN_OUI_TYPE 0x13 */
	uint8 subtype;		/* NAN_DATA_MGMT_SETUP */
	uint8 msg_type;		/* Mgmt Req, Mgmt Resp etc. */
	uint8 msg_status;
} BWL_POST_PACKED_STRUCT wifi_nan_mgmt_setup_t;

/* NAN Mgmt Request */
#define NAN_MGMT_SETUP_MSG_REQ	1	/* don't use 0 */
/* NAN Mgmt Response */
#define NAN_MGMT_SETUP_MSG_RESP	2

/* NAN Mgmt Setup Status */
#define NAN_MGMT_SETUP_STATUS_OK	0
#define NAN_MGMT_SETUP_STATUS_FAIL	1
#define NAN_MGMT_SETUP_STATUS_REJECT	2

/* nan2 ndc ie */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_ndc_attr_s {
	uint8 id;
	uint16 len;
	uint8 ndc_id[NAN_DATA_NDC_ID_SIZE];
	uint8 sched_cntrl;
	uint8 var[1];
} BWL_POST_PACKED_STRUCT wifi_nan_ndc_attr_t;

/* Schedule Indication */
#define NAN_NDC_ATTR_SCHED_IND_MASK	0x1E
#define NAN_NDC_ATTR_SCHED_IND_SHIFT 1
/* Time Bitmap Present */
#define NAN_NDC_ATTR_TBMP_PRESENT_MASK	0x20
#define NAN_NDC_ATTR_TBMP_PRESENT_SHIFT 5
/* Proposed NDC */
#define NAN_NDC_ATTR_PROPOSED_NDC_MASK	0x40
#define NAN_NDC_ATTR_PROPOSED_NDC_SHIFT 6

/* Service descriptor extension attribute */
typedef BWL_PRE_PACKED_STRUCT struct wifi_nan_svc_descriptor_ext_attr_t {
	/* Attribute ID - 0x11 */
	uint8 id;
	/* Length of the following fields in the attribute */
	uint16 len;
	/* Instance id of associated service descriptor attribute */
	uint8 instance_id;
	/* SDE control field */
	uint16 control;
	/* Range limit - ingress */
	uint16 range_ingress;
	/* Range limit - egress */
	uint16 range_egress;
} BWL_POST_PACKED_STRUCT wifi_nan_svc_descriptor_ext_attr_t;
#define NAN_SDE_ATTR_LEN (sizeof(wifi_nan_svc_descriptor_ext_attr_t))
/* SDEA control field bit definitions and access macros */
#define NAN_SDE_CF_FSD_REQUIRED		(1 << 0)
#define NAN_SDE_CF_FSD_GAS		(1 << 1)
#define NAN_SDE_CF_DP_REQUIRED		(1 << 2)
#define NAN_SDE_CF_DP_TYPE		(1 << 3)
#define NAN_SDE_CF_MULTICAST_TYPE	(1 << 4)
#define NAN_SDE_CF_QOS_REQUIRED		(1 << 5)
#define NAN_SDE_CF_SECURITY_REQUIRED	(1 << 6)
#define NAN_SDE_CF_RANGING_REQUIRED	(1 << 7)
#define NAN_SDE_CF_RANGE_PRESENT	(1 << 8)
#define NAN_SDE_FSD_REQUIRED(_sde)	((_sde)->control & NAN_SDE_CF_FSD_REQUIRED)
#define NAN_SDE_FSD_GAS(_sde)		((_sde)->control & NAN_SDE_CF_FSD_GAS)
#define NAN_SDE_DP_REQUIRED(_sde)	((_sde)->control & NAN_SDE_CF_DP_REQUIRED)
#define NAN_SDE_DP_MULTICAST(_sde)	((_sde)->control & NAN_SDE_CF_DP_TYPE)
#define NAN_SDE_MULTICAST_M_TO_M(_sde)	((_sde)->control & NAN_SDE_CF_MULTICAST_TYPE)
#define NAN_SDE_QOS_REQUIRED(_sde)	((_sde)->control & NAN_SDE_CF_QOS_REQUIRED)
#define NAN_SDE_SECURITY_REQUIRED(_sde)	((_sde)->control & NAN_SDE_CF_SECURITY_REQUIRED)
#define NAN_SDE_RANGING_REQUIRED(_sde)	((_sde)->control & NAN_SDE_CF_RANGING_REQUIRED)
#define NAN_SDE_RANGE_PRESENT(_sde)	((_sde)->control & NAN_SDE_CF_RANGE_PRESENT)


/* This marks the end of a packed structure section. */
#include <packed_section_end.h>

#endif /* _NAN_H_ */
