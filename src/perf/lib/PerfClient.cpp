/*++

    Copyright (c) Microsoft Corporation.
    Licensed under the MIT License.

Abstract:

    QUIC Perf Client Implementation

--*/

#include "PerfClient.h"

#ifdef QUIC_CLOG
#include "PerfClient.cpp.clog.h"
#endif

const char PERF_CLIENT_OPTIONS_TEXT[] =
"\n"
"Usage (client): secnetperf -target:<hostname/ip> [client options]\n"
"\n"
"Client Options:\n"
"\n"
"  Remote options:\n"
"  -ip:<0/4/6>              A hint for the resolving the hostname to an IP address. (def:0)\n"
"  -port:<####>             The UDP port of the server. (def:%u)\n"
"  -cibir:<hex_bytes>       A CIBIR well-known idenfitier.\n"
"  -incrementtarget:<0/1>   Append unique ID to target hostname for each worker (def:0).\n"
"\n"
"  Local options:\n"
//"  -bind:<addr>             The local IP address(es)/port(s) to bind to.\n"
//"  -addrs:<####>            The max number of local addresses to use. (def:%u)\n"
"  -threads:<####>          The max number of worker threads to use.\n"
"  -affinitize:<0/1>        Affinitizes worker threads to a core. (def:0)\n"
#ifdef QUIC_COMPARTMENT_ID
"  -comp:<####>             The network compartment ID to run in.\n"
#endif
"\n"
"  Config options:\n"
"  -encrypt:<0/1>           Disables/enables encryption. (def:1)\n"
"  -pacing:<0/1>            Disables/enables send pacing. (def:1)\n"
"  -sendbuf:<0/1>           Disables/enables send buffering. (def:0)\n"
"  -ptput:<0/1>             Print throughput information. (def:0)\n"
"  -pconn:<0/1>             Print connection statistics. (def:0)\n"
"  -pstream:<0/1>           Print stream statistics. (def:0)\n"
"  -platency<0/1>           Print latency statistics. (def:0)\n"
"\n"
"  Scenario options:\n"
"  -conns:<####>            The number of connections to use. (def:1)\n"
"  -streams:<####>          The number of streams to send on at a time. (def:0)\n"
"  -upload:<####>           The length of bytes to send on each stream. (def:0)\n"
"  -download:<####>         The length of bytes to receive on each stream. (def:0)\n"
"  -timed:<0/1>             Indicates the upload/download args are times (in ms). (def:0)\n"
//"  -inline:<0/1>            Create new streams on callbacks. (def:0)\n"
"  -rconn:<0/1>             Continue to loop the scenario at the connection level. (def:0)\n"
"  -rstream:<0/1>           Continue to loop the scenario at the stream level. (def:0)\n"
"  -runtime:<####>          The total runtime (in ms). Only relevant for repeat scenarios. (def:0)\n"
"\n";

static void PrintHelp() {
    WriteOutput(
        PERF_CLIENT_OPTIONS_TEXT,
        PERF_DEFAULT_PORT
        //PERF_MAX_CLIENT_PORT_COUNT
        );
}

