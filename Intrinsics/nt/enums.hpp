#pragma once

#include "fwd.hpp"

enum _KPROCESSOR_MODE
{
    KernelMode,
    UserMode,
    MaximumMode
};

enum _MODE
{
    KernelModeEnum = 0,
    UserModeEnum = 1,
    MaximumModeEnum = 2
};

enum _POOL_TYPE
{
    NonPagedPool,
    NonPagedPoolExecute = NonPagedPool,
    PagedPool,
    NonPagedPoolMustSucceed = NonPagedPool + 2,
    DontUseThisType,
    NonPagedPoolCacheAligned = NonPagedPool + 4,
    PagedPoolCacheAligned,
    NonPagedPoolCacheAlignedMustS = NonPagedPool + 6,
    MaxPoolType,
    NonPagedPoolBase = 0,
    NonPagedPoolBaseMustSucceed = NonPagedPoolBase + 2,
    NonPagedPoolBaseCacheAligned = NonPagedPoolBase + 4,
    NonPagedPoolBaseCacheAlignedMustS = NonPagedPoolBase + 6,
    NonPagedPoolSession = 32,
    PagedPoolSession = NonPagedPoolSession + 1,
    NonPagedPoolMustSucceedSession = PagedPoolSession + 1,
    DontUseThisTypeSession = NonPagedPoolMustSucceedSession + 1,
    NonPagedPoolCacheAlignedSession = DontUseThisTypeSession + 1,
    PagedPoolCacheAlignedSession = NonPagedPoolCacheAlignedSession + 1,
    NonPagedPoolCacheAlignedMustSSession = PagedPoolCacheAlignedSession + 1,
    NonPagedPoolNx = 512,
    NonPagedPoolNxCacheAligned = NonPagedPoolNx + 4,
    NonPagedPoolSessionNx = NonPagedPoolNx + 32,
};

enum _MEMORY_CACHING_TYPE
{
    MmNonCached = 0,
    MmCached = 1,
    MmWriteCombined = 2,
    MmHardwareCoherentCached = 3,
    MmNonCachedUnordered = 4,
    MmUSWCCached = 5,
    MmMaximumCacheType = 6,
    MmNotMapped = -1
};

enum _MI_ASSIGNED_REGION_TYPES
{
    AssignedRegionPfnDatabase = 0,
    AssignedRegionNonPagedPool = 1,
    AssignedRegionPagedPool = 2,
    AssignedRegionSystemCache = 3,
    AssignedRegionSystemPtes = 4,
    AssignedRegionKasan = 5,
    AssignedRegionUltraZero = 6,
    AssignedRegionCfg = 7,
    AssignedRegionHyperSpace = 8,
    AssignedRegionKernelStacks = 9,
    AssignedRegionNonCachedMappings = 10,
    AssignedRegionSoftWsles = 11,
    AssignedRegionPageTables = 12,
    AssignedRegionNotUsed = 13,
    AssignedRegionSecureNonPagedPool = 14,
    AssignedRegionKernelShadowStacks = 15,
    AssignedRegionSystemDataViews = 16,
    AssignedRegionSystemImages = 17,
    AssignedRegionMaximum = 18
};

enum _EVENT_TYPE
{
    NotificationEvent = 0,
    SynchronizationEvent = 1
};

enum _LOCK_OPERATION
{
    IoReadAccess = 0,
    IoWriteAccess = 1,
    IoModifyAccess = 2
};

enum _WORK_QUEUE_TYPE
{
    CriticalWorkQueue = 0,
    DelayedWorkQueue = 1,
    HyperCriticalWorkQueue = 2,
    NormalWorkQueue = 3,
    BackgroundWorkQueue = 4,
    RealTimeWorkQueue = 5,
    SuperCriticalWorkQueue = 6,
    MaximumWorkQueue = 7
};

enum _MM_PAGE_PRIORITY
{
    LowPagePriority = 0,
    NormalPagePriority = 16,
    HighPagePriority = 32
};

enum _KAPC_ENVIRONMENT
{
    OriginalApcEnvironment = 0,
    AttachedApcEnvironment = 1,
    CurrentApcEnvironment = 2,
    InsertApcEnvironment = 3
};

enum _IO_PRIORITY_HINT
{
    IoPriorityVeryLow = 0,
    IoPriorityLow = 1,
    IoPriorityNormal = 2,
    IoPriorityHigh = 3,
    IoPriorityCritical = 4,
    MaxIoPriorityTypes = 5
};

