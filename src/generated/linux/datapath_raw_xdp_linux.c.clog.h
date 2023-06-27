#ifndef CLOG_DO_NOT_INCLUDE_HEADER
#include <clog.h>
#endif
#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER CLOG_DATAPATH_RAW_XDP_LINUX_C
#undef TRACEPOINT_PROBE_DYNAMIC_LINKAGE
#define  TRACEPOINT_PROBE_DYNAMIC_LINKAGE
#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "datapath_raw_xdp_linux.c.clog.h.lttng.h"
#if !defined(DEF_CLOG_DATAPATH_RAW_XDP_LINUX_C) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define DEF_CLOG_DATAPATH_RAW_XDP_LINUX_C
#include <lttng/tracepoint.h>
#define __int64 __int64_t
#include "datapath_raw_xdp_linux.c.clog.h.lttng.h"
#endif
#include <lttng/tracepoint-event.h>
#ifndef _clog_MACRO_QuicTraceLogVerbose
#define _clog_MACRO_QuicTraceLogVerbose  1
#define QuicTraceLogVerbose(a, ...) _clog_CAT(_clog_ARGN_SELECTOR(__VA_ARGS__), _clog_CAT(_,a(#a, __VA_ARGS__)))
#endif
#ifndef _clog_MACRO_QuicTraceEvent
#define _clog_MACRO_QuicTraceEvent  1
#define QuicTraceEvent(a, ...) _clog_CAT(_clog_ARGN_SELECTOR(__VA_ARGS__), _clog_CAT(_,a(#a, __VA_ARGS__)))
#endif
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------
// Decoder Ring for InterfaceFree
// [ xdp][%p] Freeing Interface
// QuicTraceLogVerbose(
        InterfaceFree,
        "[ xdp][%p] Freeing Interface",
        Interface);
// arg2 = arg2 = Interface = arg2
----------------------------------------------------------*/
#ifndef _clog_3_ARGS_TRACE_InterfaceFree
#define _clog_3_ARGS_TRACE_InterfaceFree(uniqueId, encoded_arg_string, arg2)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, InterfaceFree , arg2);\

#endif




/*----------------------------------------------------------
// Decoder Ring for QueueFree
// [ xdp][%p] Freeing Queue on Interface:%p
// QuicTraceLogVerbose(
            QueueFree,
            "[ xdp][%p] Freeing Queue on Interface:%p",
            Queue,
            Interface);
// arg2 = arg2 = Queue = arg2
// arg3 = arg3 = Interface = arg3
----------------------------------------------------------*/
#ifndef _clog_4_ARGS_TRACE_QueueFree
#define _clog_4_ARGS_TRACE_QueueFree(uniqueId, encoded_arg_string, arg2, arg3)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, QueueFree , arg2, arg3);\

#endif




/*----------------------------------------------------------
// Decoder Ring for QueueInit
// [ xdp][%p] Setting up Queue on Interface:%p
// QuicTraceLogVerbose(
            QueueInit,
            "[ xdp][%p] Setting up Queue on Interface:%p",
            Queue,
            Interface);
// arg2 = arg2 = Queue = arg2
// arg3 = arg3 = Interface = arg3
----------------------------------------------------------*/
#ifndef _clog_4_ARGS_TRACE_QueueInit
#define _clog_4_ARGS_TRACE_QueueInit(uniqueId, encoded_arg_string, arg2, arg3)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, QueueInit , arg2, arg3);\

#endif




/*----------------------------------------------------------
// Decoder Ring for InterfaceInit
// [ xdp][%p] Interface init done
// QuicTraceLogVerbose(
        InterfaceInit,
        "[ xdp][%p] Interface init done",
        Interface);
// arg2 = arg2 = Interface = arg2
----------------------------------------------------------*/
#ifndef _clog_3_ARGS_TRACE_InterfaceInit
#define _clog_3_ARGS_TRACE_InterfaceInit(uniqueId, encoded_arg_string, arg2)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, InterfaceInit , arg2);\

#endif