QUIC_STATUS
PerfClient::Init(
    _In_ int argc,
    _In_reads_(argc) _Null_terminated_ char* argv[]
    ) {
    if (argc > 0 && (IsArg(argv[0], "?") || IsArg(argv[0], "help"))) {
        PrintHelp();
        return QUIC_STATUS_INVALID_PARAMETER;
    }

    if (!Configuration.IsValid()) {
        return Configuration.GetInitStatus();
    }

    //
    // Remote target/server options
    //

    const char* target;
    if (!TryGetValue(argc, argv, "target", &target) &&
        !TryGetValue(argc, argv, "server", &target)) {
        WriteOutput("Must specify '-target' argument!\n");
        PrintHelp();
        return QUIC_STATUS_INVALID_PARAMETER;
    }

    size_t Len = strlen(target);
    Target.reset(new(std::nothrow) char[Len + 1]);
    if (!Target.get()) {
        return QUIC_STATUS_OUT_OF_MEMORY;
    }
    CxPlatCopyMemory(Target.get(), target, Len);
    Target[Len] = '\0';

    uint16_t Ip;
    if (TryGetValue(argc, argv, "ip", &Ip)) {
        switch (Ip) {
        case 4: TargetFamily = QUIC_ADDRESS_FAMILY_INET; break;
        case 6: TargetFamily = QUIC_ADDRESS_FAMILY_INET6; break;
        }
    }

    TryGetValue(argc, argv, "port", &TargetPort);
    TryGetValue(argc, argv, "incrementtarget", &IncrementTarget);

    const char* CibirBytes = nullptr;
    if (TryGetValue(argc, argv, "cibir", &CibirBytes)) {
        CibirId[0] = 0; // offset
        if ((CibirIdLength = DecodeHexBuffer(CibirBytes, 6, CibirId+1)) == 0) {
            WriteOutput("Cibir ID must be a hex string <= 6 bytes.\n");
            return QUIC_STATUS_INVALID_PARAMETER;
        }
    }

    //
    // Local address and execution configuration options
    //

    /*TryGetValue(argc, argv, "addrs", &MaxLocalAddrCount);

    char* LocalAddress = (char*)GetValue(argc, argv, "bind");
    if (LocalAddress != nullptr) {
        SpecificLocalAddresses = true;
        MaxLocalAddrCount = 0;
        while (LocalAddress) {
            char* AddrEnd = strchr(LocalAddress, ',');
            if (AddrEnd) {
                *AddrEnd = '\0';
                AddrEnd++;
            }
            if (!ConvertArgToAddress(LocalAddress, 0, &LocalAddresses[MaxLocalAddrCount++])) {
                WriteOutput("Failed to decode bind IP address: '%s'!\nMust be *, a IPv4 or a IPv6 address.\n", LocalAddress);
                PrintHelp();
                return QUIC_STATUS_INVALID_PARAMETER;
            }
            LocalAddress = AddrEnd;
        }
    }

    if (MaxLocalAddrCount > PERF_MAX_CLIENT_PORT_COUNT) {
        WriteOutput("Too many local addresses!\n");
        return QUIC_STATUS_INVALID_PARAMETER;
    }*/

    WorkerCount = CxPlatProcActiveCount();
    TryGetValue(argc, argv, "threads", &WorkerCount);
    TryGetValue(argc, argv, "workers", &WorkerCount);
    TryGetValue(argc, argv, "affinitize", &AffinitizeWorkers);

#ifdef QUIC_COMPARTMENT_ID
    TryGetValue(argc, argv, "comp",  &CompartmentId);
#endif

    //
    // General configuration options
    //

    TryGetValue(argc, argv, "encrypt", &UseEncryption);
    TryGetValue(argc, argv, "pacing", &UsePacing);
    TryGetValue(argc, argv, "sendbuf", &UseSendBuffering);
    TryGetValue(argc, argv, "ptput", &PrintThroughput);
    TryGetValue(argc, argv, "pconn", &PrintConnections);
    TryGetValue(argc, argv, "pstream", &PrintStreams);
    TryGetValue(argc, argv, "platency", &PrintLatency);

    if (UseSendBuffering || !UsePacing) { // Update settings if non-default
        MsQuicSettings Settings;
        Configuration.GetSettings(Settings);
        if (!UseSendBuffering) {
            Settings.SetSendBufferingEnabled(UseSendBuffering != 0);
        }
        if (!UsePacing) {
            Settings.SetPacingEnabled(UsePacing != 0);
        }
        Configuration.SetSettings(Settings);
    }

    //
    // Scenario options
    //

    TryGetValue(argc, argv, "conns", &ConnectionCount);
    TryGetValue(argc, argv, "requests", &StreamCount);
    TryGetValue(argc, argv, "streams", &StreamCount);
    TryGetValue(argc, argv, "iosize", &IoSize);
    if (IoSize < 256) {
        WriteOutput("'iosize' too small'!\n");
        return QUIC_STATUS_INVALID_PARAMETER;
    }
    TryGetValue(argc, argv, "request", &Upload);
    TryGetValue(argc, argv, "upload", &Upload);
    TryGetValue(argc, argv, "up", &Upload);
    TryGetValue(argc, argv, "response", &Download);
    TryGetValue(argc, argv, "download", &Download);
    TryGetValue(argc, argv, "down", &Download);
    TryGetValue(argc, argv, "timed", &Timed);
    //TryGetValue(argc, argv, "inline", &SendInline);
    TryGetValue(argc, argv, "rconn", &RepeatConnections);
    TryGetValue(argc, argv, "rstream", &RepeatStreams);
    TryGetValue(argc, argv, "runtime", &RunTime);
    TryGetValue(argc, argv, "time", &RunTime);
    TryGetValue(argc, argv, "run", &RunTime);

    //
    // Other state initialization
    //

    RequestBuffer.Init(IoSize, Timed ? UINT64_MAX : Download);
    if (PrintLatency && RunTime) {
        MaxLatencyIndex = ((uint64_t)RunTime / 1000) * PERF_MAX_REQUESTS_PER_SECOND;
        if (MaxLatencyIndex > (UINT32_MAX / sizeof(uint32_t))) {
            MaxLatencyIndex = UINT32_MAX / sizeof(uint32_t);
            WriteOutput("Warning! Limiting request latency tracking to %llu requests\n",
                (unsigned long long)MaxLatencyIndex);
        }

        LatencyValues = UniquePtr<uint32_t[]>(new(std::nothrow) uint32_t[(size_t)MaxLatencyIndex]);
        if (LatencyValues == nullptr) {
            return QUIC_STATUS_OUT_OF_MEMORY;
        }
        CxPlatZeroMemory(LatencyValues.get(), (size_t)(sizeof(uint32_t) * MaxLatencyIndex));
    }

    return QUIC_STATUS_SUCCESS;
}

