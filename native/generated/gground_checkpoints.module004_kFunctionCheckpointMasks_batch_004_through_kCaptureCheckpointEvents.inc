inline constexpr std::array<std::uint32_t, 41> kFunctionCheckpointMasks_batch_004{{
 // function 599
    0x00000000U, // function 600
    0x00000000U, // function 601
    0x00000000U, // function 602
    0x00000000U, // function 603
    0x00000000U, // function 604
    0x00000000U, // function 605
    0x00000000U, // function 606
    0x00000000U, // function 607
    0x00000000U, // function 608
    0x00000000U, // function 609
    0x00000000U, // function 610
    0x00000000U, // function 611
    0x00000000U, // function 612
    0x00000000U, // function 613
    0x00000000U, // function 614
    0x00000000U, // function 615
    0x00000000U, // function 616
    0x00000000U, // function 617
    0x00000000U, // function 618
    0x00000000U, // function 619
    0x00000000U, // function 620
    0x00000000U, // function 621
    0x00000000U, // function 622
    0x00000000U, // function 623
    0x00000000U, // function 624
    0x00000000U, // function 625
    0x00000000U, // function 626
    0x00000000U, // function 627
    0x00000000U, // function 628
    0x00000000U, // function 629
    0x00000000U, // function 630
    0x00000000U, // function 631
    0x00000000U, // function 632
    0x00000000U, // function 633
    0x00000000U, // function 634
    0x00000000U, // function 635
    0x00000000U, // function 636
    0x00000000U, // function 637
    0x00000000U, // function 638
    0x00000000U, // function 639
    0x00000000U, // function 640
}};
inline constexpr auto kFunctionCheckpointMasks = ::gain_ground::source_layout_detail::concatenate_arrays(
    kFunctionCheckpointMasks_batch_000,
    kFunctionCheckpointMasks_batch_001,
    kFunctionCheckpointMasks_batch_002,
    kFunctionCheckpointMasks_batch_003,
    kFunctionCheckpointMasks_batch_004);


inline constexpr std::array<CaptureCheckpointEventContract, 13> kCaptureCheckpointEvents{{
    CaptureCheckpointEventContract{ 2U, 65535U, 8U, "machineReset", "initialization" },
    CaptureCheckpointEventContract{ 3U, 65535U, 0U, "memorySnapshot", "initialization" },
    CaptureCheckpointEventContract{ 5U, 5U, 32U, "memoryWrite", "paletteCommit" },
    CaptureCheckpointEventContract{ 6U, 65535U, 28U, "inputSample", "inputSample" },
    CaptureCheckpointEventContract{ 7U, 65535U, 16U, "ym2151Write", "audioRegisterStream" },
    CaptureCheckpointEventContract{ 8U, 65535U, 16U, "dacSample", "audioRegisterStream" },
    CaptureCheckpointEventContract{ 9U, 65535U, 186U, "frameBegin", "gameFrameBegin" },
    CaptureCheckpointEventContract{ 10U, 65535U, 148U, "tileSubmission", "drawInventory" },
    CaptureCheckpointEventContract{ 11U, 65535U, 148U, "spriteSubmission", "drawInventory" },
    CaptureCheckpointEventContract{ 12U, 65535U, 122U, "frameEnd", "gameFrameEndAndFramebuffer" },
    CaptureCheckpointEventContract{ 13U, 65535U, 36U, "presentation", "presentationCadence" },
    CaptureCheckpointEventContract{ 16U, 65535U, 24U, "freeCounterSample", "timerSample" },
    CaptureCheckpointEventContract{ 17U, 65535U, 34U, "tileDispatch", "drawInventory" },
}};

inline constexpr std::uint32_t kAssetSetCheckpointFirst = 1U;
inline constexpr std::uint32_t kAssetSetCheckpointLast = 42U;

