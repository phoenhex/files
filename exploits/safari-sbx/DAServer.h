#ifndef	_DAServer_user_
#define	_DAServer_user_

/* Module DAServer */

#include <string.h>
#include <mach/ndr.h>
#include <mach/boolean.h>
#include <mach/kern_return.h>
#include <mach/notify.h>
#include <mach/mach_types.h>
#include <mach/message.h>
#include <mach/mig_errors.h>
#include <mach/port.h>
	
/* BEGIN VOUCHER CODE */

#ifndef KERNEL
#if defined(__has_include)
#if __has_include(<mach/mig_voucher_support.h>)
#ifndef USING_VOUCHERS
#define USING_VOUCHERS
#endif
#ifndef __VOUCHER_FORWARD_TYPE_DECLS__
#define __VOUCHER_FORWARD_TYPE_DECLS__
#ifdef __cplusplus
extern "C" {
#endif
	extern boolean_t voucher_mach_msg_set(mach_msg_header_t *msg) __attribute__((weak_import));
#ifdef __cplusplus
}
#endif
#endif // __VOUCHER_FORWARD_TYPE_DECLS__
#endif // __has_include(<mach/mach_voucher_types.h>)
#endif // __has_include
#endif // !KERNEL
	
/* END VOUCHER CODE */

	
/* BEGIN MIG_STRNCPY_ZEROFILL CODE */

#if defined(__has_include)
#if __has_include(<mach/mig_strncpy_zerofill_support.h>)
#ifndef USING_MIG_STRNCPY_ZEROFILL
#define USING_MIG_STRNCPY_ZEROFILL
#endif
#ifndef __MIG_STRNCPY_ZEROFILL_FORWARD_TYPE_DECLS__
#define __MIG_STRNCPY_ZEROFILL_FORWARD_TYPE_DECLS__
#ifdef __cplusplus
extern "C" {
#endif
	extern int mig_strncpy_zerofill(char *dest, const char *src, int len) __attribute__((weak_import));
#ifdef __cplusplus
}
#endif
#endif /* __MIG_STRNCPY_ZEROFILL_FORWARD_TYPE_DECLS__ */
#endif /* __has_include(<mach/mig_strncpy_zerofill_support.h>) */
#endif /* __has_include */
	
/* END MIG_STRNCPY_ZEROFILL CODE */


#ifdef AUTOTEST
#ifndef FUNCTION_PTR_T
#define FUNCTION_PTR_T
typedef void (*function_ptr_t)(mach_port_t, char *, mach_msg_type_number_t);
typedef struct {
        char            *name;
        function_ptr_t  function;
} function_table_entry;
typedef function_table_entry   *function_table_t;
#endif /* FUNCTION_PTR_T */
#endif /* AUTOTEST */

#ifndef	DAServer_MSG_COUNT
#define	DAServer_MSG_COUNT	18
#endif	/* DAServer_MSG_COUNT */

#include <mach/std_types.h>
#include <mach/mig.h>
#include <mach/mig.h>
#include <mach/mach_types.h>
#include <mach/mach.h>
#include <sys/types.h>
#include <Security/Authorization.h>
#include "DAServer.defs.h"

#ifdef __BeforeMigUserHeader
__BeforeMigUserHeader
#endif /* __BeforeMigUserHeader */

#include <sys/cdefs.h>
__BEGIN_DECLS


/* Routine _DAServerDiskCopyDescription */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServerDiskCopyDescription
(
	mach_port_t _session,
	caddr_t _disk,
	vm_address_t *_description,
	mach_msg_type_number_t *_descriptionCnt
);

/* Routine _DAServerDiskGetOptions */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServerDiskGetOptions
(
	mach_port_t _session,
	caddr_t _disk,
	int32_t *_options
);

/* Routine _DAServerDiskGetUserUID */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServerDiskGetUserUID
(
	mach_port_t _session,
	caddr_t _disk,
	uid_t *_userUID
);

/* Routine _DAServerDiskIsClaimed */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServerDiskIsClaimed
(
	mach_port_t _session,
	caddr_t _disk,
	boolean_t *_claimed
);

/* Routine _DAServerDiskSetAdoption */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServerDiskSetAdoption
(
	mach_port_t _session,
	caddr_t _disk,
	boolean_t _adoption
);

/* Routine _DAServerDiskSetEncoding */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServerDiskSetEncoding
(
	mach_port_t _session,
	caddr_t _disk,
	int32_t _encoding
);

/* Routine _DAServerDiskSetOptions */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServerDiskSetOptions
(
	mach_port_t _session,
	caddr_t _disk,
	int32_t _options,
	int32_t _value
);