static void AppendIntToString(char* String, uint8_t Value) {
    const char* Hex = "0123456789ABCDEF";
    String[0] = Hex[(Value >> 4) & 0xF];
    String[1] = Hex[Value & 0xF];
    String[2] = '\0';
}

QUIC_STATUS
PerfClient::Start(
    _In_ CXPLAT_EVENT* StopEvent
    ) {
    CompletionEvent = StopEvent;

    //
    // Resolve the remote address to connect to (to optimize the HPS metric).
    //
    QUIC_STATUS Status;
    CXPLAT_DATAPATH* Datapath = nullptr;
    if (QUIC_FAILED(Status = CxPlatDataPathInitialize(0, nullptr, nullptr, nullptr, &Datapath))) {
        WriteOutput("Failed to initialize datapath for resolution!\n");
        return Status;
    }
    QUIC_ADDR RemoteAddr;
    Status = CxPlatDataPathResolveAddress(Datapath, Target.get(), &RemoteAddr);
    CxPlatDataPathUninitialize(Datapath);
    if (QUIC_FAILED(Status)) {
        WriteOutput("Failed to resolve remote address!\n");
        return Status;
    }

    //
    // Configure and start all the workers.
    //
    CXPLAT_THREAD_CONFIG ThreadConfig = {
        (uint16_t)(AffinitizeWorkers ? CXPLAT_THREAD_FLAG_SET_AFFINITIZE : CXPLAT_THREAD_FLAG_NONE),
        0,
        "Perf Worker",
        PerfClientWorker::s_WorkerThread,
        nullptr
    };
    const size_t TargetLen = strlen(Target.get());
    for (uint32_t i = 0; i < WorkerCount; ++i) {
        while (!CxPlatProcIsActive(ThreadConfig.IdealProcessor)) {
            ++ThreadConfig.IdealProcessor;
        }

        auto Worker = &Workers[i];
        Worker->Processor = ThreadConfig.IdealProcessor++;
        ThreadConfig.Context = Worker;
        Worker->RemoteAddr.SockAddr = RemoteAddr;
        Worker->RemoteAddr.SetPort(TargetPort);

        // Build up target hostname.
        Worker->Target.reset(new(std::nothrow) char[TargetLen + 10]);
        CxPlatCopyMemory(Worker->Target.get(), Target.get(), TargetLen);
        if (IncrementTarget) {
            AppendIntToString(Worker->Target.get() + TargetLen, (uint8_t)Worker->Processor);
        } else {
            Worker->Target.get()[TargetLen] = '\0';
        }
        Worker->Target.get()[TargetLen] = '\0';

        Status = CxPlatThreadCreate(&ThreadConfig, &Workers[i].Thread);
        if (QUIC_FAILED(Status)) {
            WriteOutput("Failed to start worker thread on processor %hu!\n", Worker->Processor);
            return Status;
        }
        Workers[i].ThreadStarted = true;
    }

    //
    // Queue the connections on the workers.
    //
    for (uint32_t i = 0; i < ConnectionCount; ++i) {
        Workers[i % WorkerCount].QueueNewConnection();
    }

    return QUIC_STATUS_SUCCESS;
}