/*----------------------------------------------------------
// Decoder Ring for XdpInitialize
// [ xdp][%p] XDP initialized, %u procs
// QuicTraceLogVerbose(
        XdpInitialize,
        "[ xdp][%p] XDP initialized, %u procs",
        Xdp,
        Xdp->WorkerCount);
// arg2 = arg2 = Xdp = arg2
// arg3 = arg3 = Xdp->WorkerCount = arg3
----------------------------------------------------------*/
#ifndef _clog_4_ARGS_TRACE_XdpInitialize
#define _clog_4_ARGS_TRACE_XdpInitialize(uniqueId, encoded_arg_string, arg2, arg3)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, XdpInitialize , arg2, arg3);\

#endif




/*----------------------------------------------------------
// Decoder Ring for XdpWorkerStart
// [ xdp][%p] XDP worker start, %u queues
// QuicTraceLogVerbose(
            XdpWorkerStart,
            "[ xdp][%p] XDP worker start, %u queues",
            Worker,
            QueueCount);
// arg2 = arg2 = Worker = arg2
// arg3 = arg3 = QueueCount = arg3
----------------------------------------------------------*/
#ifndef _clog_4_ARGS_TRACE_XdpWorkerStart
#define _clog_4_ARGS_TRACE_XdpWorkerStart(uniqueId, encoded_arg_string, arg2, arg3)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, XdpWorkerStart , arg2, arg3);\

#endif




/*----------------------------------------------------------
// Decoder Ring for XdpRelease
// [ xdp][%p] XDP release
// QuicTraceLogVerbose(
        XdpRelease,
        "[ xdp][%p] XDP release",
        Xdp);
// arg2 = arg2 = Xdp = arg2
----------------------------------------------------------*/
#ifndef _clog_3_ARGS_TRACE_XdpRelease
#define _clog_3_ARGS_TRACE_XdpRelease(uniqueId, encoded_arg_string, arg2)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, XdpRelease , arg2);\

#endif




/*----------------------------------------------------------
// Decoder Ring for XdpUninitializeComplete
// [ xdp][%p] XDP uninitialize complete
// QuicTraceLogVerbose(
            XdpUninitializeComplete,
            "[ xdp][%p] XDP uninitialize complete",
            Xdp);
// arg2 = arg2 = Xdp = arg2
----------------------------------------------------------*/
#ifndef _clog_3_ARGS_TRACE_XdpUninitializeComplete
#define _clog_3_ARGS_TRACE_XdpUninitializeComplete(uniqueId, encoded_arg_string, arg2)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, XdpUninitializeComplete , arg2);\

#endif




/*----------------------------------------------------------
// Decoder Ring for XdpUninitialize
// [ xdp][%p] XDP uninitialize
// QuicTraceLogVerbose(
        XdpUninitialize,
        "[ xdp][%p] XDP uninitialize",
        Xdp);
// arg2 = arg2 = Xdp = arg2
----------------------------------------------------------*/
#ifndef _clog_3_ARGS_TRACE_XdpUninitialize
#define _clog_3_ARGS_TRACE_XdpUninitialize(uniqueId, encoded_arg_string, arg2)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, XdpUninitialize , arg2);\

#endif




/*----------------------------------------------------------
// Decoder Ring for FailSendTo
// [ xdp][tx  ] Faild sendto. errno:%d, Umem addr:%lld
// QuicTraceLogVerbose(
            FailSendTo,
            "[ xdp][tx  ] Faild sendto. errno:%d, Umem addr:%lld", er, Packet->tx_desc->addr);
// arg2 = arg2 = er = arg2
// arg3 = arg3 = Packet->tx_desc->addr = arg3
----------------------------------------------------------*/
#ifndef _clog_4_ARGS_TRACE_FailSendTo
#define _clog_4_ARGS_TRACE_FailSendTo(uniqueId, encoded_arg_string, arg2, arg3)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, FailSendTo , arg2, arg3);\

#endif




/*----------------------------------------------------------
// Decoder Ring for DoneSendTo
// [ xdp][TX  ] Done sendto. len:%d, Umem addr:%lld
// QuicTraceLogVerbose(
            DoneSendTo,
            "[ xdp][TX  ] Done sendto. len:%d, Umem addr:%lld", SendData->Buffer.Length, Packet->tx_desc->addr);
// arg2 = arg2 = SendData->Buffer.Length = arg2
// arg3 = arg3 = Packet->tx_desc->addr = arg3
----------------------------------------------------------*/
#ifndef _clog_4_ARGS_TRACE_DoneSendTo
#define _clog_4_ARGS_TRACE_DoneSendTo(uniqueId, encoded_arg_string, arg2, arg3)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, DoneSendTo , arg2, arg3);\