/* SimpleRoutine _DAServerDiskUnclaim */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServerDiskUnclaim
(
	mach_port_t _session,
	caddr_t _disk
);

/* Routine _DAServerSessionCopyCallbackQueue */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServerSessionCopyCallbackQueue
(
	mach_port_t _session,
	vm_address_t *_queue,
	mach_msg_type_number_t *_queueCnt
);

/* Routine _DAServerSessionCreate */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServerSessionCreate
(
	mach_port_t _session,
	caddr_t _name,
	pid_t _pid,
	mach_port_t *_server
);

/* Routine _DAServerSessionQueueRequest */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServerSessionQueueRequest
(
	mach_port_t _session,
	int32_t _kind,
	caddr_t _argument0,
	int32_t _argument1,
	vm_address_t _argument2,
	mach_msg_type_number_t _argument2Cnt,
	vm_address_t _argument3,
	mach_msg_type_number_t _argument3Cnt,
	mach_vm_offset_t _address,
	mach_vm_offset_t _context
);

/* SimpleRoutine _DAServerSessionQueueResponse */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServerSessionQueueResponse
(
	mach_port_t _session,
	mach_vm_offset_t _address,
	mach_vm_offset_t _context,
	int32_t _kind,
	caddr_t _disk,
	vm_address_t _response,
	mach_msg_type_number_t _responseCnt,
	int32_t _responseID
);

/* Routine _DAServerSessionRegisterCallback */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServerSessionRegisterCallback
(
	mach_port_t _session,
	mach_vm_offset_t _address,
	mach_vm_offset_t _context,
	int32_t _kind,
	int32_t _order,
	vm_address_t _match,
	mach_msg_type_number_t _matchCnt,
	vm_address_t _watch,
	mach_msg_type_number_t _watchCnt
);

/* SimpleRoutine _DAServerSessionRelease */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServerSessionRelease
(
	mach_port_t _session
);

/* SimpleRoutine _DAServerSessionSetAuthorization */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServerSessionSetAuthorization
(
	mach_port_t _session,
	AuthorizationExternalForm _authorization
);

/* SimpleRoutine _DAServerSessionSetClientPort */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServerSessionSetClientPort
(
	mach_port_t _session,
	mach_port_t _client
);

/* SimpleRoutine _DAServerSessionUnregisterCallback */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServerSessionUnregisterCallback
(
	mach_port_t _session,
	mach_vm_offset_t _address,
	mach_vm_offset_t _context
);

/* Routine _DAServermkdir */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t _DAServermkdir
(
	mach_port_t _session,
	___path_t _path
);

__END_DECLS

/********************** Caution **************************/
/* The following data types should be used to calculate  */
/* maximum message sizes only. The actual message may be */
/* smaller, and the position of the arguments within the */
/* message layout may vary from what is presented here.  */
/* For example, if any of the arguments are variable-    */
/* sized, and less than the maximum is sent, the data    */
/* will be packed tight in the actual message to reduce  */
/* the presence of holes.                                */
/********************** Caution **************************/

/* typedefs for all requests */