QUIC_STATUS
PerfClient::Wait(
    _In_ int Timeout
    ) {
    if (Timeout == 0) {
        Timeout = RunTime;
    }

    if (Timeout) {
        WriteOutput("Running! Waiting up to %d ms!\n", Timeout);
        CxPlatEventWaitWithTimeout(*CompletionEvent, Timeout);
    } else {
        WriteOutput("Running!\n");
        CxPlatEventWaitForever(*CompletionEvent);
    }

    WriteOutput("Done!\n");
    Running = false;
    for (uint32_t i = 0; i < WorkerCount; ++i) {
        Workers[i].Uninitialize();
    }

    if (StreamCount) {
        auto StreamsCompleted = GetStreamsCompleted();
        WriteOutput("Completed %llu streams!\n", (unsigned long long)StreamsCompleted);
        CachedStreamsCompleted = StreamsCompleted;
    }

    return QUIC_STATUS_SUCCESS;
}

void
PerfClient::GetExtraDataMetadata(
    _Out_ PerfExtraDataMetadata* Result
    )
{
    Result->TestType = PerfTestType::Client;
    uint64_t DataLength = sizeof(RunTime) + sizeof(CachedStreamsCompleted) + (CachedStreamsCompleted * sizeof(uint32_t));
    CXPLAT_FRE_ASSERT(DataLength <= UINT32_MAX); // TODO Limit values properly
    Result->ExtraDataLength = (uint32_t)DataLength;
}

QUIC_STATUS
PerfClient::GetExtraData(
    _Out_writes_bytes_(*Length) uint8_t* Data,
    _Inout_ uint32_t* Length
    )
{
    CXPLAT_FRE_ASSERT(*Length >= sizeof(RunTime) + sizeof(CachedStreamsCompleted));
    CxPlatCopyMemory(Data, &RunTime, sizeof(RunTime));
    Data += sizeof(RunTime);
    CxPlatCopyMemory(Data, &CachedStreamsCompleted, sizeof(CachedStreamsCompleted));
    Data += sizeof(CachedStreamsCompleted);
    uint64_t BufferLength = *Length - sizeof(RunTime) - sizeof(CachedStreamsCompleted);
    if (BufferLength > CachedStreamsCompleted * sizeof(uint32_t)) {
        BufferLength = CachedStreamsCompleted * sizeof(uint32_t);
        *Length = (uint32_t)(BufferLength + sizeof(RunTime) + sizeof(CachedStreamsCompleted));
    }
    CxPlatCopyMemory(Data, LatencyValues.get(), (uint32_t)BufferLength);
    return QUIC_STATUS_SUCCESS;
}