#endif




/*----------------------------------------------------------
// Decoder Ring for ReleaseCons
// [ xdp][cq  ] Release %d from completion queue
// QuicTraceLogVerbose(
            ReleaseCons,
            "[ xdp][cq  ] Release %d from completion queue", completed);
// arg2 = arg2 = completed = arg2
----------------------------------------------------------*/
#ifndef _clog_3_ARGS_TRACE_ReleaseCons
#define _clog_3_ARGS_TRACE_ReleaseCons(uniqueId, encoded_arg_string, arg2)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, ReleaseCons , arg2);\

#endif




/*----------------------------------------------------------
// Decoder Ring for XdpWorkerShutdown
// [ xdp][%p] XDP worker shutdown
// QuicTraceLogVerbose(
            XdpWorkerShutdown,
            "[ xdp][%p] XDP worker shutdown",
            Worker);
// arg2 = arg2 = Worker = arg2
----------------------------------------------------------*/
#ifndef _clog_3_ARGS_TRACE_XdpWorkerShutdown
#define _clog_3_ARGS_TRACE_XdpWorkerShutdown(uniqueId, encoded_arg_string, arg2)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, XdpWorkerShutdown , arg2);\

#endif




/*----------------------------------------------------------
// Decoder Ring for RxConsPeekFail
// [ xdp][rx  ] Failed to peek from Rx queue
// QuicTraceLogVerbose(
            RxConsPeekFail,
            "[ xdp][rx  ] Failed to peek from Rx queue");
----------------------------------------------------------*/
#ifndef _clog_2_ARGS_TRACE_RxConsPeekFail
#define _clog_2_ARGS_TRACE_RxConsPeekFail(uniqueId, encoded_arg_string)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, RxConsPeekFail );\

#endif




/*----------------------------------------------------------
// Decoder Ring for RxConsPeekSucceed
// [ xdp][rx  ] Succeed peek %d from Rx queue
// QuicTraceLogVerbose(
            RxConsPeekSucceed,
            "[ xdp][rx  ] Succeed peek %d from Rx queue", rcvd);
// arg2 = arg2 = rcvd = arg2
----------------------------------------------------------*/
#ifndef _clog_3_ARGS_TRACE_RxConsPeekSucceed
#define _clog_3_ARGS_TRACE_RxConsPeekSucceed(uniqueId, encoded_arg_string, arg2)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, RxConsPeekSucceed , arg2);\

#endif




/*----------------------------------------------------------
// Decoder Ring for XdpRxRelease
// [ xdp][%p] Release %d from Rx queue (TODO:Check necesity here)
// QuicTraceLogVerbose(
            XdpRxRelease,
            "[ xdp][%p] Release %d from Rx queue (TODO:Check necesity here)",
            Queue, rcvd);
// arg2 = arg2 = Queue = arg2
// arg3 = arg3 = rcvd = arg3
----------------------------------------------------------*/
#ifndef _clog_4_ARGS_TRACE_XdpRxRelease
#define _clog_4_ARGS_TRACE_XdpRxRelease(uniqueId, encoded_arg_string, arg2, arg3)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, XdpRxRelease , arg2, arg3);\

#endif




/*----------------------------------------------------------
// Decoder Ring for XdpWorkerShutdownComplete
// [ xdp][%p] XDP worker shutdown complete
// QuicTraceLogVerbose(
            XdpWorkerShutdownComplete,
            "[ xdp][%p] XDP worker shutdown complete",
            Worker);
// arg2 = arg2 = Worker = arg2
----------------------------------------------------------*/
#ifndef _clog_3_ARGS_TRACE_XdpWorkerShutdownComplete
#define _clog_3_ARGS_TRACE_XdpWorkerShutdownComplete(uniqueId, encoded_arg_string, arg2)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, XdpWorkerShutdownComplete , arg2);\

#endif