#ifndef __Request__DAServer_subsystem__defined
#define __Request__DAServer_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		mach_msg_type_number_t _diskOffset; /* MiG doesn't use it */
		mach_msg_type_number_t _diskCnt;
		char _disk[1024];
	} __Request___DAServerDiskCopyDescription_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		mach_msg_type_number_t _diskOffset; /* MiG doesn't use it */
		mach_msg_type_number_t _diskCnt;
		char _disk[1024];
	} __Request___DAServerDiskGetOptions_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		mach_msg_type_number_t _diskOffset; /* MiG doesn't use it */
		mach_msg_type_number_t _diskCnt;
		char _disk[1024];
	} __Request___DAServerDiskGetUserUID_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		mach_msg_type_number_t _diskOffset; /* MiG doesn't use it */
		mach_msg_type_number_t _diskCnt;
		char _disk[1024];
	} __Request___DAServerDiskIsClaimed_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		mach_msg_type_number_t _diskOffset; /* MiG doesn't use it */
		mach_msg_type_number_t _diskCnt;
		char _disk[1024];
		boolean_t _adoption;
	} __Request___DAServerDiskSetAdoption_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		mach_msg_type_number_t _diskOffset; /* MiG doesn't use it */
		mach_msg_type_number_t _diskCnt;
		char _disk[1024];
		int32_t _encoding;
	} __Request___DAServerDiskSetEncoding_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		mach_msg_type_number_t _diskOffset; /* MiG doesn't use it */
		mach_msg_type_number_t _diskCnt;
		char _disk[1024];
		int32_t _options;
		int32_t _value;
	} __Request___DAServerDiskSetOptions_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		mach_msg_type_number_t _diskOffset; /* MiG doesn't use it */
		mach_msg_type_number_t _diskCnt;
		char _disk[1024];
	} __Request___DAServerDiskUnclaim_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
	} __Request___DAServerSessionCopyCallbackQueue_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		mach_msg_type_number_t _nameOffset; /* MiG doesn't use it */
		mach_msg_type_number_t _nameCnt;
		char _name[1024];
		pid_t _pid;
	} __Request___DAServerSessionCreate_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t _argument2;
		mach_msg_ool_descriptor_t _argument3;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int32_t _kind;
		mach_msg_type_number_t _argument0Offset; /* MiG doesn't use it */
		mach_msg_type_number_t _argument0Cnt;
		char _argument0[1024];
		int32_t _argument1;
		mach_msg_type_number_t _argument2Cnt;
		mach_msg_type_number_t _argument3Cnt;
		mach_vm_offset_t _address;
		mach_vm_offset_t _context;
	} __Request___DAServerSessionQueueRequest_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t _response;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_vm_offset_t _address;
		mach_vm_offset_t _context;
		int32_t _kind;
		mach_msg_type_number_t _diskOffset; /* MiG doesn't use it */
		mach_msg_type_number_t _diskCnt;
		char _disk[1024];
		mach_msg_type_number_t _responseCnt;
		int32_t _responseID;
	} __Request___DAServerSessionQueueResponse_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t _match;
		mach_msg_ool_descriptor_t _watch;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_vm_offset_t _address;
		mach_vm_offset_t _context;
		int32_t _kind;
		int32_t _order;
		mach_msg_type_number_t _matchCnt;
		mach_msg_type_number_t _watchCnt;
	} __Request___DAServerSessionRegisterCallback_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
	} __Request___DAServerSessionRelease_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		AuthorizationExternalForm _authorization;
	} __Request___DAServerSessionSetAuthorization_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_port_descriptor_t _client;
		/* end of the kernel processed data */
	} __Request___DAServerSessionSetClientPort_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		mach_vm_offset_t _address;
		mach_vm_offset_t _context;
	} __Request___DAServerSessionUnregisterCallback_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		mach_msg_type_number_t _pathOffset; /* MiG doesn't use it */
		mach_msg_type_number_t _pathCnt;
		char _path[4096];
	} __Request___DAServermkdir_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif
#endif /* !__Request__DAServer_subsystem__defined */

/* union of all requests */

#ifndef __RequestUnion__DAServer_subsystem__defined
#define __RequestUnion__DAServer_subsystem__defined
union __RequestUnion__DAServer_subsystem {
	__Request___DAServerDiskCopyDescription_t Request__DAServerDiskCopyDescription;
	__Request___DAServerDiskGetOptions_t Request__DAServerDiskGetOptions;
	__Request___DAServerDiskGetUserUID_t Request__DAServerDiskGetUserUID;
	__Request___DAServerDiskIsClaimed_t Request__DAServerDiskIsClaimed;
	__Request___DAServerDiskSetAdoption_t Request__DAServerDiskSetAdoption;
	__Request___DAServerDiskSetEncoding_t Request__DAServerDiskSetEncoding;
	__Request___DAServerDiskSetOptions_t Request__DAServerDiskSetOptions;
	__Request___DAServerDiskUnclaim_t Request__DAServerDiskUnclaim;
	__Request___DAServerSessionCopyCallbackQueue_t Request__DAServerSessionCopyCallbackQueue;
	__Request___DAServerSessionCreate_t Request__DAServerSessionCreate;
	__Request___DAServerSessionQueueRequest_t Request__DAServerSessionQueueRequest;
	__Request___DAServerSessionQueueResponse_t Request__DAServerSessionQueueResponse;
	__Request___DAServerSessionRegisterCallback_t Request__DAServerSessionRegisterCallback;
	__Request___DAServerSessionRelease_t Request__DAServerSessionRelease;
	__Request___DAServerSessionSetAuthorization_t Request__DAServerSessionSetAuthorization;
	__Request___DAServerSessionSetClientPort_t Request__DAServerSessionSetClientPort;
	__Request___DAServerSessionUnregisterCallback_t Request__DAServerSessionUnregisterCallback;
	__Request___DAServermkdir_t Request__DAServermkdir;
};
#endif /* !__RequestUnion__DAServer_subsystem__defined */
/* typedefs for all replies */