QUIC_STATUS
PerfClientConnection::ConnectionCallback(
    _Inout_ QUIC_CONNECTION_EVENT* Event
    ) {
    switch (Event->Type) {
    case QUIC_CONNECTION_EVENT_CONNECTED:
        InterlockedIncrement64((int64_t*)&Worker.ConnectionsConnected);
        for (uint32_t i = 0; i < Client.StreamCount; ++i) {
            StartNewStream();
        }
        break;
    case QUIC_CONNECTION_EVENT_SHUTDOWN_INITIATED_BY_TRANSPORT:
        //WriteOutput("Connection died, 0x%x\n", Event->SHUTDOWN_INITIATED_BY_TRANSPORT.Status);
        break;
    case QUIC_CONNECTION_EVENT_SHUTDOWN_COMPLETE:
        if (Client.PrintConnections) {
            QuicPrintConnectionStatistics(MsQuic, Handle);
        }
        Worker.OnConnectionComplete();
        break;
    case QUIC_CONNECTION_EVENT_IDEAL_PROCESSOR_CHANGED:
        /*if ((uint32_t)Event->IDEAL_PROCESSOR_CHANGED.IdealProcessor >= Client.WorkerCount) {
            Event->IDEAL_PROCESSOR_CHANGED.IdealProcessor = (uint16_t)(Client.WorkerCount - 1);
        }
        Client->Workers[Event->IDEAL_PROCESSOR_CHANGED.IdealProcessor].UpdateConnection(this);*/
        break;
    default:
        break;
    }
    return QUIC_STATUS_SUCCESS;
}

QUIC_STATUS
PerfClientConnection::StreamCallback(
    _In_ PerfClientStream* Stream,
    _In_ HQUIC StreamHandle,
    _Inout_ QUIC_STREAM_EVENT* Event
    ) {
    switch (Event->Type) {
    case QUIC_STREAM_EVENT_RECEIVE:
        Receive(Stream, Event->RECEIVE.TotalBufferLength, Event->RECEIVE.Flags & QUIC_RECEIVE_FLAG_FIN);
        break;
    case QUIC_STREAM_EVENT_SEND_COMPLETE:
        SendComplete(Stream, (QUIC_BUFFER*)Event->SEND_COMPLETE.ClientContext, Event->SEND_COMPLETE.Canceled);
        break;
    case QUIC_STREAM_EVENT_PEER_SEND_ABORTED:
        if (!Stream->RecvEndTime) {
            WriteOutput("Peer stream aborted recv!\n");
            Stream->RecvEndTime = CxPlatTimeUs64();
        }
        MsQuic->StreamShutdown(StreamHandle, QUIC_STREAM_SHUTDOWN_FLAG_ABORT, 0);
        break;
    case QUIC_STREAM_EVENT_PEER_RECEIVE_ABORTED:
        if (!Stream->SendEndTime) {
            WriteOutput("Peer stream aborted send!\n");
            Stream->SendEndTime = CxPlatTimeUs64();
        }
        MsQuic->StreamShutdown(StreamHandle, QUIC_STREAM_SHUTDOWN_FLAG_ABORT_SEND, 0);
        Stream->SendComplete = true;
        break;
    case QUIC_STREAM_EVENT_SEND_SHUTDOWN_COMPLETE:
        Stream->SendEndTime = CxPlatTimeUs64();
        if (Client.PrintStreams) {
            QUIC_STREAM_STATISTICS Stats = {0};
            uint32_t BufferLength = sizeof(Stats);
            MsQuic->GetParam(StreamHandle, QUIC_PARAM_STREAM_STATISTICS, &BufferLength, &Stats);
            WriteOutput("Flow blocked timing:\n");
            WriteOutput(
                "SCHEDULING:             %llu us\n",
                (unsigned long long)Stats.ConnBlockedBySchedulingUs);
            WriteOutput(
                "PACING:                 %llu us\n",
                (unsigned long long)Stats.ConnBlockedByPacingUs);
            WriteOutput(
                "AMPLIFICATION_PROT:     %llu us\n",
                (unsigned long long)Stats.ConnBlockedByAmplificationProtUs);
            WriteOutput(
                "CONGESTION_CONTROL:     %llu us\n",
                (unsigned long long)Stats.ConnBlockedByCongestionControlUs);
            WriteOutput(
                "CONN_FLOW_CONTROL:      %llu us\n",
                (unsigned long long)Stats.ConnBlockedByFlowControlUs);
            WriteOutput(
                "STREAM_ID_FLOW_CONTROL: %llu us\n",
                (unsigned long long)Stats.StreamBlockedByIdFlowControlUs);
            WriteOutput(
                "STREAM_FLOW_CONTROL:    %llu us\n",
                (unsigned long long)Stats.StreamBlockedByFlowControlUs);
            WriteOutput(
                "APP:                    %llu us\n",
                (unsigned long long)Stats.StreamBlockedByAppUs);
        }
        break;
    case QUIC_STREAM_EVENT_SHUTDOWN_COMPLETE:
        StreamShutdownComplete(Stream);
        break;
    case QUIC_STREAM_EVENT_IDEAL_SEND_BUFFER_SIZE:
        if (Client.Upload &&
            !Client.UseSendBuffering &&
            Stream->IdealSendBuffer < Event->IDEAL_SEND_BUFFER_SIZE.ByteCount) {
            Stream->IdealSendBuffer = Event->IDEAL_SEND_BUFFER_SIZE.ByteCount;
            SendData(Stream);
        }
    default:
        break;
    }
    return QUIC_STATUS_SUCCESS;
}