/*----------------------------------------------------------
// Decoder Ring for XdpQueueAsyncIoRxComplete
// [ xdp][%p] XDP async IO complete (RX)
// QuicTraceLogVerbose(
            XdpQueueAsyncIoRxComplete,
            "[ xdp][%p] XDP async IO complete (RX)",
            Queue);
// arg2 = arg2 = Queue = arg2
----------------------------------------------------------*/
#ifndef _clog_3_ARGS_TRACE_XdpQueueAsyncIoRxComplete
#define _clog_3_ARGS_TRACE_XdpQueueAsyncIoRxComplete(uniqueId, encoded_arg_string, arg2)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, XdpQueueAsyncIoRxComplete , arg2);\

#endif




/*----------------------------------------------------------
// Decoder Ring for AllocFailure
// Allocation of '%s' failed. (%llu bytes)
// QuicTraceEvent(
            AllocFailure,
            "Allocation of '%s' failed. (%llu bytes)",
            "XDP Queues",
            Interface->QueueCount * sizeof(*Interface->Queues));
// arg2 = arg2 = "XDP Queues" = arg2
// arg3 = arg3 = Interface->QueueCount * sizeof(*Interface->Queues) = arg3
----------------------------------------------------------*/
#ifndef _clog_4_ARGS_TRACE_AllocFailure
#define _clog_4_ARGS_TRACE_AllocFailure(uniqueId, encoded_arg_string, arg2, arg3)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, AllocFailure , arg2, arg3);\

#endif




/*----------------------------------------------------------
// Decoder Ring for LibraryErrorStatus
// [ lib] ERROR, %u, %s.
// QuicTraceEvent(
                    LibraryErrorStatus,
                    "[ lib] ERROR, %u, %s.",
                    Status,
                    "CxPlatDpRawInterfaceInitialize");
// arg2 = arg2 = Status = arg2
// arg3 = arg3 = "CxPlatDpRawInterfaceInitialize" = arg3
----------------------------------------------------------*/
#ifndef _clog_4_ARGS_TRACE_LibraryErrorStatus
#define _clog_4_ARGS_TRACE_LibraryErrorStatus(uniqueId, encoded_arg_string, arg2, arg3)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, LibraryErrorStatus , arg2, arg3);\

#endif




/*----------------------------------------------------------
// Decoder Ring for LibraryError
// [ lib] ERROR, %s.
// QuicTraceEvent(
            LibraryError,
            "[ lib] ERROR, %s.",
            "no XDP capable interface");
// arg2 = arg2 = "no XDP capable interface" = arg2
----------------------------------------------------------*/
#ifndef _clog_3_ARGS_TRACE_LibraryError
#define _clog_3_ARGS_TRACE_LibraryError(uniqueId, encoded_arg_string, arg2)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, LibraryError , arg2);\

#endif




/*----------------------------------------------------------
// Decoder Ring for RxConstructPacket
// [ xdp][rx  ] Constructing Packet from Rx, local=%!ADDR!, remote=%!ADDR!
// QuicTraceEvent(
            RxConstructPacket,
            "[ xdp][rx  ] Constructing Packet from Rx, local=%!ADDR!, remote=%!ADDR!",
            CASTED_CLOG_BYTEARRAY(sizeof(Packet->RouteStorage.LocalAddress), &Packet->RouteStorage.LocalAddress),
            CASTED_CLOG_BYTEARRAY(sizeof(Packet->RouteStorage.RemoteAddress), &Packet->RouteStorage.RemoteAddress));
// arg2 = arg2 = CASTED_CLOG_BYTEARRAY(sizeof(Packet->RouteStorage.LocalAddress), &Packet->RouteStorage.LocalAddress) = arg2
// arg3 = arg3 = CASTED_CLOG_BYTEARRAY(sizeof(Packet->RouteStorage.RemoteAddress), &Packet->RouteStorage.RemoteAddress) = arg3
----------------------------------------------------------*/
#ifndef _clog_6_ARGS_TRACE_RxConstructPacket
#define _clog_6_ARGS_TRACE_RxConstructPacket(uniqueId, encoded_arg_string, arg2, arg2_len, arg3, arg3_len)\
tracepoint(CLOG_DATAPATH_RAW_XDP_LINUX_C, RxConstructPacket , arg2_len, arg2, arg3_len, arg3);\

#endif




#ifdef __cplusplus
}
#endif
#ifdef CLOG_INLINE_IMPLEMENTATION
#include "quic.clog_datapath_raw_xdp_linux.c.clog.h.c"
#endif