#ifndef __Reply__DAServer_subsystem__defined
#define __Reply__DAServer_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t _description;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t _descriptionCnt;
	} __Reply___DAServerDiskCopyDescription_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int32_t _options;
	} __Reply___DAServerDiskGetOptions_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		uid_t _userUID;
	} __Reply___DAServerDiskGetUserUID_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		boolean_t _claimed;
	} __Reply___DAServerDiskIsClaimed_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___DAServerDiskSetAdoption_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___DAServerDiskSetEncoding_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___DAServerDiskSetOptions_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___DAServerDiskUnclaim_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t _queue;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t _queueCnt;
	} __Reply___DAServerSessionCopyCallbackQueue_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_port_descriptor_t _server;
		/* end of the kernel processed data */
	} __Reply___DAServerSessionCreate_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___DAServerSessionQueueRequest_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___DAServerSessionQueueResponse_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___DAServerSessionRegisterCallback_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___DAServerSessionRelease_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___DAServerSessionSetAuthorization_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___DAServerSessionSetClientPort_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___DAServerSessionUnregisterCallback_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___DAServermkdir_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack()
#endif
#endif /* !__Reply__DAServer_subsystem__defined */

/* union of all replies */

#ifndef __ReplyUnion__DAServer_subsystem__defined
#define __ReplyUnion__DAServer_subsystem__defined
union __ReplyUnion__DAServer_subsystem {
	__Reply___DAServerDiskCopyDescription_t Reply__DAServerDiskCopyDescription;
	__Reply___DAServerDiskGetOptions_t Reply__DAServerDiskGetOptions;
	__Reply___DAServerDiskGetUserUID_t Reply__DAServerDiskGetUserUID;
	__Reply___DAServerDiskIsClaimed_t Reply__DAServerDiskIsClaimed;
	__Reply___DAServerDiskSetAdoption_t Reply__DAServerDiskSetAdoption;
	__Reply___DAServerDiskSetEncoding_t Reply__DAServerDiskSetEncoding;
	__Reply___DAServerDiskSetOptions_t Reply__DAServerDiskSetOptions;
	__Reply___DAServerDiskUnclaim_t Reply__DAServerDiskUnclaim;
	__Reply___DAServerSessionCopyCallbackQueue_t Reply__DAServerSessionCopyCallbackQueue;
	__Reply___DAServerSessionCreate_t Reply__DAServerSessionCreate;
	__Reply___DAServerSessionQueueRequest_t Reply__DAServerSessionQueueRequest;
	__Reply___DAServerSessionQueueResponse_t Reply__DAServerSessionQueueResponse;
	__Reply___DAServerSessionRegisterCallback_t Reply__DAServerSessionRegisterCallback;
	__Reply___DAServerSessionRelease_t Reply__DAServerSessionRelease;
	__Reply___DAServerSessionSetAuthorization_t Reply__DAServerSessionSetAuthorization;
	__Reply___DAServerSessionSetClientPort_t Reply__DAServerSessionSetClientPort;
	__Reply___DAServerSessionUnregisterCallback_t Reply__DAServerSessionUnregisterCallback;
	__Reply___DAServermkdir_t Reply__DAServermkdir;
};
#endif /* !__RequestUnion__DAServer_subsystem__defined */

#ifndef subsystem_to_name_map_DAServer
#define subsystem_to_name_map_DAServer \
    { "_DAServerDiskCopyDescription", 0 },\
    { "_DAServerDiskGetOptions", 1 },\
    { "_DAServerDiskGetUserUID", 2 },\
    { "_DAServerDiskIsClaimed", 3 },\
    { "_DAServerDiskSetAdoption", 4 },\
    { "_DAServerDiskSetEncoding", 5 },\
    { "_DAServerDiskSetOptions", 6 },\
    { "_DAServerDiskUnclaim", 7 },\
    { "_DAServerSessionCopyCallbackQueue", 8 },\
    { "_DAServerSessionCreate", 9 },\
    { "_DAServerSessionQueueRequest", 10 },\
    { "_DAServerSessionQueueResponse", 11 },\
    { "_DAServerSessionRegisterCallback", 12 },\
    { "_DAServerSessionRelease", 13 },\
    { "_DAServerSessionSetAuthorization", 14 },\
    { "_DAServerSessionSetClientPort", 15 },\
    { "_DAServerSessionUnregisterCallback", 16 },\
    { "_DAServermkdir", 17 }
#endif

#ifdef __AfterMigUserHeader
__AfterMigUserHeader
#endif /* __AfterMigUserHeader */

#endif	 /* _DAServer_user_ */