void
PerfClientWorker::WorkerThread() {
#ifdef QUIC_COMPARTMENT_ID
    if (Client->CompartmentId != UINT16_MAX) {
        NETIO_STATUS status;
        if (!NETIO_SUCCESS(status = QuicCompartmentIdSetCurrent(Client->CompartmentId))) {
            WriteOutput("Failed to set compartment ID = %d: 0x%x\n", Client->CompartmentId, status);
            return;
        }
    }
#endif

    while (Client->Running) {
        while (ConnectionsCreated < ConnectionsQueued) {
            StartNewConnection();
        }
        WakeEvent.WaitForever();
    }
}

void
PerfClientWorker::OnConnectionComplete() {
    InterlockedIncrement64((int64_t*)&ConnectionsCompleted);
    InterlockedDecrement64((int64_t*)&ConnectionsActive);
    if (Client->RepeatConnections) {
        // TODO - WakeEvent.Set();
    } else {
        if (!ConnectionsActive && ConnectionsCreated == ConnectionsQueued) {
            Client->OnConnectionsComplete();
        }
    }
}

void
PerfClientWorker::StartNewConnection() {
    InterlockedIncrement64((int64_t*)&ConnectionsCreated);
    InterlockedIncrement64((int64_t*)&ConnectionsActive);
    auto Connection = ConnectionAllocator.Alloc(Client->Registration, *Client, *this); // TODO - Fix destructor part
    if (!Connection->IsValid()) {
        WriteOutput("ConnectionOpen failed, 0x%x\n", Connection->GetInitStatus());
        return;
    }

    QUIC_STATUS Status;
    if (!Client->UseEncryption) {
        Status = Connection->SetDisable1RttEncryption();
        if (QUIC_FAILED(Status)) {
            WriteOutput("SetDisable1RttEncryption failed, 0x%x\n", Status);
            Connection->Close();
            return;
        }
    }

    Status = Connection->SetShareUdpBinding();
    if (QUIC_FAILED(Status)) {
        WriteOutput("SetShareUdpBinding failed, 0x%x\n", Status);
        Connection->Close();
        return;
    }

    if (Client->CibirIdLength) {
        Status = Connection->SetCibirId(Client->CibirId, Client->CibirIdLength+1);
        if (QUIC_FAILED(Status)) {
            WriteOutput("SetCibirId failed, 0x%x\n", Status);
            Connection->Close();
            return;
        }
    }

    /*if (Client->SpecificLocalAddresses || i >= Client->MaxLocalAddrCount) {
        Status = Connection->SetLocalAddr(*(QuicAddr*)&Client->LocalAddresses[i % Client->MaxLocalAddrCount]);
        if (QUIC_FAILED(Status)) {
            WriteOutput("SetLocalAddr failed!\n");
            Connection->Close();
            return;
        }
    }*/

    Status = Connection->Start(Client->Configuration, Client->TargetFamily, Target.get(), RemoteAddr.GetPort());
    if (QUIC_FAILED(Status)) {
        WriteOutput("Start failed, 0x%x\n", Status);
        Connection->Close();
        return;
    }

    /*if (!Client->SpecificLocalAddresses && i < PERF_MAX_CLIENT_PORT_COUNT) {
        Status = Connection->GetLocalAddr(*(QuicAddr*)&Client->LocalAddresses[i]);
        if (QUIC_FAILED(Status)) {
            WriteOutput("GetLocalAddr failed!\n");
            return;
        }
    }*/
}