enum _FILE_INFORMATION_CLASS
{
    FileDirectoryInformation = 1,
    FileFullDirectoryInformation = 2,
    FileBothDirectoryInformation = 3,
    FileBasicInformation = 4,
    FileStandardInformation = 5,
    FileInternalInformation = 6,
    FileEaInformation = 7,
    FileAccessInformation = 8,
    FileNameInformation = 9,
    FileRenameInformation = 10,
    FileLinkInformation = 11,
    FileNamesInformation = 12,
    FileDispositionInformation = 13,
    FilePositionInformation = 14,
    FileFullEaInformation = 15,
    FileModeInformation = 16,
    FileAlignmentInformation = 17,
    FileAllInformation = 18,
    FileAllocationInformation = 19,
    FileEndOfFileInformation = 20,
    FileAlternateNameInformation = 21,
    FileStreamInformation = 22,
    FilePipeInformation = 23,
    FilePipeLocalInformation = 24,
    FilePipeRemoteInformation = 25,
    FileMailslotQueryInformation = 26,
    FileMailslotSetInformation = 27,
    FileCompressionInformation = 28,
    FileObjectIdInformation = 29,
    FileCompletionInformation = 30,
    FileMoveClusterInformation = 31,
    FileQuotaInformation = 32,
    FileReparsePointInformation = 33,
    FileNetworkOpenInformation = 34,
    FileAttributeTagInformation = 35,
    FileTrackingInformation = 36,
    FileIdBothDirectoryInformation = 37,
    FileIdFullDirectoryInformation = 38,
    FileValidDataLengthInformation = 39,
    FileShortNameInformation = 40,
    FileIoCompletionNotificationInformation = 41,
    FileIoStatusBlockRangeInformation = 42,
    FileIoPriorityHintInformation = 43,
    FileSfioReserveInformation = 44,
    FileSfioVolumeInformation = 45,
    FileHardLinkInformation = 46,
    FileProcessIdsUsingFileInformation = 47,
    FileNormalizedNameInformation = 48,
    FileNetworkPhysicalNameInformation = 49,
    FileIdGlobalTxDirectoryInformation = 50,
    FileIsRemoteDeviceInformation = 51,
    FileUnusedInformation = 52,
    FileNumaNodeInformation = 53,
    FileStandardLinkInformation = 54,
    FileRemoteProtocolInformation = 55,
    FileRenameInformationBypassAccessCheck = 56,
    FileLinkInformationBypassAccessCheck = 57,
    FileVolumeNameInformation = 58,
    FileIdInformation = 59,
    FileIdExtdDirectoryInformation = 60,
    FileReplaceCompletionInformation = 61,
    FileHardLinkFullIdInformation = 62,
    FileIdExtdBothDirectoryInformation = 63,
    FileDispositionInformationEx = 64,
    FileRenameInformationEx = 65,
    FileRenameInformationExBypassAccessCheck = 66,
    FileDesiredStorageClassInformation = 67,
    FileStatInformation = 68,
    FileMemoryPartitionInformation = 69,
    FileStatLxInformation = 70,
    FileCaseSensitiveInformation = 71,
    FileLinkInformationEx = 72,
    FileLinkInformationExBypassAccessCheck = 73,
    FileStorageReserveIdInformation = 74,
    FileCaseSensitiveInformationForceAccessCheck = 75,
    FileKnownFolderInformation = 76,
    FileMaximumInformation = 77
};

enum _KWAIT_REASON
{
    Executive = 0,
    FreePage = 1,
    PageIn = 2,
    PoolAllocation = 3,
    DelayExecution = 4,
    Suspended = 5,
    UserRequest = 6,
    WrExecutive = 7,
    WrFreePage = 8,
    WrPageIn = 9,
    WrPoolAllocation = 10,
    WrDelayExecution = 11,
    WrSuspended = 12,
    WrUserRequest = 13,
    WrEventPair = 14,
    WrQueue = 15,
    WrLpcReceive = 16,
    WrLpcReply = 17,
    WrVirtualMemory = 18,
    WrPageOut = 19,
    WrRendezvous = 20,
    WrKeyedEvent = 21,
    WrTerminated = 22,
    WrProcessInSwap = 23,
    WrCpuRateControl = 24,
    WrCalloutStack = 25,
    WrKernel = 26,
    WrResource = 27,
    WrPushLock = 28,
    WrMutex = 29,
    WrQuantumEnd = 30,
    WrDispatchInt = 31,
    WrPreempted = 32,
    WrYieldExecution = 33,
    WrFastMutex = 34,
    WrGuardedMutex = 35,
    WrRundown = 36,
    WrAlertByThreadId = 37,
    WrDeferredPreempt = 38,
    MaximumWaitReason = 39
};