void
PerfClientConnection::StartNewStream(bool DelaySend) {
    UNREFERENCED_PARAMETER(DelaySend);
    StreamsCreated++;
    StreamsActive++;
    auto Stream = Worker.StreamAllocator.Alloc(*this);
    if (QUIC_FAILED(
        MsQuic->StreamOpen(
            Handle,
            QUIC_STREAM_OPEN_FLAG_NONE,
            PerfClientStream::s_StreamCallback,
            Stream,
            &Stream->Handle))) {
        Worker.StreamAllocator.Free(Stream);
        return;
    }

    InterlockedIncrement64((int64_t*)&Worker.StreamsStarted);
    SendData(Stream);
}

void
PerfClientConnection::SendData(
    _In_ PerfClientStream* Stream
    )
{
    while (!Stream->SendComplete && Stream->BytesOutstanding < Stream->IdealSendBuffer) {

        const uint64_t BytesLeftToSend =
            Client.Timed ?
                UINT64_MAX : // Timed sends forever
                (Client.Upload ? (Client.Upload - Stream->BytesSent) : sizeof(uint64_t));
        uint32_t DataLength = Client.IoSize;
        QUIC_BUFFER* Buffer = Client.RequestBuffer;
        QUIC_SEND_FLAGS Flags = QUIC_SEND_FLAG_START;

        if ((uint64_t)DataLength >= BytesLeftToSend) {
            DataLength = (uint32_t)BytesLeftToSend;
            Stream->LastBuffer.Buffer = Buffer->Buffer;
            Stream->LastBuffer.Length = DataLength;
            Buffer = &Stream->LastBuffer;
            Flags |= QUIC_SEND_FLAG_FIN;
            Stream->SendComplete = true;

        } else if (Client.Timed &&
                   CxPlatTimeDiff64(Stream->StartTime, CxPlatTimeUs64()) >= MS_TO_US(Client.Upload)) {
            Flags |= QUIC_SEND_FLAG_FIN;
            Stream->SendComplete = true;
        }

        Stream->BytesSent += DataLength;
        Stream->BytesOutstanding += DataLength;

        MsQuic->StreamSend(Stream->Handle, Buffer, 1, Flags, Buffer);
    }
}

void
PerfClientConnection::SendComplete(
    _In_ PerfClientStream* Stream,
    _In_ const QUIC_BUFFER* Buffer,
    _In_ bool Canceled
    )
{
    Stream->BytesOutstanding -= Buffer->Length;
    if (!Canceled) {
        Stream->BytesAcked += Buffer->Length;
        SendData(Stream);
    }
}

void
PerfClientConnection::Receive(
    _In_ PerfClientStream* Stream,
    _In_ uint64_t Length,
    _In_ bool Finished
    )
{
    Stream->BytesReceived += Length;

    uint64_t Now = 0;
    if (!Stream->RecvStartTime) {
        Now = CxPlatTimeUs64();
        Stream->RecvStartTime = Now;
    }

    if (Finished) {
        if (Now == 0) Now = CxPlatTimeUs64();
        Stream->RecvEndTime = Now;
    } if (Client.Timed) {
        if (Now == 0) Now = CxPlatTimeUs64();
        if (CxPlatTimeDiff64(Stream->RecvStartTime, Now) >= MS_TO_US(Client.Download)) {
            MsQuic->StreamShutdown(Stream->Handle, QUIC_STREAM_SHUTDOWN_FLAG_ABORT_RECEIVE, 0);
            Stream->RecvEndTime = Now;
        }
    }
}

void
PerfClientConnection::StreamShutdownComplete(
    _In_ PerfClientStream* Stream
    )
{
    if (Client.Upload && Client.PrintThroughput) {
        const auto TotalBytes = Stream->BytesAcked;
        if (TotalBytes < sizeof(uint64_t)) {
            WriteOutput("Error: Failed to send request length! Failed to connect?\n");
        } else if (!Client.Timed && TotalBytes < Client.Upload) {
            WriteOutput("Error: Failed to send all bytes.\n");
        } else {
            const auto ElapsedMicroseconds = Stream->SendEndTime - Stream->StartTime;
            const auto Rate = (uint32_t)((TotalBytes * 1000 * 1000 * 8) / (1000 * ElapsedMicroseconds));
            WriteOutput(
                "  Upload: %llu bytes @ %u kbps (%u.%03u ms).\n",
                (unsigned long long)TotalBytes,
                Rate,
                (uint32_t)(ElapsedMicroseconds / 1000),
                (uint32_t)(ElapsedMicroseconds % 1000));
        }
    }

    if (Client.Download && Client.PrintThroughput) {
        const auto TotalBytes = Stream->BytesReceived;
        if (TotalBytes == 0 || (!Client.Timed && TotalBytes < Client.Download)) {
            WriteOutput("Error: Failed to receive all bytes.\n");
        } else {
            const auto ElapsedMicroseconds = Stream->RecvEndTime - Stream->RecvStartTime;
            const auto Rate = (uint32_t)((TotalBytes * 1000 * 1000 * 8) / (1000 * ElapsedMicroseconds));
            WriteOutput(
                "Download: %llu bytes @ %u kbps (%u.%03u ms).\n",
                (unsigned long long)TotalBytes,
                Rate,
                (uint32_t)(ElapsedMicroseconds / 1000),
                (uint32_t)(ElapsedMicroseconds % 1000));
        }
    }

    const auto Latency = CxPlatTimeDiff64(Stream->StartTime, Stream->RecvEndTime);
    const auto ToPlaceIndex = (uint64_t)InterlockedIncrement64((int64_t*)&Worker.StreamsCompleted) - 1;
    if (ToPlaceIndex < Client.MaxLatencyIndex) {
        Client.LatencyValues[(size_t)ToPlaceIndex] = Latency > UINT32_MAX ? UINT32_MAX : (uint32_t)Latency;
    }

    Worker.StreamAllocator.Free(Stream);

    StreamsActive--;
    if (Client.RepeatStreams) {
        while (StreamsActive < Client.StreamCount) {
            StartNewStream();
        }
    } else {
        if (!StreamsActive && StreamsCreated == Client.StreamCount) {
            Shutdown(0);
        }
    }
}
